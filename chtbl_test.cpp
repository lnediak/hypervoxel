#include <functional>
#include <iostream>
#include <thread>

#include "concurrent_hashtable.hpp"

template <class CMap> struct ThreadFun {

  CMap &map;
  std::size_t i;

  void operator()() {
    for (std::size_t j = 1000; j--;) {
      map[10000].store(j + i * 1000000, std::memory_order_relaxed);
    }
  };
};

int main() {
  const std::size_t numThreads = 64;
  const std::size_t sizeBits = 4;

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
  for (std::size_t i = 0; i < map.size; i++) {
    auto &e = map.table[i];
    std::cout << "value: " << e.value.first << " " << e.value.second
              << std::endl;
    std::cout << "hash: " << e.hash.load(std::memory_order_relaxed)
              << std::endl;
  }
}

