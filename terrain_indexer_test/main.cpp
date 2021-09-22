#include <chrono>
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
  for (int i = 0; i < 5; i++) {
    p[i] /= n;
  }
  return false;
}

bool ortho(const float *a, float *b) {
  float dot =
      a[0] * b[0] + a[1] * b[1] + a[2] * b[2] + a[3] * b[3] + a[4] * b[4];
  float nor =
      a[0] * a[0] + a[1] * a[1] + a[2] * a[2] + a[3] * a[3] + a[4] * a[4];
  if (nor < 1e-16) {
    return true;
  }
  float m = dot / nor;
  b[0] -= a[0] * m;
  b[1] -= a[1] * m;
  b[2] -= a[2] * m;
  b[3] -= a[3] * m;
  b[4] -= a[4] * m;
  return false;
}

/// RandFloat should return value in (-1, 1)
template <class RandFloat>
hypervoxel::SliceDirs<5> randSliceDirs(RandFloat &randFloat,
                                       bool fixM = false) {
  hypervoxel::SliceDirs<5> ret;
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
    if (ortho(&ret.r[0], &ret.u[0])) {
      continue;
    }
    if (normalize5(&ret.u[0])) {
      continue;
    }

    ret.f[0] = g[10];
    ret.f[1] = g[11];
    ret.f[2] = g[12];
    ret.f[3] = g[13];
    ret.f[4] = g[14];
    if (ortho(&ret.r[0], &ret.f[0])) {
      continue;
    }
    if (ortho(&ret.u[0], &ret.f[0])) {
      continue;
    }
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

  if (fixM) {
    ret.fm = 20;
    ret.rm = 1.5;
    ret.um = 1.5;
  }

  return ret;
}

hypervoxel::v::IVec<5>
randViewable(std::mt19937 &mtrand, const hypervoxel::v::FVec<5> &p, int sidel) {
  hypervoxel::v::IVec<5> hi = hypervoxel::vfloor(p + 0.0001);
  hypervoxel::v::IVec<5> lo = hypervoxel::vfloor(p + 0.9999) - sidel;
  hypervoxel::v::IVec<5> ret;
  for (int i = 0; i < 5; i++) {
    std::uniform_int_distribution<int> dist(lo[i], hi[i]);
    ret[i] = dist(mtrand);
  }
  return ret;
}

