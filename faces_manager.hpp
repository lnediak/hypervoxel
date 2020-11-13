#ifndef HYPERVOXEL_FACES_MANAGER_HPP_
#define HYPERVOXEL_FACES_MANAGER_HPP_

#include <numeric_limits>
#include <unordered_map>
#include <utility>

#include "pool_linked_list.hpp"
#include "vector.hpp"

namespace hypervoxel {

static inline double ab(double a) { return a < 0 ? -a : a; }

template <std::size_t N> class FacesManager {

  struct Face {
    v::IVec<N> c;
    std::size_t dim;
  };

  struct FaceHash {

    std::size_t operator()(const Face &f) const noexcept {
      std::size_t basic = CoordHash<N>()(f.c);
      return (((basic << (32 - f.dim)) | (basic >> f.dim)) ^ f.dim) + f.dim;
    }
  };

  struct Entry;

  typedef std::unordered_map<Face, Entry, FaceHash> umap;
  typedef PoolLinkedList<umap::iterator> plist;

  struct Entry {
    std::uint32_t color = 0;
    plist::Node *listEntry = nullptr;
  };

  umap map;
  plist list;
  std::size_t maxSize;
  v::DVec<N> cam;

  template <std::size_t D>
  void addFaces(const v::IVec<N> &coord, std::uint32_t val) {
    Face f1{coord, D}, f2{coord, D};
    if (cam[D] < coord[D]) {
      f2.c[D]++;
    } else if (cam[D] > (coord[D] + 1)) {
      f1.c[D]++;
    } else {
      return addFaces<D + 1>(coord, val);
    }

    auto pp = map.emplace(f1, {});
    auto next = pp.first;
    if (pp.second) {
      next->second.color = val;
      next->second.listEntry = list.addToBeg(next);
    }

    if (!(~val & 0xFF)) {
      // this means opaque
      pp = map.emplace(f2, {});
      next = pp.first;
      if (!pp.second) {
        if (next->second.listEntry) {
          list.remove(next->second.listEntry);
          next->second.listEntry = nullptr;
        }
      }
    }

    addFaces<D + 1>(coord, val);
  }

  template <> void addFaces<N>(const v::IVec<N> &, std::uint32_t) { return; }

public:
  FacesManager(std::size_t maxSize, const v::DVec<N> &cam)
      : map(maxSize * 2), list(maxSize), maxSize(maxSize), cam(cam) {}

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
    addFaces<0>(coord, val);
    return true;
  }

