#ifndef HYPERVOXEL_FACES_MANAGER_HPP_
#define HYPERVOXEL_FACES_MANAGER_HPP_

#include <limits>
#include <unordered_map>
#include <utility>

#include "pool_linked_list.hpp"
#include "terrain_slicer.hpp"
#include "vector.hpp"

namespace hypervoxel {

inline double ab(double a) { return a < 0 ? -a : a; }

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
    /* plist::Node */ void *listEntry = nullptr;
  };

  typedef std::unordered_map<Face, Entry, FaceHash> umap;
  typedef PoolLinkedList<typename umap::iterator> plist;

  umap map;
  plist list;
  std::size_t maxSize;
  v::DVec<N> cam;

  template <std::size_t M, class A, class = typename A::thisisavvec>
  struct DVecFrom {

    typedef void thisisavvec;
    typedef double value_type;
    static const std::size_t size = M;

    const A &a;

    value_type operator[](std::size_t i) const { return a[i]; }
  };

  template <class A> DVecFrom<A::size, A> toDVec(const A &a) const {
    return {a};
  }

  template <class GetColor>
  void storeEdge(const v::IVec<N> &coord, std::size_t dim,
                 const GetColor &getColor, const v::DVec<3> &a,
                 const v::DVec<3> &b) {
    auto pp = map.emplace(Face{coord, dim}, Entry{});
    auto next = pp.first;
    Entry &e = next->second;
    if (pp.second) {
      e.color = getColor();
      e.listEntry = list.addToBeg(next);
    }
    e.edges[e.edgeCount][0] = a;
    e.edges[e.edgeCount++][1] = b;
  }

  template <class BlockData>
  void storeEdge(const v::IVec<N> &coord, std::size_t dim,
                 const BlockData &bdata, std::size_t param, const v::DVec<3> &a,
                 const v::DVec<3> &b) {
    storeEdge(coord, dim,
              [&bdata, param]() -> Color { return bdata.getColor(param); }, a,
              b);
  }

public:
  FacesManager(std::size_t maxSize, const v::DVec<N> &cam)
      : map(maxSize * 2), list(maxSize), maxSize(maxSize), cam(cam) {}

  FacesManager() : maxSize(8) {}

  void clear() {
    map.clear();
    list.clear();
  }

  void setCam(const double *ncam) { cam.copyFrom(ncam); }

  /// modifies coord. Do not use afterwards
  template <class TerGen>
  bool addEdge(v::IVec<N> &coord, std::size_t dim1, std::size_t dim2,
               v::DVec<3> a, v::DVec<3> b, TerGen &terGen) {
    if (list.invSize() < 2) {
      return false;
    }
    if (map.bucket_count() - map.size() < 2) {
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
        storeEdge(coord, dim1, s1, tdim1, a, b);
        coord[dim1] -= cmod1;
      }
      if (s2.isVisible()) {
        coord[dim2] += cmod2;
        storeEdge(coord, dim2, s2, tdim2, a, b);
        coord[dim2] -= cmod2;
      }
    }
    if (back.isVisible()) {
      if (!s1.isOpaque()) {
        coord[dim1] += mod1;
        coord[dim2] += cmod2;
        storeEdge(coord, dim2, back, tdim2, a, b);
        coord[dim2] -= cmod2;
        coord[dim1] -= mod1;
      }
      if (!s2.isOpaque()) {
        coord[dim2] += mod2;
        coord[dim1] += cmod1;
        storeEdge(coord, dim1, back, tdim1, a, b);
        coord[dim1] -= cmod1;
        coord[dim2] -= mod2;
      }
    }
    return true;
  }

  float *fillVertexAttribPointer(float *out, float *out_fend) {
    float *out_end = out_fend - 21 * N * (N - 1) * (N - 2);
    for (typename plist::iterator iter = list.begin(), iter_end = list.end();
         iter != iter_end; ++iter) {
      if (out >= out_end) {
        break;
      }

      Entry &fe = (*iter)->second;
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
        if (v::dist(v0, v1) < 1e-8 || v::dist(v0, v2) < 1e-8) {
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

