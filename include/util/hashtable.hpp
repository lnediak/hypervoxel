#ifndef HYPERVOXEL_HASHTABLE_HPP_
#define HYPERVOXEL_HASHTABLE_HPP_

#include <atomic>
#include <memory>
#include <mutex>
#include <utility>

namespace hypervoxel {

template <class K, class V, class Hash, class Eq>
struct HashMapNoResize {

  typedef std::pair<K, V> value_type;

  static const std::size_t hashmask = std::size_t(1) << 31;

  struct Entry {
    value_type value;
    std::size_t hash; /// hash = (real hash) | hashmask, and unoccupied is 0

    Entry() : value{}, hash{0} {}
  };

  std::size_t size;
  std::size_t sizeMask;
  std::unique_ptr<Entry[]> table;
  Hash hasher;
  Eq eqer;

  explicit HashMapNoResize(std::size_t sizeBits)
      : size(1 << sizeBits), sizeMask(size - 1),
        table(new Entry[size]), hasher{}, eqer{} {}

  void clear() {
    for (std::size_t i = 0; i < size; i++) {
      table[i].hash = 0;
    }
  }

  template <class F>
  decltype(std::declval<F>()(std::declval<V &>(), false))
  findAndRun(const K &k, F &&functor) {
    std::size_t kh = hasher(k);
    std::size_t khm = kh | hashmask;
    Entry *tptr = table.get();
    for (std::size_t i = kh & sizeMask;; i = (i + 1) & sizeMask) {
      Entry *tmp = tptr + i;
      std::size_t hash = tmp->hash;
      if (hash == 0) {
        tmp->hash = khm;
        tmp->value.first = k;
        ::new (&tmp->value.second) V{};
        return functor(tmp->value.second, true);
      }
      if (hash == khm) {
        if (eqer(k, tmp->value.first)) {
          return functor(tmp->value.second, false);
        }
      }
    }
  }

