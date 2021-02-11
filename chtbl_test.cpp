#include <functional>
#include <iostream>
#include <thread>

#include "concurrent_hashtable.hpp"

template <class CMap> struct ThreadFun {

  CMap &map;
  std::size_t i;

  void operator()() {
    for (std::size_t kk = 1000; kk--;) {
      if (i + kk == 1000000) {
        std::cout << "WHOOOHOOOO!!! IMPOSSIBLE!!!! L33T H4X0R!!!!!!" << std::endl;
      }
    }
    for (std::size_t j = 10; j--;) {
      int val = i + j * 100;
      map.findAndRun(256 * i, [this, val](std::atomic<int> &v) -> void {
        v.store(val, std::memory_order_relaxed);
      });
      map.findAndRun(256 * i + 131072, [this, val](std::atomic<int> &v) -> void {
        v.store(val, std::memory_order_relaxed);
      });
    }
  };
};

int main() {
  std::ios_base::sync_with_stdio(false);
  const std::size_t numThreads = 64;
  const std::size_t sizeBits = 8;

  hypervoxel::ConcurrentHashMapNoResize<int, std::atomic<int>, std::hash<int>,
                                        std::equal_to<int>>
      map(sizeBits);
  std::thread ts[numThreads];
  for (std::size_t i = numThreads; i--;) {
    ts[i] = std::thread(ThreadFun<decltype(map)>{map, i + 1});
  }
  ThreadFun<decltype(map)>{map, numThreads + 1}();
  for (std::size_t i = numThreads; i--;) {
    ts[i].join();
  }
  std::size_t count = 0;
  for (std::size_t i = 0; i < map.size; i++) {
    auto &e = map.table[i];
    std::cout << "value: " << e.value.first << " " << e.value.second
              << std::endl;
    std::size_t hash = e.hash.load(std::memory_order_relaxed);
    count += !!hash;
    std::cout << "hash: " << hash << std::endl;
  }
  std::cout << "TOTAL ELEMENTS: " << count << std::endl;
}

