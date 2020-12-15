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

  struct Entry {
    std::uint32_t color = 0;
    /* plist::Node */ void *listEntry = nullptr;
    std::size_t numVisible = 0;
    int faces[N]; // 0 means not known, 1 means hidden, 2 means visible

    Entry() {
      for (std::size_t i = 0; i < N; i++) {
        faces[i] = 0;
      }
    }
  };

  typedef std::unordered_map<v::IVec<N>, Entry, v::IVecHash<N>,
                             v::EqualFunctor<v::IVec<N>, v::IVec<N>>>
      umap;
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

public:
  FacesManager(std::size_t maxSize, const v::DVec<N> &cam)
      : map(maxSize * 2), list(maxSize), maxSize(maxSize), cam(cam) {}

  FacesManager() : maxSize(8) {}

  void clear() {
    map.clear();
    list.clear();
  }

  void setCam(const double *ncam) { cam.copyFrom(ncam); }

  bool addCube(const v::IVec<N> &coord, std::uint32_t val) {
    if (!(val & 0xFF)) {
      return true;
    }
    if (list.invSize() < 2) {
      return false;
    }
    if (map.bucket_count() - map.size() < 2) {
      return false;
    }
    for (std::size_t dim = N; dim--;) {
      v::IVec<N> f1 = coord, f2 = coord;
      if (cam[dim] < coord[dim]) {
        f2[dim]++;
      } else if (cam[dim] > (coord[dim] + 1)) {
        f1[dim]++;
      } else {
        continue;
      }

      auto pp = map.emplace(f1, Entry{});
      auto next = pp.first;
      Entry &e1 = next->second;
      if (e1.faces[dim] == 0) {
        e1.faces[dim] = 2;
        if (!e1.numVisible++) {
          e1.listEntry = list.addToBeg(next);
        }
      }

      if (!(~val & 0xFF)) {
        // this means opaque
        pp = map.emplace(f2, Entry{});
        next = pp.first;
        Entry &e2 = next->second;
        int tmp = e2.faces[dim];
        e2.faces[dim] = 1;
        if (tmp == 2) {
          if (!--e2.numVisible) {
            if (e2.listEntry) {
              list.remove(static_cast<typename plist::Node *>(e2.listEntry));
              e2.listEntry = nullptr;
            }
          }
        }
      }
    }
    return true;
  }

  float *fillVertexAttribPointer(const SliceDirs<N> &sd, float *out,
                                 float *out_fend) const {
    v::DVec<N> r = sd.right;
    v::DVec<N> u = sd.up;
    v::DVec<N> f = sd.forward;

    constexpr std::size_t z[] = {1, 2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31};

    constexpr std::size_t maxVertices =
        z[2 * N - 1] * z[2 * N - 2] * z[2 * N - 3] + 1;

    struct VertexDescription {
      std::size_t i, j, k;
    } vertexDescriptions[maxVertices];

    constexpr std::size_t maxEdges = z[2 * N - 1] * z[2 * N - 2] + 1;

    struct EdgeDescription {
      std::size_t i, j;
    } edgeDescriptions[maxEdges];

    struct EdgeEntry {
      std::size_t vertexCount = 0;
      std::size_t vertexList[2 * N];
    };

    struct MatEntry {
      std::size_t i, j, k;
      double invmat[3][3];
    } mats[4 * N * (N - 1) * (N - 2) / 3];
    std::size_t matcount = 0;

    for (std::size_t i = N; i--;) {
      for (std::size_t j = i; j--;) {

        edgeDescriptions[z[i] * z[j]] = {i, j};
        edgeDescriptions[z[i + N] * z[j]] = {i + N, j};
        edgeDescriptions[z[i] * z[j + N]] = {i, j + N};
        edgeDescriptions[z[i + N] * z[j + N]] = {i + N, j + N};

        for (std::size_t k = j; k--;) {
          vertexDescriptions[z[i] * z[j] * z[k]] = {i, j, k};
          vertexDescriptions[z[i + N] * z[j] * z[k]] = {i + N, j, k};
          vertexDescriptions[z[i] * z[j + N] * z[k]] = {i, j + N, k};
          vertexDescriptions[z[i + N] * z[j + N] * z[k]] = {i + N, j + N, k};
          vertexDescriptions[z[i] * z[j] * z[k + N]] = {i, j, k + N};
          vertexDescriptions[z[i + N] * z[j] * z[k + N]] = {i + N, j, k + N};
          vertexDescriptions[z[i] * z[j + N] * z[k + N]] = {i, j + N, k + N};
          vertexDescriptions[z[i + N] * z[j + N] * z[k + N]] = {i + N, j + N,
                                                                k + N};

          // for explanation:
          // double mat[3][3] = {{r[i], u[i], f[i]},
          //                     {r[j], u[j], f[j]},
          //                     {r[k], u[k], f[k]}};
          double invmat[3][3] = {
              {u[j] * f[k] - u[k] * f[j], u[k] * f[i] - u[i] * f[k],
               u[i] * f[j] - u[j] * f[i]},

              {r[k] * f[j] - r[j] * f[k], r[i] * f[k] - r[k] * f[i],
               r[j] * f[i] - r[i] * f[j]},

              {r[j] * u[k] - r[k] * u[j], r[k] * u[i] - r[i] * u[k],
               r[i] * u[j] - r[j] * u[i]}};
          double det =
              r[i] * invmat[0][0] + u[i] * invmat[1][0] + f[i] * invmat[2][0];
          if (-1e-12 < det && det < 1e-12) {
            continue;
          }
          MatEntry &ref = mats[matcount];
          ref.invmat[0][0] = invmat[0][0] / det;
          ref.invmat[0][1] = invmat[0][1] / det;
          ref.invmat[0][2] = invmat[0][2] / det;
          ref.invmat[1][0] = invmat[1][0] / det;
          ref.invmat[1][1] = invmat[1][1] / det;
          ref.invmat[1][2] = invmat[1][2] / det;
          ref.invmat[2][0] = invmat[2][0] / det;
          ref.invmat[2][1] = invmat[2][1] / det;
          ref.invmat[2][2] = invmat[2][2] / det;
          ref.i = i;
          ref.j = j;
          ref.k = k;
          matcount++;

          mats[matcount] = ref;
          mats[matcount++].i += N;

          mats[matcount] = ref;
          mats[matcount++].j += N;

          mats[matcount] = ref;
          mats[matcount].i += N;
          mats[matcount++].j += N;

          mats[matcount] = ref;
          mats[matcount++].k += N;

          mats[matcount] = ref;
          mats[matcount].i += N;
          mats[matcount++].k += N;

          mats[matcount] = ref;
          mats[matcount].j += N;
          mats[matcount++].k += N;

          mats[matcount] = ref;
          mats[matcount].i += N;
          mats[matcount].j += N;
          mats[matcount++].k += N;
        }
      }
    }
    float *out_end = out_fend - 21 * N * (N - 1) * (N - 2);

    // PREPARATION |||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||

    // MAIN ALGORITHM ||||||||||||||||||||||||||||||||||||||||||||||||||||||||||

    for (typename plist::iterator iter = list.begin(), iter_end = list.end();
         iter != iter_end; ++iter) {

      if (out >= out_end) {
        break;
      }

      v::DVec<N> ccc = toDVec((*iter)->first) - cam;
      double cc[N * 2];
      for (std::size_t i = N; i--;) {
        cc[i + N] = (cc[i] = ccc[i]) + 1;
      }
      int *faces = (*iter)->second.faces;

      v::DVec<3> vertices[maxVertices];
      std::size_t vertexCount = 0;
      std::size_t vertexList[maxVertices];

      EdgeEntry edges[maxEdges];
      std::size_t edgeCount = 0;
      std::size_t edgeList[maxEdges]{1000000000000000LL};

      for (std::size_t i = matcount; i--;) {
        MatEntry &e = mats[i];
        bool shouldContinue = false;
        double vv0 = e.invmat[0][0] * cc[e.i] + e.invmat[0][1] * cc[e.j] +
                     e.invmat[0][2] * cc[e.k];

        double vv1 = e.invmat[1][0] * cc[e.i] + e.invmat[1][1] * cc[e.j] +
                     e.invmat[1][2] * cc[e.k];

        double vv2 = e.invmat[2][0] * cc[e.i] + e.invmat[2][1] * cc[e.j] +
                     e.invmat[2][2] * cc[e.k];
        for (std::size_t ind = N; ind--;) {
          double result = vv0 * r[ind] + vv1 * u[ind] + vv2 * f[ind];
          if (result < cc[ind] - 1e-6 || result > cc[ind] + (1 + 1e-6)) {
            shouldContinue = true;
            break;
          }
        }

        if (shouldContinue) {
          continue;
        }
        std::size_t vertexInd = z[e.i] * z[e.j] * z[e.k];
        vertices[vertexInd] = {vv0, vv1, vv2};
        vertexList[vertexCount++] = vertexInd;

        std::size_t ei1 = z[e.i] * z[e.j];
        std::size_t ei2 = z[e.i] * z[e.k];
        std::size_t ei3 = z[e.j] * z[e.k];

        if (!edges[ei1].vertexCount) {
          edgeList[edgeCount++] = ei1;
        }
        if (edges[ei1].vertexCount < 2) {
          edges[ei1].vertexList[edges[ei1].vertexCount++] = vertexInd;
        }
        if (!edges[ei2].vertexCount) {
          edgeList[edgeCount++] = ei2;
        }
        if (edges[ei2].vertexCount < 2) {
          edges[ei2].vertexList[edges[ei2].vertexCount++] = vertexInd;
        }
        if (!edges[ei3].vertexCount) {
          edgeList[edgeCount++] = ei3;
        }
        if (edges[ei3].vertexCount < 2) {
          edges[ei3].vertexList[edges[ei3].vertexCount++] = vertexInd;
        }
      }

      struct FaceEntry {
        std::size_t edgeCount = 0;
        std::size_t edges[2 * N * (2 * N - 1)][2];
      } faceEntries[2 * N];
      for (std::size_t i = edgeCount; i--;) {
        EdgeEntry &cedge = edges[edgeList[i]];
        if (cedge.vertexCount < 2) {
          continue;
        }
        EdgeDescription ed = edgeDescriptions[edgeList[i]];
        if (faces[ed.i] != 2 && faces[ed.j] != 2) {
          continue;
        }

        std::size_t v0 = cedge.vertexList[0];
        std::size_t v1 = cedge.vertexList[1];
        if (cedge.vertexCount > 2) {
          // this is degeneracy case
          v::DVec<3> df = vertices[v1] - vertices[v0];
          double dv0 = v::dot(vertices[v0], df);
          double dv1 = v::dot(vertices[v1], df);
          for (std::size_t i = cedge.vertexCount; i-- > 2;) {
            std::size_t vt = cedge.vertexList[i];
            double dvt = v::dot(vertices[vt], df);
            if (dvt > dv1 + 1e-12) {
              v1 = vt;
              dv1 = dvt;
            } else if (dvt < dv0 - 1e-12) {
              v0 = vt;
              dv0 = dvt;
            }
          }
        }
        if (faces[ed.i] == 2) {
          FaceEntry &fe = faceEntries[ed.i];
          fe.edges[fe.edgeCount][0] = v0;
          fe.edges[fe.edgeCount++][1] = v1;
        }
        if (faces[ed.j] == 2) {
          FaceEntry &fe = faceEntries[ed.j];
          fe.edges[fe.edgeCount][0] = v0;
          fe.edges[fe.edgeCount++][1] = v1;
        }
      }
      std::uint32_t col = (*iter)->second.color;
      float r = (col >> 24) / 256.f;
      float g = ((col >> 16) & 0xFF) / 256.f;
      float b = ((col >> 8) & 0xFF) / 256.f;
      float a = (col & 0xFF) / 256.f;
      for (std::size_t dim = N; dim--;) {
        if (faces[dim] != 2) {
          continue;
        }
        FaceEntry &fe = faceEntries[dim];
        if (fe.edgeCount < 3) {
          continue;
        }
        std::size_t v0 = fe.edges[0][0];
        v::DVec<3> vv0 = vertices[v0];
        for (std::size_t i = fe.edgeCount; i-- > 1;) {
          std::size_t v1 = fe.edges[i][0];
          std::size_t v2 = fe.edges[i][1];
          if (v1 == v0 || v2 == v0) {
            continue;
          }
          v::DVec<3> &vv1 = vertices[v1];
          v::DVec<3> &vv2 = vertices[v2];
          *out++ = vv0[0];
          *out++ = vv0[1];
          *out++ = vv0[2];
          *out++ = dim % 3 == 0; // r;
          *out++ = dim % 2 == 1; // g;
          *out++ = dim >= 2;     // b;
          *out++ = 1;            // a;

          *out++ = vv1[0];
          *out++ = vv1[1];
          *out++ = vv1[2];
          *out++ = dim % 3 == 0; // r;
          *out++ = dim % 2 == 1; // g;
          *out++ = dim >= 2;     // b;
          *out++ = 1;            // a;

          *out++ = vv2[0];
          *out++ = vv2[1];
          *out++ = vv2[2];
          *out++ = dim % 3 == 0; // r;
          *out++ = dim % 2 == 1; // g;
          *out++ = dim >= 2;     // b;
          *out++ = 1;            // a;
        }
      }
    }
    return out;
  } // end fillVertexAttribPointer(...)
};

} // namespace hypervoxel

#endif // HYPERVOXEL_FACES_MANAGER_HPP_