#define DO_INCCOORD5_TEST
int runTest(int numIndexers = 100000, int numPoints = 10000,
            bool usePyramid = true, int verbosity = 100,
            std::uint_fast32_t seed = 1) {
  std::mt19937 mtrand(seed);
  std::uniform_real_distribution<float> distro(-1, 1);

  std::uniform_int_distribution<> iDist(1, 1);
  // std::uniform_int_distribution<> iDist(1, 10);
  std::uniform_real_distribution<float> fDist(0, 1);
#ifdef DO_INCCOORD5_TEST
  int maxDur = 0;
#endif // DO_INCCOORD5_TEST
  for (int spam = 0; spam < numIndexers; spam++) {
    if (spam % verbosity == 0) {
      std::cout << "Indexer #" << spam << std::endl;
    }
    auto lambda = [&]() -> float { return distro(mtrand); };
    // TODO: change true to false
    hypervoxel::SliceDirs<5> sd = randSliceDirs(lambda, true);
    int sidel = iDist(mtrand);
    hypervoxel::TerrainIndexer ti(sd, sidel, usePyramid);

    // added here - incCoord5 test
#ifdef DO_INCCOORD5_TEST
    hypervoxel::v::IVec<5> i5 = ti.getI5(0);
    hypervoxel::v::IVec<5> coord = ti.getCoord5(i5);
    std::size_t index = 0;
    auto currTime = std::chrono::high_resolution_clock::now();
    do {
      if (ti.getIndex(coord) != index || ti.getCoord5(i5) != coord ||
          ti.getI5(index) != i5) {
        std::cout << "ERROR!!! incCoord values not lined up" << std::endl;
        std::cout << "index: " << index << std::endl;
        std::cout << "i5: " << i5 << std::endl;
        std::cout << "coord: " << coord << std::endl;
        ti.report();
        return 1;
      }
      index++;
    } while (ti.incCoord5(coord, i5));
    if (index != ti.getSize()) {
      std::cout << "ERROR!!! incCoord does not get exactly 'size' values"
                << std::endl;
      std::cout << "wrong index: " << index << std::endl;
      ti.report();
    }
    int dur = std::chrono::duration_cast<std::chrono::milliseconds>(
                  std::chrono::high_resolution_clock::now() - currTime)
                  .count();
    if (dur > maxDur) {
      maxDur = dur;
    }
#endif // DO_INCCOORD5_TEST
       // end added test

    for (int rkel = 0; rkel < numPoints; rkel++) {
      float x, y, z;
      do {
        z = fDist(mtrand) * sd.fm;
        x = fDist(mtrand) * z * sd.rm;
        y = fDist(mtrand) * z * sd.um;
      } while (!usePyramid && x * x + y * y + z * z > sd.fm * sd.fm);

      hypervoxel::v::FVec<5> point = sd.c + x * sd.r + y * sd.u + z * sd.f;
      hypervoxel::v::IVec<5> v = randViewable(mtrand, point, sidel);

      // here's the real test!
      std::size_t i = ti.getIndex(v);
      if (i >= ti.getSize()) {
        std::cout << "ERROR!!! Index out of bounds" << std::endl;
        std::cout << i << std::endl;
        std::cout << "v: " << v << std::endl;
        std::cout << "point: " << point << std::endl;
        std::cout << "x,y,z: " << x << " " << y << " " << z << std::endl;
        ti.report();
        return 1;
      }
      hypervoxel::v::IVec<5> vr = ti.getCoord(i);
      if (vr != v) {
        std::cout << "ERROR!!! Vector not reproduced properly" << std::endl;
        std::cout << i << std::endl;
        std::cout << "v,vr: " << v << " " << vr << std::endl;
        std::cout << "point: " << point << std::endl;
        std::cout << "x,y,z: " << x << " " << y << " " << z << std::endl;
        ti.report();
        return 1;
      }
    }
  }
#ifdef DO_INCCOORD5_TEST
  std::cout << "max duration: " << maxDur << std::endl;
#endif
  return 0;
}

void printMaxSize(int numIndexers, bool usePyramid) {
  std::mt19937 mtrand(1);
  std::uniform_real_distribution<float> distro(-1, 1);

  std::uniform_real_distribution<float> fDist(0, 1);
  std::size_t maxSize = 0;
  hypervoxel::SliceDirs<5> maxSlices;
  char maxTi[sizeof(hypervoxel::TerrainIndexer)];
  for (int spam = 0; spam < numIndexers; spam++) {
    if (spam % 1000000 == 0) {
      std::cout << "Indexer #" << spam << std::endl;
    }
    auto lambda = [&]() -> float { return distro(mtrand); };
    hypervoxel::SliceDirs<5> sd = randSliceDirs(lambda, true);
    int sidel = 1; // can change this
    hypervoxel::TerrainIndexer ti(sd, sidel, usePyramid);
    if (ti.getSize() > maxSize) {
      maxSize = ti.getSize();
      maxSlices = sd;
      *(hypervoxel::TerrainIndexer *)maxTi = ti;
    }
  }
  std::cout << maxSize << std::endl;
  std::cout << "c: " << maxSlices.c << std::endl;
  std::cout << "r: " << maxSlices.r << std::endl;
  std::cout << "u: " << maxSlices.u << std::endl;
  std::cout << "f: " << maxSlices.f << std::endl;
  std::cout << "rm, um, fm: " << maxSlices.rm << " " << maxSlices.um << " "
            << maxSlices.fm << std::endl;
  (*(hypervoxel::TerrainIndexer *)maxTi).report();
}

int main() {
  return runTest(10000, 1, false);
  // printMaxSize(100000000, false);
}

