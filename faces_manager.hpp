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
    if (list.invSize() < N) {
      return false;
    }
    if (map.bucket_count() - map.size() <= N) {
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
      if (pp.second) {
        next->second.color = val;
        next->second.listEntry = list.addToBeg(next);
      }

      pp = map.emplace(f1, Entry{});
      next = pp.first;
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
      size_t i, j, k;
    } vertexDescriptions[maxVertices];

    struct MatEntry {
      std::size_t i, j, k;
      double invmat[3][3];
    } mats[4 * N * (N - 1) * (N - 2) / 3];
    std::size_t matcount = 0;

    for (std::size_t i = N; i--;) {
      for (std::size_t j = i; j--;) {
        for (std::size_t k = j; k--;) {

          //std::cout << "MATITERATION " << i << " " << j << " " << k << std::endl;

          vertexDescriptions[z[i] * z[j] * z[k]] = {i, j, k};
          vertexDescriptions[z[i + N] * z[j] * z[k]] = {i + N, j, k};
          vertexDescriptions[z[i] * z[j + N] * z[k]] = {i, j + N, k};
          vertexDescriptions[z[i + N] * z[j + N] * z[k]] = {i + N, j + N, k};
          vertexDescriptions[z[i] * z[j] * z[k + N]] = {i, j, k + N};
          vertexDescriptions[z[i + N] * z[j] * z[k + N]] = {i + N, j, k + N};
          vertexDescriptions[z[i] * z[j + N] * z[k + N]] = {i, j + N, k + N};
          vertexDescriptions[z[i + N] * z[j + N] * z[k + N]] = {i + N, j + N, k + N};

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

            //std::cout << "CONTINUE" << std::endl;

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
    float *out_end = out_fend - 21 * N;

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

      //std::cout << std::endl << std::endl << std::endl;
      //std::cout << "ccc: " << ccc[0] << " " << ccc[1] << " " << ccc[2] << " " << ccc[3] << std::endl;
      //std::cout << "faces: " << faces[0] << " " << faces[1] << " " << faces[2] << " " << faces[3] << std::endl;
      //std::cout << "RUFRUFRUFRUFRUFRUFRUFRUFRUFRUFRUFRUFRUFRUF: " << r[0] << " " << r[1] << " " << r[2] << " " << r[3] << std::endl;
      //std::cout << "RUFRUFRUFRUFRUFRUFRUFRUFRUFRUFRUFRUFRUFRUF: " << u[0] << " " << u[1] << " " << u[2] << " " << u[3] << std::endl;
      //std::cout << "RUFRUFRUFRUFRUFRUFRUFRUFRUFRUFRUFRUFRUFRUF: " << f[0] << " " << f[1] << " " << f[2] << " " << f[3] << std::endl;

      // TODO: Update with simplex method for vertex enumeration
      v::DVec<3> vertices[maxVertices];
      std::size_t vertexCount = 0;
      std::size_t vertexList[maxVertices];

      constexpr std::size_t maxEdges = z[2 * N - 1] * z[2 * N - 2] + 1;

      struct EdgeEntry {
        std::size_t vertexCount = 0;
        std::size_t vertexList[2];
      } edges[maxEdges];

      std::size_t edgeCount = 0;
      std::size_t edgeList[maxEdges];

      struct VertexEntry {
        std::size_t i = 0;
        std::size_t vertexCount = 0;
        std::size_t vertexList[2];
      };
      struct FaceEntry {
        std::size_t vertexCount = 0;
        VertexEntry vertices[(N - 1) * (N - 2)];
        std::size_t getInd[maxVertices];
      } faceEntries[2 * N];

      for (std::size_t i = matcount; i--;) {
        MatEntry &e = mats[i];
        
        //std::cout << std::endl << std::endl;
        //std::cout << "e.i, e.j, e.k: " << e.i << " " << e.j << " " << e.k << std::endl;
        //std::cout << "cc[e.i], cc[e.j], cc[e.k]: " << cc[e.i] << " " << cc[e.j] << " " << cc[e.k] << std::endl;

        bool out = false;
        double vv0 = e.invmat[0][0] * cc[e.i] + e.invmat[0][1] * cc[e.j] +
                     e.invmat[0][2] * cc[e.k];

        double vv1 = e.invmat[1][0] * cc[e.i] + e.invmat[1][1] * cc[e.j] +
                     e.invmat[1][2] * cc[e.k];

        double vv2 = e.invmat[2][0] * cc[e.i] + e.invmat[2][1] * cc[e.j] +
                     e.invmat[2][2] * cc[e.k];
        for (std::size_t ind = N; ind--;) {
          double result = vv0 * r[ind] + vv1 * u[ind] + vv2 * f[ind];

          //std::cout << "ind, result: " << ind << " " << result << std::endl;

          if (result < cc[ind] - 1e-6 || result > cc[ind] + (1 + 1e-6)) {
            out = true;
            break;
          }
        }

        //std::cout << "vv0, vv1, vv2: " << vv0 << " " << vv1 << " " << vv2 << std::endl << std::endl;

        if (out) {
          continue;
        }

        //std::cout << "YES!" << std::endl << std::endl;

        std::size_t vertexInd = z[e.i] * z[e.j] * z[e.k];
        vertices[vertexInd] = {vv0, vv1, vv2};
        vertexList[vertexCount++] = vertexInd;

        std::size_t ei1 = z[e.i] * z[e.j];
        std::size_t ei2 = z[e.i] * z[e.k];
        std::size_t ei3 = z[e.j] * z[e.k];

        //std::cout << "ei1, ei2, ei3: " << ei1 << " " << ei2 << " " << ei3 << std::endl;
        //std::cout << "edges[ei1].vertexCount: " << edges[ei1].vertexCount << std::endl;
        //std::cout << "edges[ei2].vertexCount: " << edges[ei3].vertexCount << std::endl;
        //std::cout << "edges[ei3].vertexCount: " << edges[ei2].vertexCount << std::endl;

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

        if (faces[e.i] == 2) {
          faceEntries[e.i].getInd[vertexInd] = faceEntries[e.i].vertexCount;
          faceEntries[e.i].vertices[faceEntries[e.i].vertexCount++].i =
              vertexInd;
        }
        if (faces[e.j] == 2) {
          faceEntries[e.j].getInd[vertexInd] = faceEntries[e.j].vertexCount;
          faceEntries[e.j].vertices[faceEntries[e.j].vertexCount++].i =
              vertexInd;
        }
        if (faces[e.k] == 2) {
          faceEntries[e.k].getInd[vertexInd] = faceEntries[e.k].vertexCount;
          faceEntries[e.k].vertices[faceEntries[e.k].vertexCount++].i =
              vertexInd;
        }
      }

      for (std::size_t i = vertexCount; i--;) {
        //std::cout << "vertex " << i << std::endl;
        //auto &e = vertexDescriptions[vertexList[i]];
        //std::cout << e.i << " " << e.j << " " << e.k << std::endl;
      }

      for (std::size_t i = edgeCount; i--;) {

        //std::cout << "edge " << edgeList[i] << std::endl;

        if (!edges[edgeList[i]].vertexCount) {

          //std::cout << "Empty" << std::endl;

          continue;
        }
        if (edges[edgeList[i]].vertexCount < 2) {
          //std::cout << "THIS IS AN ERROR. WHY IS THIS MY LIFE PLEASE " "STOP_________)(#*@@@@@@@@@@@@@@@%*@#$%@@@@@@@@@$%############" << std::endl;
          continue;
        }
        std::size_t v0 = edges[edgeList[i]].vertexList[0];
        std::size_t v1 = edges[edgeList[i]].vertexList[1];
        VertexDescription dv0 = vertexDescriptions[v0];
        VertexDescription dv1 = vertexDescriptions[v1];
        std::size_t count = 0;
        std::size_t dims[2];
        if (dv0.i == dv1.i) {
          dims[count++] = dv0.i;
        }
        if (dv0.i == dv1.j) {
          dims[count++] = dv0.i;
        }
        if (dv0.i == dv1.k) {
          dims[count++] = dv0.i;
        }

        if (dv0.j == dv1.i) {
          dims[count++] = dv0.j;
        }
        if (dv0.j == dv1.j) {
          dims[count++] = dv0.j;
        }
        if (dv0.j == dv1.k) {
          dims[count++] = dv0.j;
        }

        if (dv0.k == dv1.i) {
          dims[count++] = dv0.k;
        }
        if (dv0.k == dv1.j) {
          dims[count++] = dv0.k;
        }
        if (dv0.k == dv1.k) {
          dims[count++] = dv0.k;
        }
        if (faces[dims[0]] == 2) {
          VertexEntry &ve0 =
              faceEntries[dims[0]].vertices[faceEntries[dims[0]].getInd[v0]];
          ve0.vertexList[ve0.vertexCount++] = v1;
          VertexEntry &ve1 =
              faceEntries[dims[0]].vertices[faceEntries[dims[0]].getInd[v1]];
          ve1.vertexList[ve1.vertexCount++] = v0;
        }
        if (faces[dims[1]] == 2) {
          VertexEntry &ve0 =
              faceEntries[dims[1]].vertices[faceEntries[dims[1]].getInd[v0]];
          ve0.vertexList[ve0.vertexCount++] = v1;
          VertexEntry &ve1 =
              faceEntries[dims[1]].vertices[faceEntries[dims[1]].getInd[v1]];
          ve1.vertexList[ve1.vertexCount++] = v0;
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
        if (fe.vertexCount < 3) {
          continue;
        }
        VertexEntry baseVertex = fe.vertices[0];
        VertexEntry prevVertex =
            fe.vertices[fe.getInd[baseVertex.vertexList[0]]];
        VertexEntry currVertex =
            prevVertex.vertexList[0] != baseVertex.i
                ? fe.vertices[fe.getInd[prevVertex.vertexList[0]]]
                : fe.vertices[fe.getInd[prevVertex.vertexList[1]]];
        for (;;) {
          *out++ = vertices[baseVertex.i][0];
          *out++ = vertices[baseVertex.i][1];
          *out++ = vertices[baseVertex.i][2];
          *out++ = dim % 3 == 0;//r;
          *out++ = dim % 2 == 1;//g;
          *out++ = dim >= 2;//b;
          *out++ = 1;//a;

          *out++ = vertices[prevVertex.i][0];
          *out++ = vertices[prevVertex.i][1];
          *out++ = vertices[prevVertex.i][2];
          *out++ = dim % 3 == 0;//r;
          *out++ = dim % 2 == 1;//g;
          *out++ = dim >= 2;//b;
          *out++ = 1;//a;

          *out++ = vertices[currVertex.i][0];
          *out++ = vertices[currVertex.i][1];
          *out++ = vertices[currVertex.i][2];
          *out++ = dim % 3 == 0;//r;
          *out++ = dim % 2 == 1;//g;
          *out++ = dim >= 2;//b;
          *out++ = 1;//a;

          VertexEntry nextVertex =
              currVertex.vertexList[0] != prevVertex.i
                  ? fe.vertices[fe.getInd[currVertex.vertexList[0]]]
                  : fe.vertices[fe.getInd[currVertex.vertexList[1]]];
          if (nextVertex.i == baseVertex.i) {
            break;
          }
          prevVertex = currVertex;
          currVertex = nextVertex;
        }
      }
    }
    return out;
  } // end fillVertexAttribPointer(...)
};

} // namespace hypervoxel

#endif // HYPERVOXEL_FACES_MANAGER_HPP_

