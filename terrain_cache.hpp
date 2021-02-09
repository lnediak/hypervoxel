#ifndef HYPERVOXEL_TERRAIN_CACHE_HPP_
#define HYPERVOXEL_TERRAIN_CACHE_HPP_

#include <unordered_map>

#include "pool_linked_list.hpp"
#include "vector.hpp"

namespace hypervoxel {

template <std::size_t N, class TerGen> class TerrainCache {

  typedef typename TerGen::blockdata BData;

  struct CacheEntry {
    BData blockdata;
    /* plist::Node */ void *listEntry = nullptr;
    CacheEntry(BData blockdata, void *listEntry)
        : blockdata(blockdata), listEntry(listEntry) {}
  };

  typedef std::unordered_map<v::IVec<N>, CacheEntry, v::IVecHash<N>,
                             v::EqualFunctor<v::IVec<N>, v::IVec<N>>>
      umap;
  typedef PoolLinkedList<typename umap::iterator> plist;

  TerGen terGen;
  umap cache;
  plist cachelist;
  std::size_t maxSize;

  /// Adds val to cachelist by removing back if full
  typename plist::Node *addToListSafe(typename umap::iterator val) {
    if (cachelist.invSize()) {
      return cachelist.addToBeg(val);
    }
    auto oldEntry = cachelist.back();
    cache.erase(oldEntry->obj);
    cachelist.remove(oldEntry);
    return cachelist.addToBeg(val);
  }

public:
  TerrainCache(const TerGen &terGen, std::size_t maxSize)
      : terGen(terGen), cache(static_cast<std::size_t>(maxSize * 1.23)),
        cachelist(maxSize), maxSize(maxSize) {}

  void replaceCacheEntry(const v::IVec<N> &coord, BData blockdata) {
    auto iter = cache.emplace(coord, {}).first;
    iter->second.blockdata = blockdata;
    if (iter->second.listEntry) {
      cachelist.remove(
          static_cast<typename plist::Node *>(iter->second.listEntry));
      iter->second.listEntry = cachelist.addToBeg(iter);
      return;
    }
    iter->second.listEntry = addToListSafe(iter);
  }

  void insertCacheEntry(const v::IVec<N> &coord, BData blockdata) {
    auto iter = cache.emplace(coord, CacheEntry{blockdata, nullptr}).first;
    if (iter->second.listEntry) {
      cachelist.remove(
          static_cast<typename plist::Node *>(iter->second.listEntry));
      iter->second.listEntry = cachelist.addToBeg(iter);
      return;
    }
    iter->second.listEntry = addToListSafe(iter);
  }

  BData operator()(const v::IVec<N> &coord) {
    auto iter = cache.find(coord);
    if (iter == cache.end()) {
      BData toreturn = terGen(coord);
      insertCacheEntry(coord, toreturn);
      return toreturn;
    }
    return iter->second.blockdata;
  }

  BData *peek(const v::IVec<N> &coord) const {
    auto iter = cache.find(coord);
    if (iter == cache.end()) {
      return nullptr;
    }
    return &iter->second.blockdata;
  }
};

} // namespace hypervoxel

#endif // HYPERVOXEL_TERRAIN_CACHE_HPP_

