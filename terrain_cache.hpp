#ifndef HYPERVOXEL_TERRAIN_CACHE_HPP_
#define HYPERVOXEL_TERRAIN_CACHE_HPP_

#include <unordered_map>

#include "concurrent_hashtable.hpp"
#include "vector.hpp"

namespace hypervoxel {

template <std::size_t N, class TerGen> class TerrainCache {

  typedef typename TerGen::blockdata BData;

  typedef ConcurrentCacher<v::IVec<N>, BData, v::IVecHash<N>,
                           v::EqualFunctor<v::IVec<N>, v::IVec<N>>>
      umap;

  TerGen terGen;
  umap cache;

public:
  TerrainCache(TerGen &&terGen, std::size_t minSize, std::size_t maxSize)
      : terGen(terGen), cache(ceilLog2(maxSize) + 1, minSize, maxSize) {}

  void replaceCacheEntry(const v::IVec<N> &coord, BData blockdata) {
    cache.findAndRun(
        coord, [blockdata](BData &v, bool isNew) -> void { v = blockdata; });
  }

  void insertCacheEntry(const v::IVec<N> &coord, BData blockdata) {
    cache.findAndRun(coord, [blockdata](BData &v, bool isNew) -> void {
      if (isNew) {
        v = blockdata;
      }
    });
  }

  BData operator()(const v::IVec<N> &coord) {
    return cache.findAndRun(coord,
                            [this, &coord](BData &v, bool isNew) -> BData {
                              if (isNew) {
                                v = terGen(coord);
                              }
                              return v;
                            });
  }

  BData *peek(const v::IVec<N> &coord) const {
    return cache.runIfFound(coord, [](BData &v) -> BData * { return &v; },
                            nullptr);
  }
};

} // namespace hypervoxel

#endif // HYPERVOXEL_TERRAIN_CACHE_HPP_

