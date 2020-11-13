#ifndef HYPERVOXEL_TERRAIN_CACHE_HPP_
#define HYPERVOXEL_TERRAIN_CACHE_HPP_

#include <cstdint>
#include <thread>
#include <unordered_map>

#include "faces_manager.hpp"
#include "pool_linked_list.hpp"
#include "terrain_generator.hpp"
#include "vector.hpp"

namespace hypervoxel {

template <std::size_t N, template <std::size_t> G> class TerrainCache {

  struct CacheEntry;

  typedef std::unordered_map<v::IVec<N>, CacheEntry, v::IVecHash<N>> umap;
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

  /// Adds val to cachelist by removing back if full
  plist::Node *addToListSafe(umap::value_type val) {
    if (cachelist.invSize()) {
      return cachelist.addToBeg(val);
    }
    auto oldEntry = cachelist.back();
    cache.erase(oldEntry->obj);
    cachelist.remove(oldEntry);
    return cachelist.addToBeg(val);
  }

public:
  TerrainCache(G<N> &&terGen, std::size_t maxSize)
      : terGen(terGen), cache(maxSize * 2), cachelist(maxSize),
        maxSize(maxSize) {}

  void replaceCacheEntry(const v::IVec<N> &coord, std::uint32_t color) {
    auto iter = cache.emplace(coord, {}).first;
    iter->second.color = color;
    if (iter->second.listEntry) {
      cachelist.remove(iter->second.listEntry);
      iter->second.listEntry = cachelist.addToBeg(iter);
      return;
    }
    iter->second.listEntry = addToListSafe(iter);
  }

  void insertCacheEntry(const v::IVec<N> &coord, std::uint32_t color) {
    auto iter = cache.emplace(coord, {color, nullptr}).first;
    if (iter->second.listEntry) {
      cachelist.remove(iter->second.listEntry);
      iter->second.listEntry = cachelist.addToBeg(iter);
      return;
    }
    iter->second.listEntry = addToListSafe(iter);
  }

  std::uint32_t operator()(const v::IVec<N> &coord) const {
    auto iter = cache.find(coord);
    if (iter == cache.end()) {
      std::uint32_t toreturn = terGen(coord);
      insertCacheEntry(coord, toreturn);
      return toreturn;
    }
    return iter->second.color;
  }

  std::uint32_t *peek(const v::IVec<N> &coord) const {
    auto iter = cache.find(coord);
    if (iter == cache.end()) {
      return nullptr;
    }
    return &iter->second.color;
  }
};

} // namespace hypervoxel

#endif // HYPERVOXEL_TERRAIN_CACHE_HPP_