  template <class F>
  decltype(std::declval<F>()(std::declval<V &>()))
  runIfFound(const K &k, F &&functor,
             const decltype(functor(std::declval<V &>())) &def) {
    std::size_t kh = hasher(k);
    std::size_t khm = kh | hashmask;
    Entry *tptr = table.get();
    for (std::size_t i = kh & sizeMask;; i = (i + 1) & sizeMask) {
      Entry *tmp = tptr + i;
      std::size_t hash = tmp->hash;
      if (hash == 0) {
        return def;
      }
      if (hash == khm) {
        if (eqer(k, tmp->value.first)) {
          return functor(tmp->value.second);
        }
      }
    }
  }
};

template <class T> struct CondVal {
  char val[sizeof(T)];
  bool valid;

  CondVal() : valid{false} {}
  CondVal(T &&rval) : valid{true} { *reinterpret_cast<T *>(val) = rval; }

  T operator()() { return *reinterpret_cast<T *>(val); }
};
template <> struct CondVal<void> {
  bool valid;

  CondVal() : valid{false} {}
  CondVal(int) : valid{true} {}

  void operator()() {}
};
template <class T, class F, class... Args> struct EvalCondVal {
  CondVal<T> operator()(F &&fun, Args &&... args) { return fun(args...); }
};
template <class F, class... Args> struct EvalCondVal<void, F, Args...> {
  CondVal<void> operator()(F &&fun, Args &&... args) {
    fun(args...);
    return {};
  }
};
template <class T> struct WrapVal {
  T val;
  WrapVal(const T &val) : val(val) {}
  WrapVal(T &&val) : val(val) {}

  T operator()() { return val; }
};
template <> struct WrapVal<void> {
  void operator()() {}
};
template <class T, class F, class... Args> struct EvalWrapVal {
  WrapVal<T> operator()(F &&fun, Args &&... args) { return fun(args...); }
};
template <class F, class... Args> struct EvalWrapVal<void, F, Args...> {
  WrapVal<void> operator()(F &&fun, Args &&... args) {
    fun(args...);
    return {};
  }
};

template <class K, class V, class Hash, class Eqer> class Cacher {

  typedef HashMapNoResize<K, V, Hash, Eqer> table_t;
  table_t tables[2];
  std::atomic<std::size_t> sizes[2];
  /// table_t *main = &tables[flags & 1]
  /// table_t *other = &tables[!(flags & 1)]
  /// table_t *back = (flags & 2)? other : nullptr
  std::atomic<int> flags;
  std::size_t minSize, maxSize;

public:
  Cacher(std::size_t sizeBits, std::size_t minSize, std::size_t maxSize)
      : tables{table_t(sizeBits), table_t(sizeBits)}, sizes{{0}, {0}}, flags{0},
        minSize(minSize), maxSize(maxSize) {}

  template <class F>
  decltype(std::declval<F>()(std::declval<V &>(), false))
  findAndRun(const K &k, F &&functor) {
    typedef decltype(functor(std::declval<V &>(), false)) ret_t;
    int flagsl = flags.load(std::memory_order_relaxed);
    int flagsl1 = flagsl & 1;
    int flagsl0 = !flagsl1;
    table_t *mp = &tables[flagsl1];
    std::atomic<std::size_t> *ms = &sizes[flagsl1];
    table_t *other = &tables[flagsl0];
    std::atomic<std::size_t> *os = &sizes[flagsl0];
    if (flagsl & 2) {
      CondVal<ret_t> val = mp->runIfFound(
          k,
          [&functor](V &val) -> CondVal<ret_t> {
            return EvalCondVal<ret_t, F &, V &, bool>{}(functor, val, false);
          },
          {});
      if (val.valid) {
        return val();
      }
      bool needClearM = false;
      WrapVal<ret_t> toreturn = other->findAndRun(
          k,
          [this, &needClearM, &os, &functor](V &val,
                                             bool isNew) -> WrapVal<ret_t> {
            if (isNew) {
              needClearM =
                  os->fetch_add(1, std::memory_order_relaxed) >= minSize;
            }
            return EvalWrapVal<ret_t, F &, V &, bool &>{}(functor, val, isNew);
          });
      if (needClearM) {
        if (flags.compare_exchange_strong(flagsl, flagsl0,
                                          std::memory_order_relaxed)) {
          ms->store(0, std::memory_order_relaxed);
          mp->clear();
        }
      }
      return toreturn();
    }
    return mp->findAndRun(
        k,
        [this, flagsl, flagsl0, mp, ms, &functor](V &val, bool isNew) -> ret_t {
          if (isNew) {
            std::size_t cms = ms->fetch_add(1, std::memory_order_relaxed);
            if (cms >= maxSize) {
              flags.store(flagsl | 2, std::memory_order_relaxed);
            }
          }
          return functor(val, isNew);
        });
  }

  template <class F>
  decltype(std::declval<F>()(std::declval<V &>()))
  runIfFound(const K &k, F &&functor,
             const decltype(functor(std::declval<V &>())) &def) {
    typedef decltype(functor(std::declval<V &>())) ret_t;
    int flagsl = flags.load(std::memory_order_relaxed);
    int flagsl1 = flagsl & 1;
    int flagsl0 = !flagsl1;
    table_t *mp = &tables[flagsl1];
    std::atomic<std::size_t> *ms = &sizes[flagsl1];
    table_t *other = &tables[flagsl0];
    std::atomic<std::size_t> *os = &sizes[flagsl0];
    if (flagsl & 2) {
      CondVal<ret_t> val =
          mp->runIfFound(k,
                         [&functor](V &val) -> CondVal<ret_t> {
                           return EvalCondVal<ret_t, F &, V &>{}(functor, val);
                         },
                         {});
      if (val.valid) {
        return *reinterpret_cast<ret_t *>(val.val);
      }
      return other->runIfFound(k,
                               [this, flagsl, flagsl0, mp, ms, os, &functor](
                                   V &val) -> ret_t { return functor(val); },
                               def);
    }
    return mp->runIfFound(k,
                          [this, flagsl, flagsl0, mp, ms,
                           &functor](V &val) -> ret_t { return functor(val); },
                          def);
  }
};

} // namespace hypervoxel

#endif // HYPERVOXEL_HASHTABLE_HPP_

