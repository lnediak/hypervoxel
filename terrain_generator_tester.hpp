#ifndef TERRAIN_GENERATOR_TESTER_HPP_
#define TERRAIN_GENERATOR_TESTER_HPP_

#include <type_traits>

#include "primitives.hpp"

namespace hypervoxel {

template <std::size_t N> struct BoolBlockdata {

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

/*
template <std::size_t N, class = typename std::enable_if<N == 4>::type>
struct TerrainGeneratorTester {

  typedef BoolBlockdata<4> blockdata;
  int mod, off;

  blockdata operator()(const v::IVec<4> &coord) const {
    return {coord[0] == coord[2] + 1 && coord[1] == coord[2] - 3 &&
            coord[2] == coord[3] + 2};
  }
};
*/

/*
template <std::size_t N, class = typename std::enable_if<N == 3>::type>
struct TerrainGeneratorTester {

  typedef BoolBlockdata<3> blockdata;

  blockdata operator()(const v::IVec<3> &coord) const {
    return {coord[0] == coord[2] && coord[1] == coord[2]};
  }
};
*/

template <std::size_t N> struct TerrainGeneratorTester {

  typedef BoolBlockdata<N> blockdata;
  int mod, off;

  blockdata operator()(const v::IVec<N> &coord) const {
    return {get(coord) % mod == 1};
  }

  template <std::size_t M> std::size_t get(const v::IVec<M> &coord) const {
    std::size_t prevResult = TerrainGeneratorTester<N - 1>{mod, off}.get(coord);
    return prevResult * off + coord[N - 1];
  }
};

template <> struct TerrainGeneratorTester<1> {

  typedef BoolBlockdata<1> blockdata;
  int mod, off;

  blockdata operator()(const v::IVec<1> &coord) const {
    return {get(coord) % mod == 1};
  }

  template <std::size_t M> std::size_t get(const v::IVec<M> &coord) const {
    return coord[0];
  }
};

} // namespace hypervoxel

#endif // TERRAIN_GENERATOR_TESTER_HPP_

