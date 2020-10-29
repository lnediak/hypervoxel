#ifndef HYPERVOXEL_TERRAIN_MANAGER_HPP_
#define HYPERVOXEL_TERRAIN_MANAGER_HPP_

#include <cstdint>
#include <thread>
#include <unordered_map>

#include "pool_linked_list.hpp"
#include "terrain_generator.hpp"
#include "vector.hpp"

namespace hypervoxel {

template <std::size_t N, template <std::size_t> G> class TerrainManager {

  struct CacheEntry;

  typedef std::unordered_map<IVec<N>, CacheEntry, IVecHash<N>> umap;
  typedef PoolLinkedList<umap::iterator> plist;

  struct CacheEntry {
    std::uint32_t color = 0;
    plist::Node *listEntry = nullptr;
    CacheEntry(std::uint32_t color, plist::Node *listEntry)
        : color(color), listEntry(listEntry) {}
  };

  G<N> terGen;
  umap cache;
  plist cachelist;
  std::size_t maxSize;

  plist::Node *bumpCacheList(umap::value_type val) {
    if (cachelist.invSize()) {
      return cachelist.addToBeg(val);
    }
    auto oldEntry = cachelist.back();
    cache.erase(oldEntry->obj);
    cachelist.remove(oldEntry);
    return cachelist.addToBeg(val);
  }

public:
  TerrainManager(G<N> &&terGen, std::size_t maxSize)
      : terGen(terGen), cache(maxSize * 2), cachelist(maxSize),
        maxSize(maxSize) {}

  std::uint32_t *getCacheEntry(const IVec<N> &coord) const {
    auto iter = cache.find(coord);
    if (iter == cache.end()) {
      return nullptr;
    }
    return &iter->second.color;
  }

  void replaceCacheEntry(const IVec<N> &coord, std::uint32_t color) {
    auto iter = cache.emplace(coord, {}).first;
    iter->second.color = color;
    if (iter->second.listEntry) {
      cachelist.remove(iter->second.listEntry);
      iter->second.listEntry = cachlist.addToBeg(iter);
      return;
    }
    iter->second.listEntry = bumpCacheList(iter);
  }

  void insertCacheEntry(const IVec<N> &coord, std::uint32_t color) {
    auto val = cache.emplace(coord, {color, nullptr});
    if (val.second) {
      val.first->second.listEntry = bumpCacheList(iter);
    }
  }

  /// forward, right, and up have to be orthonormal to each other
  void generateAll(double *forward, double *right, double *up, double width,
                   double height, std::uint32_t dist1, std::uint32_t dist2) {
    //
  }
};

} // namespace hypervoxel

#endif // HYPERVOXEL_TERRAIN_MANAGER_HPP_

