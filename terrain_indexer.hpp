#ifndef HYPERVOXEL_TERRAIN_INDEXER_HPP_
#define HYPERVOXEL_TERRAIN_INDEXER_HPP_

#include <cmath>
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

  v::IVec<3> cs;
  std::size_t xs, ys, zs;
  std::size_t s4;

  std::size_t size;

  int wx1, wy1, wz1, w41, w51;

  static float safeAbsR(float a, float b) {
    if (a == 0 && b == 0) {
      return 0;
    }
    return std::abs(a / b);
  }

  /// minimizing c1x+c2y+c3z under:
  /// z>=0,-rm<=x/z<=rm,-um<=y/z<=um,x^2+y^2+z^2<=fm^2
  static float optimize(float c1, float c2, float c3, float rm, float um,
                        float fm) {
    float curMin = INFINITY;
    // applying lagrange multipliers:
    // 2lx=c1,2ly=c2,2lz=c3,x^2+y^2+z^2=fm^2
    // r=1/2l implies obj=r(c1^2+c2^2+c3^2)
    // while fm^2=x^2+y^2+z^2=r^2(c1^2+c2^2+c3^2)
    float nor = c1 * c1 + c2 * c2 + c3 * c3;
    float fm2 = fm * fm;
    float fatr = -std::sqrt(fm2 / nor);
    /* std::cout << "x,y,z(sphere): " << fatr * c1 << " " << fatr * c2 << " "
              << fatr * c3 << std::endl; */
    if (c3 * fatr >= 0 && safeAbsR(c1, c3) <= rm && safeAbsR(c2, c3) <= um) {
      curMin = fatr * nor;
    }

    // great circles: x=-zrm,x=zrm,y=-zum,y=zum
    // take a given circle, say x=zrm. then
    // it can be written in just y and z.
    // obj=c1rmz+c2y+c3z, so z's coef is cz=(c1rm+c3).
    // x^2+y^2+z^2=y^2+z^2(1+rm^2)=fm^2.
    // let z0=z(1+rm^2)^0.5. cz0=cz/(1+rm^2)^0.5.
    // applying lagrange multipliers:
    // 2ly=c2,2lz0=cz0,y^2+z0^2=fm^2
    // r=1/2l implies obj=r(c2^2+cz0^2)
    // while fm^2=y^2+z0^2=r^2(c2^2+cz0^2)
    float rm12 = 1 + rm * rm;
    float cz = c3 + c1 * rm;
    nor = c2 * c2 + cz * cz / rm12;
    fatr = -std::sqrt(fm2 / nor);
    float c3mod = cz / rm12;
    /* std::cout << "x,y,z(great circle): " << fatr * c3mod / rm << " "
              << fatr * c2 << " " << fatr * c3mod << std::endl; */
    if (cz * fatr >= 0 && safeAbsR(c2, c3mod) <= um) {
      curMin = std::min(curMin, fatr * nor);
    }
    cz = c3 - c1 * rm;
    nor = c2 * c2 + cz * cz / rm12;
    fatr = -std::sqrt(fm2 / nor);
    c3mod = cz / rm12;
    /* std::cout << "x,y,z(great circle): " << fatr * c3mod / rm << " "
              << fatr * c2 << " " << fatr * c3mod << std::endl; */
    if (cz * fatr >= 0 && safeAbsR(c2, c3mod) <= um) {
      curMin = std::min(curMin, fatr * nor);
    }
    float um12 = 1 + um * um;
    cz = c3 + c2 * um;
    nor = c1 * c1 + cz * cz / um12;
    fatr = -std::sqrt(fm2 / nor);
    c3mod = cz / um12;
    /* std::cout << "x,y,z(great circle): " << fatr * c1 << " "
              << fatr * c3mod / um << " " << fatr * c3mod << std::endl; */
    if (cz * fatr >= 0 && safeAbsR(c1, c3mod) <= rm) {
      curMin = std::min(curMin, fatr * nor);
    }
    cz = c3 - c2 * um;
    nor = c1 * c1 + cz * cz / um12;
    fatr = -std::sqrt(fm2 / nor);
    c3mod = cz / um12;
    /* std::cout << "x,y,z(great circle): " << fatr * c1 << " "
              << fatr * c3mod / um << " " << fatr * c3mod << std::endl; */
    if (cz * fatr >= 0 && safeAbsR(c1, c3mod) <= rm) {
      curMin = std::min(curMin, fatr * nor);
    }

    // finally, we can do the basic version
    float z = fm;
    float x = rm * fm;
    float y = um * fm;
    float mod = std::sqrt(x * x + y * y + z * z) / fm;
    z /= mod;
    x /= mod;
    y /= mod;
    float pru = c1 * x + c2 * y + c3 * z;
    float pr0u = c1 * x - c2 * y + c3 * z;
    float p0ru = -c1 * x + c2 * y + c3 * z;
    float p0r0u = -c1 * x - c2 * y + c3 * z;
    return std::min(
        curMin,
        std::min(0.f, std::min(pru, std::min(pr0u, std::min(p0ru, p0r0u)))));
  }

