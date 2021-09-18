#include <iostream>

#include "util.hpp"
#include "vector.hpp"

namespace hypervoxel {

class TerrainIndexer {

  int d1, d2, d3;
  int d4, d5;
  v::FVec<3> m4, m5;
  v::FVec<3> b4, b5;
  double o4, o5;
  v::FVec<5> c;

  v::IVec<3> cs;
  std::size_t xs, ys, zs;
  std::size_t s4;

  std::size_t size;

public:
  TerrainIndexer(const SliceDirs &sd, int sidel) {
    v::FVec<5> rmod = sd.fm * sd.rm * sd.r;
    v::FVec<5> umod = sd.fm * sd.um * sd.u;
    v::FVec<5> tmp = sd.c + sd.fm * sd.f;

    v::FVec<5> tmpr = tmp + rmod;

    v::FVec<5> tmpru = tmpr + umod;
    v::FVec<5> tmprnu = tmpr - umod;

    v::FVec<5> tmpnr = tmp - rmod;

    v::FVec<5> tmpnru = tmpnr + umod;
    v::FVec<5> tmpnrnu = tmpnr - umod;

    /*
    std::cout << "tmp vals, here we go!" << std::endl;
    std::cout << sd.c << std::endl;
    std::cout << tmpru << std::endl;
    std::cout << tmprnu << std::endl;
    std::cout << tmpnru << std::endl;
    std::cout << tmpnrnu << std::endl;
    std::cout << std::endl;
    */

    v::IVec<5> lowest = vfloor(
        v::elementwiseMin(
            sd.c, v::elementwiseMin(
                      tmpru, v::elementwiseMin(
                                 tmprnu, v::elementwiseMin(tmpnru, tmpnrnu)))) -
        sidel - 1e-3f);
    v::IVec<5> highest = vfloor(
        v::elementwiseMax(
            sd.c, v::elementwiseMax(
                      tmpru, v::elementwiseMax(
                                 tmprnu, v::elementwiseMax(tmpnru, tmpnrnu)))) +
        1e-3f);
    v::IVec<5> diffs = highest - lowest + 1; // nums in interval [a,b] is b-a+1

    v::FVec<5> a = sd.r;
    v::FVec<5> b = sd.u;
    v::FVec<5> c = sd.f;
    size = -1;
    const int N = 5;
    for (int td1 = 0; td1 < N - 2; td1++) {
      for (int td2 = td1 + 1; td2 < N - 1; td2++) {
        for (int td3 = td2 + 1; td3 < N; td3++) {
          float det = a[td1] * b[td2] * c[td3] + a[td2] * b[td3] * c[td1] +
                      a[td3] * b[td1] * c[td2] - a[td1] * b[td3] * c[td2] -
                      a[td2] * b[td1] * c[td3] - a[td3] * b[td2] * c[td1];
          if (-1e-4 < det && det < 1e-4) {
            continue;
          }
          // columns of the inverse matrix
          v::FVec<3> c1 = {b[td2] * c[td3] - b[td3] * c[td2],
                           a[td3] * c[td2] - a[td2] * c[td3],
                           a[td2] * b[td3] - a[td3] * b[td2]};
          c1 /= det;
          v::FVec<3> c2 = {b[td3] * c[td1] - b[td1] * c[td3],
                           a[td1] * c[td3] - a[td3] * c[td1],
                           a[td3] * b[td1] - a[td1] * b[td3]};
          c2 /= det;
          v::FVec<3> c3 = {b[td1] * c[td2] - b[td2] * c[td1],
                           a[td2] * c[td1] - a[td1] * c[td2],
                           a[td1] * b[td2] - a[td2] * b[td1]};
          c3 /= det;

          /*
          std::cout << "inverse matrix transpose pog" << std::endl;
          std::cout << c1 << std::endl;
          std::cout << c2 << std::endl;
          std::cout << c3 << std::endl;
          */

          int td4 = 0;
          for (; td4 < N; td4++) {
            if (td4 != td1 && td4 != td2 && td4 != td3) {
              break;
            }
          }
          int td5 = td4 + 1;
          for (; td5 < N; td5++) {
            if (td5 != td1 && td5 != td2 && td5 != td3) {
              break;
            }
          }

          // rows of a 2x3 matrix product
          v::FVec<3> tm4 = {a[td4] * c1[0] + b[td4] * c1[1] + c[td4] * c1[2],
                            a[td4] * c2[0] + b[td4] * c2[1] + c[td4] * c2[2],
                            a[td4] * c3[0] + b[td4] * c3[1] + c[td4] * c3[2]};
          v::FVec<3> tm5 = {a[td5] * c1[0] + b[td5] * c1[1] + c[td5] * c1[2],
                            a[td5] * c2[0] + b[td5] * c2[1] + c[td5] * c2[2],
                            a[td5] * c3[0] + b[td5] * c3[1] + c[td5] * c3[2]};

          // std::cout << "tm5: " << tm5 << std::endl;

          std::size_t w4 = fastFloor(sidel * l1norm(tm4) + sidel + 1.001);
          std::size_t w5 = fastFloor(sidel * l1norm(tm5) + sidel + 1.001);
          std::size_t wz = diffs[td3];
          std::size_t wy = diffs[td2];
          std::size_t wx = diffs[td1];
          std::size_t tmp = wx * wy * wz * w4 * w5;

          if (tmp < size) {
            size = tmp;
            d1 = td1;
            d2 = td2;
            d3 = td3;
            d4 = td4;
            d5 = td5;
            m4 = tm4;
            m5 = tm5;
            cs = {lowest[d1], lowest[d2], lowest[d3]};
            s4 = w5;
            zs = w4 * w5;
            ys = wz * zs;
            xs = wy * ys;
          }
        }
      }
    }

    o4 = sd.c[d4] - sidel + 1;
    o5 = sd.c[d5] - sidel + 1;

    v::FVec<3> defBase = {sd.c[d1], sd.c[d2], sd.c[d3]};
    b4 = defBase - sidel * v::FVec<3>{(float)(m4[0] < 0), (float)(m4[1] < 0),
                                      (float)(m4[2] < 0)};
    b5 = defBase - sidel * v::FVec<3>{(float)(m5[0] < 0), (float)(m5[1] < 0),
                                      (float)(m5[2] < 0)};
  }

