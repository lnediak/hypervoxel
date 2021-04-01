#ifndef HYPERVOXEL_FACES_MANAGER_HPP_
#define HYPERVOXEL_FACES_MANAGER_HPP_

#include <limits>
#include <unordered_map>
#include <utility>

#include "concurrent_hashtable.hpp"
#include "primitives.hpp"

namespace hypervoxel {

inline double ab(double a) { return a < 0 ? -a : a; }

#define MAX_THREADS 64

template <std::size_t N> class FacesManager {

  struct Face {

    v::IVec<N> c;
    std::size_t dim;

    constexpr bool operator==(const Face &other) const noexcept {
      return dim == other.dim && c == other.c;
    }
  };

  struct FaceHash {

    std::size_t operator()(const Face &f) const noexcept {
      std::size_t basic = v::IVecHash<N>()(f.c);
      return (((basic << (32 - f.dim)) | (basic >> f.dim)) ^ f.dim) + f.dim;
    }
  };

  struct Entry {
    Color color;
    std::size_t edgeCount = 0;
    v::DVec<3> edges[2 * N][2];
  };

  typedef ConcurrentHashMapNoResize<Face, Entry, FaceHash, std::equal_to<Face>>
      umap;

  umap map;
  std::size_t currSizes[MAX_THREADS]{0};
  std::size_t maxSize;
  std::size_t threadLocalMaxSize;
  v::DVec<N> cam;

  template <class GetColor>
  void storeEdge(const v::IVec<N> &coord, std::size_t dim,
                 const GetColor &getColor, const v::DVec<3> &a,
                 const v::DVec<3> &b, std::size_t threadi) {
    map.findAndRun(Face{coord, dim},
                   [this, &getColor, &a, &b, threadi](Entry &e, bool) -> void {
                     if (!e.edgeCount) {
                       currSizes[threadi]++;
                       e.color = getColor();
                     }
                     e.edges[e.edgeCount][0] = a;
                     e.edges[e.edgeCount++][1] = b;
                   });
  }

  template <class BlockData>
  void storeEdge(const v::IVec<N> &coord, std::size_t dim,
                 const BlockData &bdata, std::size_t param, const v::DVec<3> &a,
                 const v::DVec<3> &b, std::size_t threadi) {
    storeEdge(coord, dim,
              [&bdata, param]() -> Color { return bdata.getColor(param); }, a,
              b, threadi);
  }

public:
  FacesManager(std::size_t maxSize, std::size_t threadLocalMaxSize,
               const v::DVec<N> &cam)
      : map(ceilLog2(maxSize) + 1), maxSize(maxSize),
        threadLocalMaxSize(threadLocalMaxSize), cam(cam) {}

  FacesManager() : map(4), maxSize(8) {}

  void clear() {
    for (std::size_t i = MAX_THREADS; i--;) {
      currSizes[i] = 0;
    }
    map.clear();
  }

  void acquireClear() { map.acquireClear(); }

  void setCam(const double *ncam) { cam.copyFrom(ncam); }

  /// modifies coord. Do not use afterwards
  template <class TerGen>
  bool addEdge(v::IVec<N> &coord, std::size_t dim1, std::size_t dim2,
               v::DVec<3> a, v::DVec<3> b, TerGen &terGen,
               std::size_t threadi) {
    if (currSizes[threadi] + 3 > maxSize) {
      return false;
    }

    std::int32_t mod1 = 1, mod2 = 1;
    std::int32_t cmod1 = 0, cmod2 = 0;
    std::size_t tdim1 = dim1, tdim2 = dim2;
    if (cam[dim1] < coord[dim1]) {
      cmod1 = 1;
      coord[dim1]--;
    } else {
      mod1 = -1;
      tdim1 += N;
    }
    if (cam[dim2] < coord[dim2]) {
      cmod2 = 1;
      coord[dim2]--;
    } else {
      mod2 = -1;
      tdim2 += N;
    }

    auto front = terGen(coord);
    coord[dim1] += mod1;
    auto s1 = terGen(coord);
    coord[dim2] += mod2;
    auto back = terGen(coord);
    coord[dim1] -= mod1;
    auto s2 = terGen(coord);
    coord[dim2] -= mod2;

    if (!front.isOpaque()) {
      if (s1.isVisible()) {
        coord[dim1] += cmod1;
        storeEdge(coord, dim1, s1, tdim1, a, b, threadi);
        coord[dim1] -= cmod1;
      }
      if (s2.isVisible()) {
        coord[dim2] += cmod2;
        storeEdge(coord, dim2, s2, tdim2, a, b, threadi);
        coord[dim2] -= cmod2;
      }
    }
    if (back.isVisible()) {
      if (!s1.isOpaque()) {
        coord[dim1] += mod1;
        coord[dim2] += cmod2;
        storeEdge(coord, dim2, back, tdim2, a, b, threadi);
        coord[dim2] -= cmod2;
        coord[dim1] -= mod1;
      }
      if (!s2.isOpaque()) {
        coord[dim2] += mod2;
        coord[dim1] += cmod1;
        storeEdge(coord, dim1, back, tdim1, a, b, threadi);
        coord[dim1] -= cmod1;
        coord[dim2] -= mod2;
      }
    }
    return true;
  }

  float *fillVertexAttribPointer(float *out, float *out_fend) {
    std::atomic_thread_fence(std::memory_order_acquire);
    float *out_end = out_fend - 21 * N * (N - 1) * (N - 2);
    for (std::size_t i = map.size; i--;) {
      if (out >= out_end) {
        break;
      }

      auto &e = map.table[i];
      if (e.hash.load(std::memory_order_relaxed) == 0) {
        continue;
      }
      Entry &fe = map.table[i].value.second;
      if (fe.edgeCount < 3) {
        continue;
      }
      float r = fe.color.r;
      float g = fe.color.g;
      float b = fe.color.b;
      float a = fe.color.a;
      v::DVec<3> v0 = fe.edges[0][0];
      for (std::size_t i = fe.edgeCount; i-- > 1;) {
        v::DVec<3> v1 = fe.edges[i][0];
        v::DVec<3> v2 = fe.edges[i][1];
        if (v::dist2(v0, v1) < 1e-8 || v::dist2(v0, v2) < 1e-8) {
          continue;
        }
        *out++ = v0[0];
        *out++ = v0[1];
        *out++ = v0[2];
        *out++ = r;
        *out++ = g;
        *out++ = b;
        *out++ = a;

        *out++ = v1[0];
        *out++ = v1[1];
        *out++ = v1[2];
        *out++ = r;
        *out++ = g;
        *out++ = b;
        *out++ = a;

        *out++ = v2[0];
        *out++ = v2[1];
        *out++ = v2[2];
        *out++ = r;
        *out++ = g;
        *out++ = b;
        *out++ = a;
      }
    }
    return out;
  }
};

} // namespace hypervoxel

#endif // HYPERVOXEL_FACES_MANAGER_HPP_