  void fillVertexAttribPointer(const double *forward, const double *right,
                               const double *up, float *out,
                               std::size_t maxTriangles) const {
    v::DVec<N> f;
    f.copyInto(forward);
    v::DVec<N> r;
    r.copyInto(right);
    v::DVec<N> u;
    u.copyInto(up);

    v::DVec<3> normals[N][2];
    v::Vec<std::size_t, 3> sort[N];
    v::DVec<3> side1[N];
    v::DVec<3> side2[N];
    for (std::size_t i = N; i--;) {
      normals[i][1][0] = r[i];
      normals[i][1][1] = u[i];
      normals[i][1][2] = f[i];
      normals[i][0] = -normals[i][1];
      std::size_t minI;
      std::size_t maxI;
      if (ab(r[i]) > ab(u[i])) {
        minI = ab(f[i]) > ab(u[i]) ? 1 : 2;
        maxI = ab(f[i]) > ab(r[i]) ? 2 : 0;
      } else {
        minI = ab(f[i]) > ab(r[i]) ? 0 : 2;
        maxI = ab(f[i]) > ab(u[i]) ? 2 : 1;
      }
      std::size_t midI = minI ? (maxI ? 0 : (minI % 2 + 1)) : (maxI % 2 + 1);
      sort[i][0] = maxI;
      sort[i][1] = midI;
      sort[i][2] = minI;

      side1[i][maxI] = normals[i][1][midI];
      side1[i][midI] = -normals[i][1][maxI];
      side1[i][minI] = 0;

      side2[i][maxI] = -normals[i][1][minI];
      side2[i][midI] =
          -normals[i][1][midI] * normals[i][1][minI] / normals[i][1][maxI];
      side2[i][minI] =
          normals[i][1][midI] * normals[i][1][midI] / normals[i][1][maxI] +
          normals[i][1][maxI];
    }

    v::DVec<2> reductions[N][N][2];
    v::Vec<std::size_t, 2> reductionSort[N][N];
    double reductionAngles[N][N][2];
    for (std::size_t i = N; i--;) {
      for (std::size_t j = N; j--;) {
        if (i == j) {
          continue;
        }
        reductions[i][j][1][0] = v::dot(normals[j[1]], side1[i]);
        reductions[i][j][1][1] = v::dot(normals[j[1]], side2[i]);
        reductionSort[i][j][0] =
            ab(reductions[i][j][1][0]) > ab(reductions[i][j][1][1]) ? 0 : 1;
        reductionSort[i][j][1] = !reductionSort[i][j][1][0];
        reductions[i][j][0] = -reductions[i][j][1];
        reductionAngles[i][j][0] =
            std::atan2(reduction[i][j][0][1], reduction[i][j][0][0]);
        reductionAngles[i][j][1] =
            std::atan2(reduction[i][j][1][1], reduction[i][j][1][0]);
      }
    }

    struct HPlnI {
      std::size_t dim;
      bool forward;
    };
    HPlnI sortedPlanes[N][2 * N - 2];
    std::size_t planeCounts[N];
    for (std::size_t i = N; i--;) {
      planeCounts[i] = 0;
      for (std::size_t j = N - 1; j--;) {
        std::size_t dim = j + (j >= i);
        if (ab(reductions[i][j][0][reductionsSort[i][j][0]]) < 1e-6) {
          continue;
        }
        sortedPlanes[i][planeCounts[i]++] = {dim, true};
        sortedPlanes[i][planeCounts[i]++] = {dim, false};
      }
      std::sort(&sortedPlanes[i][0], &sortedPlanes[i + 1][0],
                [&reductionAngles, i](HPlnI a, HPlnI b) -> bool {
                  return reductionAngles[i][a.dim][a.forward] <
                         reductionAngles[i][b.dim][b.forward];
                });
    }

    // --------------------------------------------------------------PREPARATION

    // !@#$!@#$!@#$!@#$!@#$!@#$!@#$!@#$!@#$!@#$!@#$!@#$!@#$!@#$!@#$!@#$!@#$!@#$!

    // -----------------------------------------------------------MAIN ALGORITHM

    double *out_end = out + maxTriangles * 7 - 21 * N;
    for (plist::iterator iter = plist.begin(), iter_end = plist.end();
         iter != iter_end && out < out_end; ++iter) {
      if (out >= out_end) {
        break;
      }
      std::size_t dim = iter->first.dim;
      if (ab(normals[dim][sort[dim][0]]) < 1e-6) {
        continue;
      }
      v::IVec<N> c = iter->first.c;
      std::int32_t cc[N][2];
      for (std::size_t i = N; i--;) {
        cc[i][1] = c[i];
        cc[i][0] = -c[i] - 1;
      }
      v::DVec<3> basep;
      basep[sort[dim][0]] = c[dim] / normals[dim][sort[dim][0]];
      basep[sort[dim][1]] = 0;
      basep[sort[dim][2]] = 0;
      v::DVec<2> planebps[N][2];
      for (std::size_t i = N; i--;) {
        if (i == dim) {
          continue;
        }
        planebps[i][0][reductionSort[dim][i][0]] =
            invc[i] / reduction[dim][i][0][reductionSort[dim][i][0]];
        planebps[i][0][reductionSort[dim][i][1]] = 0;
        planebps[i][1][reductionSort[dim][i][0]] =
            c[i] / reduction[dim][i][1][reductionSort[dim][i][0]];
        planebps[i][1][reductionSort[dim][i][1]] = 0;
      }
      struct EdgeEntry {
        HPlnI pln;
        double i1, i2;
      };
      EdgeEntry edges[N];
      edges[0] = {sortedPlanes[0], std::numeric_limits<double>::quiet_NaN(),
                  std::numeric_limits<double>::infinity()};
      std::size_t edgeCount = 1;
      bool feasible = true;
      for (std::size_t i = 1; i < planeCount[dim]; i++) {
        HPlnI cpln = sortedPlanes[i];
        v::DVec<2> redcpln = reductions[dim][cpln.dim][cpln.forward];
        double ccc = cc[cpln.dim][cpln.forward];
        std::size_t j = edgeCount;
        while (j--) {
          v::DVec<2> &redjpln =
              reductions[dim][edges[j].pln.dim][edges[j].pln.forward];
          v::DVec<2> &jplnbp = planebps[edges[j].pln.dim][edges[j].pln.forward];
          // the denominator looks funny to deal with the case of parallel edges
          edges[j].i2 = (ccc - v::dot(redcpln, jplnbp)) /
                        -(redcpln[1] * redjpln[0] - redcpln[0] * redjpln[1]);
          if (std::isnan(edges[j].i2)) {
            // a wildly unlikely but nonetheless deadly case
            edges[j].i2 = -std::numeric_limits<double>::infinity();
          }
          if (edges[j].i1 >= edges[j].i2) {
            // edge is either useless or proves infeasibility
            // also j != 0 since edges[0].i1 = NaN
            v::DVec<2> &redppln =
                reductions[dim][edges[j - 1].pln.dim][edges[j - 1].pln.forward];
            if (redppln[0] * redcpln[1] - redppln[1] * redcpln[0] <= 0) {
              feasible = false;
              break;
            }
            continue;
          }
          break;
        }
        if (!feasible) {
          break;
        }
        v::DVec<2> &redjpln =
            reductions[dim][edges[j].pln.dim][edges[j].pln.forward];
        double jcc = cc[edges[j].pln.dim][edges[j].pln.forward];
        v::DVec<2> &cplnbp = planebps[cpln.dim][cpln.forward];
        edgeCount = j + 1;
        edges[edgeCount++] = {
            cpln,
            (jcc - v::dot(redjpln, cplnbp)) /
                (redjpln[0] * redcpln[1] - redjpln[1] * redcpln[0]),
            std::numeric_limits<double>::quiet_NaN()};
      }
      if (!feasible) {
        continue;
      }
      EdgeEntry *edgesBeg = edges;
      EdgeEntry *edgesEnd = edges + edgeCount - 1;
      v::DVec<2> redeepln =
          reductions[dim][edgesEnd->pln.dim][edgesEnd->pln.forward];
      double eecc = cc[edgesEnd->pln.dim][edgesEnd->pln.forward];
      v::DVec<2> bplnbp = planebps[edgesBeg->pln.dim][edgesBeg->pln.forward];
      while (true) {
        v::DVec<2> &redbpln =
            reductions[dim][edgesBeg->pln.dim][edgesBeg->pln.forward];
        edgesBeg->i1 = (eecc - v::dot(redeepln, bplnbp)) /
                       (redeepln[0] * redbpln[1] - redeepln[1] * redbpln[0]);
        if (edgesBeg->i1 >= edgesBeg->i2) {
          edgesBeg++;
          v::DVec<2> &rednpln =
              reductions[dim][edgesBeg[1].pln.dim][edgesBeg[1].pln.forward];
          bplnbp = planebps[edgesBeg->pln.dim][edgesBeg->pln.forward];
          double ncc = cc[edgesBeg[1].pln.dim][edgesBeg[1].forward];
          edgesBeg->i2 = (ncc - v::dot(rednpln, bplnbp)) /
                         -(rednpln[1] * redbpln[0] - rednpln[0] * redbpln[1]);
          continue;
        }
        break;
      }
      v::DVec<2> redbpln =
          reductions[dim][edgesBeg->pln.dim][edgesBeg->pln.forward];
      double bcc = cc[edgesBeg->pln.dim][edgesBeg->pln.forward];
      v::DVec<2> eeplnbp = planebps[edgesEnd->pln.dim][edgesEnd->pln.forward];
      while (true) {
        v::DVec<2> &redeepln =
            reductions[dim][edgesEnd->pln.dim][edgesEnd->pln.forward];
        edgesEnd->i2 = (bcc - v::dot(redbpln, eeplnbp)) /
                       -(redbpln[1] * redeepln[0] - redbpln[0] * redeepln[1]);
        if (edgesEnd->i1 >= edgesEnd->i2) {
          edgesEnd--;
          v::DVec<2> &redppln =
              reductions[dim][edgesEnd[-1].pln.dim][edgesEnd[-1].pln.forward];
          eeplnbp = planebps[edgesBeg->pln.dim][edgesBeg->pln.forward];
          double pcc = cc[edgesEnd[-1].pln.dim][edgesEnd[-1].forward];
          edgesEnd->i1 = (pcc - v::dot(redppln, eeplnbp)) /
                         (redppln[0] * redeepln[1] - redppln[1] * redeepln[0]);
          continue;
        }
        break;
      }

      v::DVec<3> vertices[N];
      EdgeEntry *eiter = edgesBeg;
      std::size_t vertexCount = 0;
      for (;; vertexCount++, ++eiter) {
        v::DVec<2> vertex2 = planebps[eiter->pln.dim][eiter->pln.forward];
        vertices[vertexCount++] =
            vertex2[0] * side1[dim] + vertex2[1] * side2[dim];
        if (eiter >= edgesEnd) {
          break;
        }
      }

      std::uint32_t col = iter->second.color;
      float r = (col >> 24) / 256.f;
      float g = ((col >> 16) & 0xFF) / 256.f;
      float b = ((col >> 8) & 0xFF) / 256.f;
      float a = (col & 0xFF) / 256.f;
      for (std::size_t i = 1; i < vertexCount - 1; i++) {
        *out++ = vertices[0][0];
        *out++ = vertices[0][1];
        *out++ = vertices[0][2];
        *out++ = r;
        *out++ = g;
        *out++ = b;
        *out++ = a;

        *out++ = vertices[i][0];
        *out++ = vertices[i][1];
        *out++ = vertices[i][2];
        *out++ = r;
        *out++ = g;
        *out++ = b;
        *out++ = a;

        *out++ = vertices[i + 1][0];
        *out++ = vertices[i + 1][1];
        *out++ = vertices[i + 1][2];
        *out++ = r;
        *out++ = g;
        *out++ = b;
        *out++ = a;
      }
    } // end for (...)
  }   // end fillVertexAttribPointer(...)
};

} // namespace hypervoxel

#endif // HYPERVOXEL_FACES_MANAGER_HPP_

