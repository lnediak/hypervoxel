#ifndef TERRAIN_GENERATOR_TESTER_HPP_
#define TERRAIN_GENERATOR_TESTER_HPP_

#include "vector.hpp"
#include <type_traits>

namespace hypervoxel {

template <std::size_t N> struct TerrainGeneratorTester {

  int mod, off;

  template <std::size_t M, class = typename std::enable_if<M >= N>::type>
  std::uint32_t operator()(const v::IVec<M> &coord) const {
    return 0xFFFFFFFFU * (get(coord) % mod == 1);
  }

  template <std::size_t M> std::size_t get(const v::IVec<M> &coord) const {
    std::size_t prevResult = TerrainGeneratorTester<N - 1>{mod, off}.get(coord);
    return prevResult * off + coord[N - 1];
  }
};

template <> struct TerrainGeneratorTester<1> {

  int mod, off;

  template <std::size_t M, class = typename std::enable_if<M >= 1>::type>
  std::uint32_t operator()(const v::IVec<M> &coord) const {
    return 0xFFFFFFFFU * (get(coord) % mod == 1);
  }

  template <std::size_t M> std::size_t get(const v::IVec<M> &coord) const {
    return coord[0];
  }
};

} // namespace hypervoxel

#endif // TERRAIN_GENERATOR_TESTER_HPP_