  std::size_t getSize() const { return size; }

  std::size_t getIndex(v::IVec<5> v) const {
    v::FVec<3> vf = {(float)v[d1], (float)v[d2], (float)v[d3]};
    int v4c = harshFloor(v::dot(vf - b4, m4) + o4);
    int v5c = harshFloor(v::dot(vf - b5, m5) + o5);

    /*
    std::cout << "GETINDEX HERE!!!" << std::endl;
    std::cout << "v: " << v << std::endl;
    std::cout << size << " " << d1 << " " << d2 << " " << d3 << " " << d4 << " "
              << d5 << std::endl;
    std::cout << cs[0] << " " << cs[1] << " " << cs[2] << " " << v4c << " "
              << v5c << std::endl;
    std::cout << xs << " " << ys << " " << zs << " " << s4 << std::endl;
    std::cout << "vf: " << vf << std::endl;
    std::cout << "b5: " << b5 << std::endl;
    std::cout << "vf - b5: " << vf - b5 << std::endl;
    std::cout << "o5: " << o5 << std::endl;
    std::cout << "m5: " << m5 << std::endl;
    */

    std::size_t kek = (v[d1] - cs[0]) * xs + (v[d2] - cs[1]) * ys +
                      (v[d3] - cs[2]) * zs + (v[d4] - v4c) * s4 + (v[d5] - v5c);
    return kek;
  }

  /// in format [d1, d2, d3, d4, d5]
  v::IVec<5> getCoord5(v::IVec<5> i5) const {
    v::IVec<5> ret;
    ret[d1] = i5[d1] + cs[0];
    ret[d2] = i5[d2] + cs[1];
    ret[d3] = i5[d3] + cs[2];
    v::FVec<3> vf = {(float)ret[d1], (float)ret[d2], (float)ret[d3]};
    int v4c = harshFloor(v::dot(vf - b4, m4) + o4);
    int v5c = harshFloor(v::dot(vf - b5, m5) + o5);

    /*
    std::cout << "GETCOORD5 HERE!!!" << std::endl;
    std::cout << "i5: " << i5 << std::endl;
    std::cout << d1 << " " << d2 << " " << d3 << " " << d4 << " " << d5
              << std::endl;
    std::cout << cs[0] << " " << cs[1] << " " << cs[2] << " " << v4c << " "
              << v5c << std::endl;
    std::cout << "vf: " << vf << std::endl;
    std::cout << "b5: " << b5 << std::endl;
    std::cout << "vf - b5: " << vf - b5 << std::endl;
    std::cout << "o5: " << o5 << std::endl;
    std::cout << "m5: " << m5 << std::endl;
    */

    ret[d4] = i5[d4] + v4c;
    ret[d5] = i5[d5] + v5c;
    return ret;
  }

  v::IVec<5> getCoord(std::size_t i) const {
    v::IVec<5> i5;
    i -= xs * (i5[d1] = i / xs);
    i -= ys * (i5[d2] = i / ys);
    i -= zs * (i5[d3] = i / zs);
    i -= s4 * (i5[d4] = i / s4);
    i5[d5] = i;
    return getCoord5(i5);
  }
};

} // namespace hypervoxel

