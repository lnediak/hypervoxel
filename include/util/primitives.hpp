#ifndef HYPERVOXEL_PRIMITIVES_HPP_
#define HYPERVOXEL_PRIMITIVES_HPP_

#include "vector.hpp"

namespace hypervoxel {

template <std::size_t N> struct Line {
  v::DVec<N> a, b;
  v::DVec<3> a3, b3;
  std::size_t dim1 = 0, dim2 = 0;
};

template <std::size_t N> struct Slice {
  v::DVec<N> cam, right, up, forward;
  double width2, height2;
  int x, y, w, h;
};

template <std::size_t N> struct SlicesSpec {
  std::size_t numSlices;
  std::unique_ptr<Slice<N>[]> slices;

  explicit SlicesSpec(std::size_t numSlices)
      : numSlices(numSlices), slices(new Slice<N>[numSlices]) {}
};

struct Color {
  float r, g, b, a;
};

inline int ceilLog2(std::uint64_t v) {
  // Credit: https://stackoverflow.com/a/11398748
  const int tab64[64] = {64, 1,  59, 2,  60, 48, 54, 3,  61, 40, 49, 28, 55,
                         34, 43, 4,  62, 52, 38, 41, 50, 19, 29, 21, 56, 31,
                         35, 12, 44, 15, 23, 5,  63, 58, 47, 53, 39, 27, 33,
                         42, 51, 37, 18, 20, 30, 11, 14, 22, 57, 46, 26, 32,
                         36, 17, 10, 13, 45, 25, 16, 9,  24, 8,  7,  6};
  v--;
  v |= v >> 1;
  v |= v >> 2;
  v |= v >> 4;
  v |= v >> 8;
  v |= v >> 16;
  v |= v >> 32;
  return tab64[((std::uint64_t)((v - (v >> 1)) * 0x07EDD5E59A4E28C2)) >> 58];
}

} // namespace hypervoxel

#endif // HYPERVOXEL_PRIMITIVES_HPP_

