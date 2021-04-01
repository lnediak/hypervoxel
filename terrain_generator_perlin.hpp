#ifndef TERRAIN_GENERATOR_PERLIN_HPP_
#define TERRAIN_GENERATOR_PERLIN_HPP_

#include <type_traits>

#include "primitives.hpp"

namespace hypervoxel {

template <std::size_t N> struct BBlockdata {

  bool val;

  Color getColor(std::size_t dir) const {
    std::size_t dim = dir >= N ? dir - N : dir;
    Color toreturn = {0, 0, 0, 0};
    if (!val) {
      return toreturn;
    }
    toreturn.a = 1;
    switch (dim % 4) {
    case 0:
      toreturn.r = ((dim + 1. + N) / (2. * N));
      break;
    case 1:
      toreturn.g = ((dim + 1. + N) / (2. * N));
      break;
    case 2:
      toreturn.b = ((dim + 1. + N) / (2. * N));
      break;
    default:
      toreturn.r = ((dim + 1. + N) / (2. * N));
      toreturn.g = ((dim + 1. + N) / (2. * N));
      toreturn.b = ((dim + 1. + N) / (2. * N));
    }
    return toreturn;
  }

  bool isOpaque() const { return val; }

  bool isVisible() const { return val; }
};

template <std::size_t N> class TerrainGeneratorPerlin {

  struct BitsVec {
    typedef void thisisavvec;
    typedef std::int32_t value_type;
    static const std::size_t size = N;

    std::size_t value;

    value_type operator[](std::size_t i) const {
      return (value & (1 << i)) >> i;
    }
  };
  struct SplitVec {
    typedef void thisisavvec;
    typedef double value_type;
    static const std::size_t size = N;

    const v::IVec<N> &inds;
    const double *vecs[2];

    value_type operator[](std::size_t i) const {
      return vecs[inds[i]][i];
    }
  };
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
      const v::DVec<N> gvec{
          gradVecs +
          N * ((v::IVecHash<N>{}(posf + bits) + octave) & numGradVecsMask)};
      return v::sum(svecs * gvec);
    }
  };
  template <std::size_t M, std::size_t I, class A> struct Lerper {
    typedef void thisisavvec;
    typedef double value_type;
    static const std::size_t size = 1 << (M - 1);

    A tolerp;
    const v::DVec<N> &lerp;

    value_type operator[](std::size_t i) const {
      double x = tolerp[i];
      double y = tolerp[i + size];
      return x + lerp[I] * (y - x);
    }
  };
  template <std::size_t M, std::size_t I, class A> struct LerperT {
    typedef LerperT<M + 1, I - 1, A> nextlerpert;
    typedef Lerper<M, I, typename nextlerpert::nextlerper> nextlerper;

    nextlerper operator()(A &&tolerp, const v::DVec<N> &lerp) const {
      return {nextlerpert{}(std::move(tolerp), lerp), lerp};
    }
  };
  template <std::size_t M, class A> struct LerperT<M, 0, A> {
    typedef Lerper<M, 0, A> nextlerper;

    nextlerper operator()(A &&tolerp, const v::DVec<N> &lerp) const {
      return {std::move(tolerp), lerp};
    }
  };

public:
  typedef BBlockdata<N> blockdata;

  struct Options {
    v::DVec<N> scale;
    double *gradVecs;
    std::size_t numGradVecsMask;
    std::size_t numOctaves;
    double persistence;
  } options;

  blockdata operator()(const v::IVec<N> &coord) const {
    return {get(coord) > 0.3};
  }

  double get(const v::IVec<N> &coord) const {
    double total = 0;
    double amplitude = 1;
    v::DVec<N> pos = (v::toDVec(coord) + 0.5) / options.scale;
    for (std::size_t i = options.numOctaves; i--;) {
      v::IVec<N> posf = v::DVecFloor<N>{pos};
      v::DVec<N> vec0 = pos - v::toDVec(posf);
      v::DVec<N> vec1 = vec0 - 1.;
      v::DVec<N> lerp = vec0 * vec0 * (3. - 2. * vec0);
      total += LerperT<1, N - 1, GetResult>{}(GetResult{options.gradVecs,
                                                        options.numGradVecsMask,
                                                        i,
                                                        posf,
                                                        {vec0.data, vec1.data}},
                                              lerp)[0] *
               amplitude;
      amplitude *= options.persistence;
      pos *= 2.;
    }
    return total;
  }
};

} // namespace hypervoxel

#endif // TERRAIN_GENERATOR_PERLIN_HPP_

