#include <cmath>
#include <iostream>
#include <random>

#include "terrain_indexer.hpp"

/// returns true on failure
bool normalize5(float *p) {
  float n = p[0] * p[0] + p[1] * p[1] + p[2] * p[2] + p[3] * p[3] + p[4] * p[4];
  if (n < 1e-16) {
    // very unlikely lol
    return true;
  }
  n = std::sqrt(n);
  for (int i = 0; i < 4; i++) {
    p[i] /= n;
  }
  return false;
}

/// RandFloat should return value in (-1, 1)
template <class RandFloat>
hypervoxel::SliceDirs randSliceDirs(RandFloat &randFloat) {
  hypervoxel::SliceDirs ret;
  do {
    float g[16];
    for (int i = 0; i < 16; i += 2) {
      float x, y, s;
      do {
        x = randFloat();
        y = randFloat();
        s = x * x + y * y;
      } while (s > 1 || s <= 1e-12);
      float tmp = std::sqrt(-2 * std::log(s) / s);
      g[i] = x * tmp;
      g[i + 1] = y * tmp;
    }
    ret.r[0] = g[0];
    ret.r[1] = g[1];
    ret.r[2] = g[2];
    ret.r[3] = g[3];
    ret.r[4] = g[4];
    if (normalize5(&ret.r[0])) {
      continue;
    }

    ret.u[0] = g[5];
    ret.u[1] = g[6];
    ret.u[2] = g[7];
    ret.u[3] = g[8];
    ret.u[4] = g[9];
    if (normalize5(&ret.u[0])) {
      continue;
    }

    ret.f[0] = g[10];
    ret.f[1] = g[11];
    ret.f[2] = g[12];
    ret.f[3] = g[13];
    ret.f[4] = g[14];
    if (normalize5(&ret.f[0])) {
      continue;
    }
  } while (false);

  ret.c[0] = randFloat() * 100;
  ret.c[1] = randFloat() * 100;
  ret.c[2] = randFloat() * 100;
  ret.c[3] = randFloat() * 100;
  ret.c[4] = randFloat() * 100;

  ret.fm = (randFloat() + 1.01) * 100;
  ret.rm = randFloat() + 1.5;
  ret.um = randFloat() + 1.5;

  return ret;
}

hypervoxel::v::IVec<5>
randViewable(std::mt19937 &mtrand, const hypervoxel::v::FVec<5> &p, int sidel) {
  /*
  std::cout << "Point: " << p[0] << " " << p[1] << " " << p[2] << " " << p[3]
            << " " << p[4] << std::endl;
            */
  hypervoxel::v::IVec<5> hi = hypervoxel::vfloor(p + 0.0001);
  hypervoxel::v::IVec<5> lo = hypervoxel::vfloor(p + 0.9999) - sidel;
  hypervoxel::v::IVec<5> ret;
  for (int i = 0; i < 5; i++) {
    std::uniform_int_distribution<int> dist(lo[i], hi[i]);
    ret[i] = dist(mtrand);
  }
  return ret;
}

int main() {
  std::mt19937 mtrand(1);
  std::uniform_real_distribution<float> distro(-1, 1);

  std::uniform_int_distribution<> iDist(1, 10);
  std::uniform_real_distribution<float> fDist(0, 1);
  for (int spam = 0; spam < 100000; spam++) {
    if (spam % 100 == 0) {
      std::cout << "Indexer #" << spam << std::endl;
    }
    auto lambda = [&]() -> float { return distro(mtrand); };
    hypervoxel::SliceDirs sd = randSliceDirs(lambda);
    int sidel = iDist(mtrand);
    hypervoxel::TerrainIndexer ti(sd, sidel);

    /*
    std::cout << sidel << std::endl;
    std::cout << sd.c[0] << " " << sd.c[1] << " " << sd.c[2] << " " << sd.c[3]
              << " " << sd.c[4] << std::endl;
    std::cout << sd.r[0] << " " << sd.r[1] << " " << sd.r[2] << " " << sd.r[3]
              << " " << sd.r[4] << std::endl;
    std::cout << sd.u[0] << " " << sd.u[1] << " " << sd.u[2] << " " << sd.u[3]
              << " " << sd.u[4] << std::endl;
    std::cout << sd.f[0] << " " << sd.f[1] << " " << sd.f[2] << " " << sd.f[3]
              << " " << sd.f[4] << std::endl;
    std::cout << sd.fm << " " << sd.rm << " " << sd.um << std::endl;
    */

    for (int rkel = 0; rkel < 10000; rkel++) {
      float z = fDist(mtrand) * sd.fm;
      float x = fDist(mtrand) * z * sd.rm;
      float y = fDist(mtrand) * z * sd.um;

      // std::cout << "x,y,z: " << x << " " << y << " " << z << std::endl;

      hypervoxel::v::FVec<5> point = sd.c + x * sd.r + y * sd.u + z * sd.f;
      hypervoxel::v::IVec<5> v = randViewable(mtrand, point, sidel);

      // here's the real test!
      std::size_t i = ti.getIndex(v);
      if (i >= ti.getSize()) {
        std::cout << "ERROR!!! Index out of bounds" << std::endl;
        std::cout << i << std::endl;
        return 1;
      }
      hypervoxel::v::IVec<5> vr = ti.getCoord(i);
      if (vr != v) {
        std::cout << "ERROR!!! Vector not reproduced properly" << std::endl;
        std::cout << i << std::endl;
        std::cout << v[0] << " " << v[1] << " " << v[2] << " " << v[3] << " "
                  << v[4] << std::endl;
        std::cout << vr[0] << " " << vr[1] << " " << vr[2] << " " << vr[3]
                  << " " << vr[4] << std::endl;
        return 1;
      }
    }
  }
}

