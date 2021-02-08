#ifndef HYPERVOXEL_TERRAIN_SLICER_HPP_
#define HYPERVOXEL_TERRAIN_SLICER_HPP_

#include <algorithm>
#include <cmath>

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

template <std::size_t N>
inline Line<N> *getLines(const SliceDirs<N> &sd, double dist, Line<N> *lines) {
  constexpr double maxDiag = std::sqrt(N);
  double roff, uoff, foff;
  if (sd.width2 > sd.height2) {
    roff = maxDiag * sd.width2 / sd.height2;
    uoff = maxDiag;
    foff = maxDiag / sd.height2;
  } else {
    roff = maxDiag;
    uoff = maxDiag * sd.height2 / sd.width2;
    foff = maxDiag / sd.width2;
  }
  roff += sd.width2 * dist;
  uoff += sd.height2 * dist;
  v::DVec<N> vcam = sd.cam;
  v::DVec<N> vfor = sd.forward * dist;
  v::DVec<N> vright = sd.right * roff;
  v::DVec<N> vup = sd.up * uoff;

  v::DVec<N> p1 = vcam - sd.forward * foff;
  v::DVec<N> p2 = vcam + vfor - vright - vup;
  v::DVec<N> p3 = vcam + vfor - vright + vup;
  v::DVec<N> p4 = vcam + vfor + vright + vup;
  v::DVec<N> p5 = vcam + vfor + vright - vup;
  v::DVec<3> p13 = {0, 0, -foff};
  v::DVec<3> p23 = {-roff, -uoff, dist};
  v::DVec<3> p33 = {-roff, +uoff, dist};
  v::DVec<3> p43 = {+roff, +uoff, dist};
  v::DVec<3> p53 = {+roff, -uoff, dist};
  struct LineRange {
    v::DVec<N> p1, p2;
    v::DVec<N> min, max;
    v::DVec<3> p13, p23;

    LineRange() {}

    LineRange(const v::DVec<N> &p1, const v::DVec<N> &p2, const v::DVec<3> &p13,
              const v::DVec<3> &p23)
        : p1(p1), p2(p2), min(v::elementwiseMin(p1, p2)),
          max(v::elementwiseMax(p1, p2)), p13(p13), p23(p23) {}
  };
  LineRange origlines[] = {{p1, p2, p13, p23}, {p1, p3, p13, p33},
                           {p1, p4, p13, p43}, {p1, p5, p13, p53},
                           {p2, p3, p23, p33}, {p3, p4, p33, p43},
                           {p4, p5, p43, p53}, {p2, p5, p23, p53}};
  std::size_t faceis[][2] = {{0, 3}, {0, 1}, {1, 2}, {2, 3},
                             {0, 4}, {1, 4}, {2, 4}, {3, 4}};
  int planes[N];
  for (std::size_t i = N; i-- > 1;) {
    const v::DVec<N> *ps[] = {&p1, &p2, &p3, &p4, &p5};
    double val = (*ps[0])[i];
    for (std::size_t k = 1; k < 5; k++) {
      double tmp = (*ps[k])[i];
      if (tmp > val) {
        val = tmp;
      }
    }
    planes[i] = val;
    planes[i] -= val <= planes[i];
  }
  for (std::size_t dim1 = N; dim1-- > 1;) {
    for (int plane1 = planes[dim1];; plane1--) {
      struct Intersection {
        bool isReal = false;
        std::size_t linei = -1;
        v::DVec<N> p;
        v::DVec<3> p3;
      };
      Intersection itions1[5];
      std::size_t itionc1 = 0;
      for (std::size_t j = 8; j--;) {
        LineRange l = origlines[j];
        if (l.min[dim1] <= plane1 && plane1 < l.max[dim1]) {
          itions1[itionc1].isReal = true;
          itions1[itionc1].linei = j;
          double offset = l.max[dim1] - l.min[dim1] >= 1e-8
                              ? (plane1 - l.p1[dim1]) / (l.p2[dim1] - l.p1[dim1])
                              : 0.5; // error shouldn't be noticable
          itions1[itionc1].p = l.p1 + (l.p2 - l.p1) * offset;
          itions1[itionc1].p3 = l.p13 + (l.p23 - l.p13) * offset;
          itions1[itionc1++].p[dim1] = plane1;
        }
      }
      if (!itionc1) {
        break;
      }

      std::size_t facecs[5] = {0, 0, 0, 0, 0};
      std::size_t faceits[5][2];
      for (std::size_t k = itionc1; k--;) {
        std::size_t linei = itions1[k].linei;
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
        std::size_t fi0 = faceits[k][0];
        std::size_t fi1 = faceits[k][1];
        lines2d[c++] = {itions1[fi0].p, itions1[fi1].p, itions1[fi0].p3,
                        itions1[fi1].p3};
      }
      int planes2[N];
      for (std::size_t j = N; j--;) {
        double val = itions1[0].p[j];
        for (std::size_t k = itionc1; k-- > 1;) {
          double tmp = itions1[k].p[j];
          if (tmp > val) {
            val = tmp;
          }
        }
        planes2[j] = val;
        planes2[j] -= val <= planes2[j];
      }
      for (std::size_t dim2 = dim1; dim2--;) {
        for (int plane2 = planes2[dim2];; plane2--) {
          Intersection itions2[2];
          std::size_t itionc2 = 0;
          for (std::size_t j = itionc1; j--;) {
            LineRange l = lines2d[j];
            if (l.min[dim2] <= plane2 && plane2 < l.max[dim2]) {
              itions2[itionc2].isReal = true;
              itions2[itionc2].linei = j;
              double offset =
                  l.max[dim2] - l.min[dim2] >= 1e-8
                      ? (plane2 - l.p1[dim2]) / (l.p2[dim2] - l.p1[dim2])
                      : 0.5;
              itions2[itionc2].p = l.p1 + (l.p2 - l.p1) * offset;
              itions2[itionc2].p3 = l.p13 + (l.p23 - l.p13) * offset;
              itions2[itionc2++].p[dim2] = plane2;
            }
          }
          if (!itionc2) {
            break;
          }
          lines->dim1 = dim1;
          lines->dim2 = dim2;
          if (itions2[0].p3[2] < itions2[1].p3[2]) {
            lines->a = itions2[0].p;
            lines->a3 = itions2[0].p3;
            lines->b = itions2[1].p;
            lines->b3 = itions2[1].p3;
          } else {
            lines->a = itions2[1].p;
            lines->a3 = itions2[1].p3;
            lines->b = itions2[0].p;
            lines->b3 = itions2[0].p3;
          }
          lines++;
        }
      }
    }
  }
  return lines;
}

} // namespace hypervoxel

#endif // HYPERVOXEL_TERRAIN_SLICER_HPP_

