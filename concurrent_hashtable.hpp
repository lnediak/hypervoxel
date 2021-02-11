#ifndef HYPERVOXEL_CONCURRENT_HASHTABLE_HPP_
#define HYPERVOXEL_CONCURRENT_HASHTABLE_HPP_

#include <atomic>
#include <memory>
#include <mutex>
#include <utility>

namespace hypervoxel {

template <class K, class V, class Hash, class Eq, class Mutex = std::mutex>
struct ConcurrentHashMapNoResize {

  typedef std::pair<K, V> value_type;

  static const std::size_t hashmask = std::size_t(1) << 31;

  struct Entry {
    Mutex lock;       /// Applies to r/w on value, just w on hash
    value_type value; /// hash = (real hash) | hashmask, and unoccupied is 0
    std::atomic<std::size_t> hash;

    Entry() : lock{}, value{}, hash{0} {}
  };

  std::size_t size;
  std::size_t sizeMask;
  std::unique_ptr<Entry[]> table;
  Hash hasher;
  Eq eqer;

  explicit ConcurrentHashMapNoResize(std::size_t sizeBits)
      : size(1 << sizeBits), sizeMask(size - 1),
        table(new Entry[size]), hasher{}, eqer{} {}

  /// run acquireClear after running this
  void clearNotThreadsafe() {
    for (std::size_t i = 0; i < size; i++) {
      table[i].hash.store(0, std::memory_order_relaxed);
    }
    std::atomic_thread_fence(std::memory_order_release);
  }

  /// run this in each thread after running clearNotThreadsafe
  void acquireClear() { std::atomic_thread_fence(std::memory_order_acquire); }

  template <class F>
  decltype(std::declval<F>()(std::declval<V &>())) findAndRun(const K &k,
                                                              F &&functor) {
    std::size_t kh = hasher(k);
    std::size_t khm = kh | hashmask;
    Entry *tptr = table.get();
    for (std::size_t i = kh & sizeMask;; i = (i + 1) & sizeMask) {
      Entry *tmp = tptr + i;
      std::size_t hash = tmp->hash.load(std::memory_order_relaxed);
      if (hash == 0) {
        std::unique_lock<Mutex> ll(tmp->lock);
        hash = tmp->hash.load(std::memory_order_relaxed);
        if ((hash != 0) && (hash != khm)) {
          // Another thread has stolen this bucket for their own key
          continue;
        } else if (hash == 0) {
          tmp->hash.store(khm, std::memory_order_relaxed);
          tmp->value.first = k;
          ::new (&tmp->value.second) V{};
        }
        return functor(tmp->value.second);
      }
      if (hash == khm) {
        std::unique_lock<Mutex> ll(tmp->lock);
        if (eqer(k, tmp->value.first)) {
          return functor(tmp->value.second);
        }
      }
    }
  }
};

} // namespace hypervoxel

#endif // HYPERVOXEL_CONCURRENT_HASHTABLE_HPP_

