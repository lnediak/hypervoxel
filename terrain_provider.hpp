#ifndef HYPERVOXEL_TERRAIN_PROVIDER_HPP_
#define HYPERVOXEL_TERRAIN_PROVIDER_HPP_

#include <memory>

#include "terrain_indexer.hpp"

namespace hypervoxel {

struct Float32 {
  float s[32];
};

#define LERP_SCALE 16
#define LERP_SCALEf 16f

class TerrainProvider {

  TerrainIndexer ti;
  std::unique_ptr<Float32[]> td;

  v::IVec<5> sb;   // scaled version of current block
  v::IVec<5> sbs;  // scuff-scaled version (sb * LERP_SCALE)
  std::size_t tii; // index in ti

  float hit = 0;

  static SliceDirs scaleSliceDirs(const SliceDirs &sd) {
    SliceDirs ret;
    ret.c = sd.c / LERP_SCALE;
    ret.r = sd.r / LERP_SCALE;
    ret.u = sd.u / LERP_SCALE;
    ret.f = sd.f / LERP_SCALE;
    ret.rm = sd.rm;
    ret.um = sd.um;
    ret.fm = sd.fm / LERP_SCALE;
    return ret;
  }

public:
  TerrainProvider(const SliceDirs &sd) : ti(scaleSliceDirs(sd), 1, false) {
    int farOff = fastFloor(1 + sd.fm / LERP_SCALE);
    std::size_t tdlen = 32 * farOff * farOff * farOff;
    td.reset(new Float32[tdlen]);
    std::memset(td.get(), 0, sizeof(Float32) * tdlen);
    sb = vfloor(sd.c) / LERP_SCALE;
    sbs = sb * LERP_SCALE;
    tii = ti.getIndex(sb);
  }

  bool operator()(v::IVec<5> cb, std::size_t dim, int sig) {
    const int sig0a[2] = {1, 0, 0};
    if (!((cb[dim] + sig0a[sig + 1]) & 0xF)) {
      sb[dim] += sig;
      tii = ti.getIndex(sb);
      sbs[dim] += LERP_SCALE * sig;
    }

    Float32 *p = td[tii];
    struct Splitter {
      typedef void thisisavvec;
      typedef float value_type;
      static const std::size_t size = 32;
      const Float32 *p;
      value_type operator[](std::size_t i) const { return p.s[i]; }
    };
    v::FVec<5> dfs = (cb - sbs) / LERP_SCALEf;
    v::FVec<5> lerp = dfs * dfs * (3. - 2. * dfs);
    float result = vlerpAll(Splitter{p}, lerp);
    if (result > 0) {
      hit = result;
      return true;
    }
  }
};

} // namespace hypervoxel

#endif // HYPERVOXEL_TERRAIN_PROVIDER_HPP_

