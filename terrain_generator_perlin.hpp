#ifndef TERRAIN_GENERATOR_PERLIN_HPP_
#define TERRAIN_GENERATOR_PERLIN_HPP_

#include <type_traits>

#include "vector.hpp"

namespace hypervoxel {

template <std::size_t N> class TerrainGeneratorPerlin {

  struct GetResult {
    typedef void thisisavvec;
    typedef double value_type;
    static const std::size_t size = 1 << N;

    double *gradVecs;
    std::size_t numGradVecsMask;
    std::size_t octave;
    const v::IVec<N> &posf;
    const double *vecs[2];

    value_type operator[](std::size_t i) const {
      const v::IVec<N> bits = BitsVec{i};
      const SplitVec svecs{bits, {vecs[0], vecs[1]}};
      const v::DVec<N> gvec(
          gradVecs +
          N * ((v::IVecHash<N>{}(posf + bits) + octave) & numGradVecsMask));
      return v::sum(svecs * gvec);
    }
  };

public:
  typedef bool blockdata;

  struct Options {
    v::DVec<N> scale;
    double *gradVecs;
    std::size_t numGradVecsMask;
    std::size_t numOctaves;
    double persistence;
  } options;

  blockdata operator()(const v::IVec<N> &coord) const {
    return get(coord) > 0.3;
  }

  double get(const v::IVec<N> &coord) const {
    double total = 0;
    double amplitude = 1;
    v::DVec<N> pos = (v::toDVec(coord) + 0.5) / options.scale;
    for (std::size_t i = options.numOctaves; i--;) {
      pos += 0.5;
      v::IVec<N> posf = v::DVecFloor<N>{pos};
      v::DVec<N> vec0 = pos - v::toDVec(posf);
      v::DVec<N> vec1 = vec0 - 1.;
      v::DVec<N> lerp = vec0 * vec0 * (3. - 2. * vec0);
      total += vlerpAll(GetResult{options.gradVecs,
                                  options.numGradVecsMask,
                                  i,
                                  posf,
                                  {vec0.data, vec1.data}},
                        lerp) *
               amplitude;
      amplitude *= options.persistence;
      pos *= 2.;
    }
    return total;
  }
};

} // namespace hypervoxel

#endif // TERRAIN_GENERATOR_PERLIN_HPP_

