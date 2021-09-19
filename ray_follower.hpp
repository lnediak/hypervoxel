#ifndef HYPERVOXEL_RAY_FOLLOWER_HPP_
#define HYPERVOXEL_RAY_FOLLOWER_HPP_

#include <cmath>

#include "util.hpp"

namespace hypervoxel {

template <std::size_t N, class Op> class RayFollower {

  v::FVec<N> c;   // camera
  Op op;          // operation
  v::FVec<N> ray; // ray

  struct FSign1 {
    static float apply(float a) { return a > 0 ? 1.00001 : -0.00001; }
  };
  struct FSign2 {
    static int apply(float a) { return a > 0 ? 1 : -1; }
  };

  float ffmax(float a, float b) { return a > b ? a : b; }

public:
  RayFollower(const SliceDirs<N> &sd, Op &&op, float r, float u)
      : c(sd.c), op(op), ray(r * sd.r + u * sd.u + sd.f) {
    float norm = std::sqrt(v::norm2(ray));
    ray *= sd.fm / norm;
  }

  void operator()() {
    v::FVec<N> cp;             // current point
    v::IVec<N> cb = vfloor(c); // current block
    float dist = 0;
    v::FVec<N> invs = 1.f / ray; // inverses
    v::FVec<N> fsig1 = v::UnaryOp<float, N, FSign1, v::FVec<N>>(invs);
    v::IVec<N> fsig2 = v::UnaryOp<int, N, FSign2, v::FVec<N>>(invs);
    int sigtmp = 0;
    EI<float> val{0.f, 0};
    while (true) {
      cp = c + dist * ray;
      if (op(cb, val.b, sigtmp)) {
        return;
      }
      val = vminI((fsig1 + vint2float(cb) - cp) * invs);
      dist += ffmax(val.a, 1e-5f);
      if (dist >= 1) {
        break;
      }
      cb[val.b] += sigtmp = fsig2[val.b];
    }
  }
};

} // namespace hypervoxel

#endif // HYPERVOXEL_RAY_FOLLOWER_HPP_

