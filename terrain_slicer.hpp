#ifndef HYPERVOXEL_TERRAIN_SLICER_HPP_
#define HYPERVOXEL_TERRAIN_SLICER_HPP_

#include <algorithm>
#include <cmath>

#include "vector.hpp"

struct Line {
  v::DVec<N> a, b;
  double arr, brr;
  std::size_t dim1, dim2;
};

template <std::size_t N>
inline Line *getLines(const double *cam, const double *forward,
                      const double *right, const double *up, double width2,
                      double height2, double dist1, double dist2, Line *lines) {
  v::DVec<N> vcam;
  vcam.copyInto(cam);
  v::DVec<N> vfor;
  vfor.copyInto(forward);
  vfor *= dist2;
  v::DVec<N> vright;
  vright.copyInto(right);
  vright *= width2 * dist2;
  v::DVec<N> vup;
  vup.copyInto(up);
  vup *= height2 * dist2;
  double findist = dist2 * dist2 + width2 * width2 + height2 * height2;
  v::DVec<N> p1 = vcam;
  v::DVec<N> p2 = vcam - vright - vup;
  v::DVec<N> p3 = vcam - vright + vup;
  v::DVec<N> p4 = vcam + vright + vup;
  v::DVec<N> p5 = vcam + vright - vup;
  struct LineRange {
    v::DVec<N> p1, p2;
    v::DVec<N> min, max;

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
    planes[i] = std::floor(cam[i]);
    planes[i + N] = planes[i] + 1;
  }
  for (std::size_t i = N * 2; i--;) {
    std::size_t dim = i >= N ? i - N : i;
    for (int plane = planes[i];; plane++) {
      struct Intersection {
        bool isReal = false;
        std::size_t linei = -1;
        v::DVec<N> p;
      };
      Intersection itions[5]();
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
      std::size_t facecs[5](0);
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
      for (std::size_t ii = N * 2; ii--;) {
        std::size_t dim2 = ii >= N ? ii - N : ii;
        if (dim == dim2) {
          continue;
        }
        for (int plane2 = planes2[ii];; plane2++) {
          Intersection itions2[2]();
          std::size_t itionc2 = 0;
          for (std::size_t j = itionc; j--;) {
            LineRange l = lines2d[j];
            if (l.min[dim2] <= plane2 && plane2 < l.max[dim2]) {
              itions2[itionc2].isReal = true;
              itions2[itionc2].linei = j;
              double offset =
                  l.max[dim2] - l.min[dim2] >= 1e-8
                      ? (plane - l.p1[dim2]) / (l.p2[dim2] - l.p1[dim2])
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
          lines++;
        }
      }
    }
  }
  return lines;
}

#endif // HYPERVOXEL_TERRAIN_SLICER_HPP_

