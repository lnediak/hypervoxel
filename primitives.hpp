#ifndef HYPERVOXEL_PRIMITIVES_HPP_
#define HYPERVOXEL_PRIMITIVES_HPP_

#include "vector.hpp"

namespace hypervoxel {

template <std::size_t N> struct Line {
  v::DVec<N> a, b;
  v::DVec<3> a3, b3;
  std::size_t dim1 = 0, dim2 = 0;
};

template <std::size_t N> struct SliceDirs {
  v::DVec<N> cam, right, up, forward;
  double width2, height2;
};

struct Color {
  float r, g, b, a;
};

} // hypervoxel

#endif // HYPERVOXEL_PRIMITIVES_HPP_

