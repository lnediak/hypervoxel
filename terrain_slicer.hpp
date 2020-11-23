#ifndef HYPERVOXEL_TERRAIN_SLICER_HPP_
#define HYPERVOXEL_TERRAIN_SLICER_HPP_

#include <algorithm>
#include <cmath>

#include "vector.hpp"

namespace hypervoxel {

template <std::size_t N> struct Line {
  v::DVec<N> a, b;
  /// all defs here with a meaning a - cam, b meaning b - cam
  double arr = 0;    /// a dot a
  double brr = 0;    /// b dot b
  double dfdf = 0;   /// (b - a) dot (b - a)
  double adf = 0;    /// a dot (b - a)
  double bdf = 0;    /// b dot (b - a)
  double pdiscr = 0; /// adf * adf - dfdf * arr = bdf * bdf - dfdf * brr
                     /// = (a dot b)^2 - (a dot a)*(b dot b)
  std::size_t dim1 = 0, dim2 = 0;
};

template <std::size_t N> struct SliceDirs {
  v::DVec<N> cam, forward, right, up;
  double width2, height2;
};

template <std::size_t N>
inline Line<N> *getLines(const SliceDirs<N> &sd, double dist, Line<N> *lines) {
  v::DVec<N> vcam = sd.cam;
  v::DVec<N> vfor = sd.forward;
  vfor *= dist;
  v::DVec<N> vright = sd.right;
  vright *= sd.width2 * dist;
  v::DVec<N> vup = sd.up;
  vup *= sd.height2 * dist;
  v::DVec<N> p1 = vcam;
  v::DVec<N> p2 = vcam + vfor - vright - vup;
  v::DVec<N> p3 = vcam + vfor - vright + vup;
  v::DVec<N> p4 = vcam + vfor + vright + vup;
  v::DVec<N> p5 = vcam + vfor + vright - vup;
  struct LineRange {
    v::DVec<N> p1, p2;
    v::DVec<N> min, max;

    LineRange() {}

    LineRange(const v::DVec<N> &p1, const v::DVec<N> &p2)
        : p1(p1), p2(p2), min(v::elementwiseMin(p1, p2)),
          max(v::elementwiseMax(p1, p2)) {}
  };
  LineRange origlines[] = {{p1, p2}, {p1, p3}, {p1, p4}, {p1, p5},
                           {p2, p3}, {p3, p4}, {p4, p5}, {p2, p5}};
  std::size_t faceis[][2] = {{0, 3}, {0, 1}, {1, 2}, {2, 3},
                             {0, 4}, {1, 4}, {2, 4}, {3, 4}};
  int planes[N * 2];
  for (std::size_t i = N; i--;) {
    planes[i] = std::floor(vcam[i]);
    planes[i + N] = planes[i] + 1;
  }
  for (std::size_t i = N * 2; i--;) {
    std::size_t dim = i >= N ? i - N : i;
    for (int plane = planes[i];; i >= N ? plane++ : plane--) {
      struct Intersection {
        bool isReal = false;
        std::size_t linei = -1;
        v::DVec<N> p;
      };
      Intersection itions[5];
      std::size_t itionc = 0;
      for (std::size_t j = 8; j--;) {
        LineRange l = origlines[j];
        if (l.min[dim] <= plane && plane < l.max[dim]) {
          itions[itionc].isReal = true;
          itions[itionc].linei = j;
          double offset = l.max[dim] - l.min[dim] >= 1e-8
                              ? (plane - l.p1[dim]) / (l.p2[dim] - l.p1[dim])
                              : 0.5; // error shouldn't be noticable
          itions[itionc].p = l.p1 + (l.p2 - l.p1) * offset;
          itions[itionc++].p[dim] = plane;
        }
      }
      if (!itionc) {
        break;
      }

      std::size_t facecs[5] = {0, 0, 0, 0, 0};
      std::size_t faceits[5][2];
      for (std::size_t k = itionc; k--;) {
        std::size_t linei = itions[k].linei;
        std::size_t face1 = faceis[linei][0];
        std::size_t face2 = faceis[linei][1];
        faceits[face1][facecs[face1]++] = k;
        faceits[face2][facecs[face2]++] = k;
      }
      LineRange lines2d[5];
      for (std::size_t k = 5, c = 0; k--;) {
        if (!facecs[k]) {
          continue;
        }
        lines2d[c++] = {itions[faceits[k][0]].p, itions[faceits[k][1]].p};
      }
      int planes2[N * 2];
      for (std::size_t j = N; j--;) {
        planes2[j] = std::floor(itions[0].p[j]);
        planes2[j + N] = planes2[j] + 1;
      }
      for (std::size_t ii = i; ii--;) {
        std::size_t dim2 = ii >= N ? ii - N : ii;
        if (dim == dim2) {
          continue;
        }
        for (int plane2 = planes2[ii];; ii >= N ? plane2++ : plane2--) {
          Intersection itions2[2];
          std::size_t itionc2 = 0;
          for (std::size_t j = itionc; j--;) {
            LineRange l = lines2d[j];
            if (l.min[dim2] <= plane2 && plane2 < l.max[dim2]) {
              itions2[itionc2].isReal = true;
              itions2[itionc2].linei = j;
              double offset =
                  l.max[dim2] - l.min[dim2] >= 1e-8
                      ? (plane2 - l.p1[dim2]) / (l.p2[dim2] - l.p1[dim2])
                      : 0.5;
              itions2[itionc2].p = l.p1 + (l.p2 - l.p1) * offset;
              itions2[itionc2++].p[dim2] = plane2;
            }
          }
          if (!itionc2) {
            break;
          }
          lines->dim1 = dim;
          lines->dim2 = dim2;
          double p1rr = v::dist(itions2[0].p, vcam);
          double p2rr = v::dist(itions2[1].p, vcam);
          if (p1rr < p2rr) {
            lines->a = itions2[0].p;
            lines->b = itions2[1].p;
            lines->arr = p1rr;
            lines->brr = p2rr;
          } else {
            lines->a = itions2[1].p;
            lines->b = itions2[0].p;
            lines->arr = p2rr;
            lines->brr = p1rr;
          }
          lines->dfdf = v::norm(lines->b - lines->a);
          lines->adf = v::dot(lines->a - vcam, lines->b - lines->a);
          lines->bdf = v::dot(lines->b - vcam, lines->b - lines->a);
          double ab = v::dot(lines->a - vcam, lines->b - vcam);
          lines->pdiscr = ab * ab - p1rr * p2rr;
          lines++;
        }
      }
    }
  }
  return lines;
}

} // namespace hypervoxel

#endif // HYPERVOXEL_TERRAIN_SLICER_HPP_