public:
  TerrainIndexer(const SliceDirs<5> &sd, int sidel, bool usePyramid = true) {
    v::IVec<5> lowest, highest;
    if (usePyramid) {
      v::FVec<5> rmod = sd.fm * sd.rm * sd.r;
      v::FVec<5> umod = sd.fm * sd.um * sd.u;
      v::FVec<5> tmp = sd.c + sd.fm * sd.f;
      v::FVec<5> tmpr = tmp + rmod;
      v::FVec<5> tmpru = tmpr + umod;
      v::FVec<5> tmpr0u = tmpr - umod;
      v::FVec<5> tmp0r = tmp - rmod;
      v::FVec<5> tmp0ru = tmp0r + umod;
      v::FVec<5> tmp0r0u = tmp0r - umod;

      lowest = vfloor(
          v::elementwiseMin(
              sd.c,
              v::elementwiseMin(
                  tmpru, v::elementwiseMin(
                             tmpr0u, v::elementwiseMin(tmp0ru, tmp0r0u)))) -
          sidel - 1e-3f);
      highest = vfloor(
          v::elementwiseMax(
              sd.c,
              v::elementwiseMax(
                  tmpru, v::elementwiseMax(
                             tmpr0u, v::elementwiseMax(tmp0ru, tmp0r0u)))) +
          1e-3f);
    } else {
      v::FVec<5> lo, hi;
      lo[0] = optimize(sd.r[0], sd.u[0], sd.f[0], sd.rm, sd.um, sd.fm);
      lo[1] = optimize(sd.r[1], sd.u[1], sd.f[1], sd.rm, sd.um, sd.fm);
      lo[2] = optimize(sd.r[2], sd.u[2], sd.f[2], sd.rm, sd.um, sd.fm);
      lo[3] = optimize(sd.r[3], sd.u[3], sd.f[3], sd.rm, sd.um, sd.fm);
      lo[4] = optimize(sd.r[4], sd.u[4], sd.f[4], sd.rm, sd.um, sd.fm);
      lowest = vfloor(sd.c + lo - sidel - 1e-3f);

      hi[0] = -optimize(-sd.r[0], -sd.u[0], -sd.f[0], sd.rm, sd.um, sd.fm);
      hi[1] = -optimize(-sd.r[1], -sd.u[1], -sd.f[1], sd.rm, sd.um, sd.fm);
      hi[2] = -optimize(-sd.r[2], -sd.u[2], -sd.f[2], sd.rm, sd.um, sd.fm);
      hi[3] = -optimize(-sd.r[3], -sd.u[3], -sd.f[3], sd.rm, sd.um, sd.fm);
      hi[4] = -optimize(-sd.r[4], -sd.u[4], -sd.f[4], sd.rm, sd.um, sd.fm);
      highest = vfloor(sd.c + hi + 1e-3f);
    }
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

            wx1 = wx - 1;
            wy1 = wy - 1;
            wz1 = wz - 1;
            w41 = w4 - 1;
            w51 = w5 - 1;

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

  void getV4cV5c(int v1, int v2, int v3, int &v4c, int &v5c) const {
    v::FVec<3> vf = {(float)v1, (float)v2, (float)v3};
    v4c = harshFloor(v::dot(vf - b4, m4) + o4);
    v5c = harshFloor(v::dot(vf - b5, m5) + o5);
  }

  std::size_t getIndex(v::IVec<5> v) const {
    int v4c, v5c;
    getV4cV5c(v[d1], v[d2], v[d3], v4c, v5c);
    std::size_t kek = (v[d1] - cs[0]) * xs + (v[d2] - cs[1]) * ys +
                      (v[d3] - cs[2]) * zs + (v[d4] - v4c) * s4 + (v[d5] - v5c);
    return kek;
  }

  v::IVec<5> getI5(std::size_t i) const {
    v::IVec<5> i5;
    i -= xs * (i5[d1] = i / xs);
    i -= ys * (i5[d2] = i / ys);
    i -= zs * (i5[d3] = i / zs);
    i -= s4 * (i5[d4] = i / s4);
    i5[d5] = i;
    return i5;
  }

  /// in format [d1, d2, d3, d4, d5]
  v::IVec<5> getCoord5(v::IVec<5> i5) const {
    v::IVec<5> ret;
    int v1 = ret[d1] = i5[d1] + cs[0];
    int v2 = ret[d2] = i5[d2] + cs[1];
    int v3 = ret[d3] = i5[d3] + cs[2];
    int v4c, v5c;
    getV4cV5c(v1, v2, v3, v4c, v5c);
    ret[d4] = i5[d4] + v4c;
    ret[d5] = i5[d5] + v5c;
    return ret;
  }

  v::IVec<5> getCoord(std::size_t i) const { return getCoord5(getI5(i)); }

  bool incCoord5(v::IVec<5> &coord, v::IVec<5> &i5) {
    if (i5[4] >= w51) {
      i5[4] = 0;
      if (i5[3] >= w41) {
        i5[3] = 0;
        if (i5[2] >= wz1) {
          i5[2] = 0;
          coord[d3] -= wz1;
          if (i5[1] >= wy1) {
            i5[1] = 0;
            coord[d2] -= wy1;
            if (i5[0] >= wx1) {
              return false;
            }
            i5[0]++;
            coord[d1]++;
          } else {
            i5[1]++;
            coord[d2]++;
          }
        } else {
          i5[2]++;
          coord[d3]++;
        }
        getV4cV5c(coord[d1], coord[d2], coord[d3], coord[d4], coord[d5]);
        return true;
      }
      coord[d5] -= w51;
      i5[3]++;
      coord[d4]++;
    } else {
      i5[4]++;
      coord[d5]++;
    }
    return true;
  }

  // ---------- FOR TRANSFERING TO OPENCL ---------- //

  template <class cflt, class cint> void serialize(cflt *fd, cint *id) const {
    /*
     *id++ = d1;
     *id++ = d2;
     *id++ = d3;
     *id++ = d4;
     *id++ = d5;*/

    fd = (cflt *)id;
    *fd++ = m4[0];
    *fd++ = m4[1];
    *fd++ = m4[2];
    *fd++ = m5[0];
    *fd++ = m5[1];
    *fd++ = m5[2];

    *fd++ = b4[0];
    *fd++ = b4[1];
    *fd++ = b4[2];
    *fd++ = b5[0];
    *fd++ = b5[1];
    *fd++ = b5[2];

    *fd++ = o4;
    *fd++ = o5;

    id = (cint *)fd;
    *id++ = cs[0];
    *id++ = cs[1];
    *id++ = cs[2];
    *id++ = xs;
    *id++ = ys;
    *id++ = zs;
    *id++ = s4;
  }

  template <class T1, class T2>
  void serializeOrdered(const T1 *in, T2 *out) const {
    *out++ = in[d1];
    *out++ = in[d2];
    *out++ = in[d3];
    *out++ = in[d4];
    *out++ = in[d5];
  }

  v::IVec<5> getDs() const { return {d1, d2, d3, d4, d5}; }

  static const std::size_t SERIAL_LEN = 4 * 26;

  // ---------- DEBUG ---------- //

  void report() const {
    std::cout << "Report of Internal Data in TerrainIndexer." << std::endl;
    std::cout << "d's: " << d1 << " " << d2 << " " << d3 << " " << d4 << " "
              << d5 << std::endl;
    std::cout << "m4: " << m4 << std::endl;
    std::cout << "m5: " << m5 << std::endl;
    std::cout << "b4: " << b4 << std::endl;
    std::cout << "b5: " << b5 << std::endl;
    std::cout << "o4, o5: " << o4 << " " << o5 << std::endl;
    std::cout << std::endl;
    std::cout << "cs: " << cs << std::endl;
    std::cout << "xs, ys, zs, s4, size: " << xs << " " << ys << " " << zs << " "
              << s4 << " " << size << std::endl;
    std::cout << "wx1, wy1, wz1, w41, w51: " << wx1 << " " << wy1 << " " << wz1
              << " " << w41 << " " << w51 << std::endl;
  }
};

} // namespace hypervoxel

#endif // HYPERVOXEL_TERRAIN_INDEXER_HPP_

