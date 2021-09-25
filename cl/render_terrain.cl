typedef struct MinI {
  float f;
  int i;
} MinI;

MinI minMinI(MinI a, MinI b) {
  int val = a.f > b.f;
  a.f = min(a.f, b.f);
  a.i += val * (b.i - a.i);
  return a;
}

MinI minMinI5(float8 v) {
  // printf("minMinI5 test! v: %f,%f,%f,%f,%f", v.s0, v.s1, v.s2, v.s3, v.s4);
  MinI x;
  x.f = v.s0;
  x.i = 0;
  MinI y;
  y.f = v.s1;
  y.i = 1;
  x = minMinI(x, y);
  y.f = v.s2;
  y.i = 2;
  x = minMinI(x, y);
  y.f = v.s3;
  y.i = 3;
  x = minMinI(x, y);
  y.f = v.s4;
  y.i = 4;
  // printf(", and the result is: %f,%d\n", minMinI(x, y).f, minMinI(x, y).i);
  return minMinI(x, y);
}

__kernel void renderTerrain(__global const float *fsd,
                            __global const float *fti1,
                            __global const float *d1, __global uchar *out) {
  out += 4 * (get_global_id(0) * get_global_size(1) + get_global_id(1));

  SliceDirs sd;
  initSliceDirs(&sd, fsd);
  float8 c = sd.c;

  float ium = 0.5f + get_global_id(0) - get_global_size(0) / 2;
  ium /= get_global_size(0);
  float irm = 0.5f + get_global_id(1) - get_global_size(1) / 2;
  irm /= get_global_size(1);

  float8 ray = sd.f + irm * sd.rm * sd.r + ium * sd.um * sd.u;
  float nrm = ray.s0 * ray.s0 + ray.s1 * ray.s1 + ray.s2 * ray.s2 +
              ray.s3 * ray.s3 + ray.s4 * ray.s4;
  ray *= sd.fm / sqrt(nrm);

  TerrainIndexer ti1;
  initTerrainIndexer(&ti1, fti1);

  Int8NoScam cb; // current block
  cb.v = fastFloor5(c);
  float8 invs = 1.f / ray;
  Float8NoScam ainvs;
  ainvs.v = fabs(invs);

  // OpenCL > operator is so scuffed
  // it is -1 when true, not 1
  float8 fsig0 = -int52float5(invs > 0);
  float8 fsig1 = -0.00001f + 1.00002f * fsig0;
  Float8NoScam fsig2;
  fsig2.v = -1.f + 2.f * fsig0;

  Float8NoScam tds;
  tds.v = (fsig1 + int52float5(cb.v) - c) * invs;
  float dist = 0;
  while (true) {

    /*int8 cbret = getCoord5(&ti1, getIndex(&ti1, cb.v));
    if (cb.s[0] != cbret.s0 || cb.s[1] != cbret.s1 || cb.s[2] != cbret.s2 ||
        cbret.s3 != cbret.s3 || cbret.s4 != cbret.s4) {
      printf("cb.v: %d,%d,%d,%d,%d\n", cb.s[0], cb.s[1], cb.s[2], cb.s[3],
             cb.s[4]);
      printf("index: %d\n", getIndex(&ti1, cb.v));
      printf("cb.v-ret: %d,%d,%d,%d,%d\n", cbret.s0, cbret.s1, cbret.s2,
             cbret.s3, cbret.s4);
      printf("ray: %f,%f,%f,%f,%f\n", ray.s0, ray.s1, ray.s2, ray.s3, ray.s4);
      printf("invs: %f,%f,%f,%f,%f\n", invs.s0, invs.s1, invs.s2, invs.s3,
             invs.s4);
      printf("fsig0: %f,%f,%f,%f,%f\n", fsig0.s0, fsig0.s1, fsig0.s2, fsig0.s3,
             fsig0.s4);
      printf("fsig2: %f,%f,%f,%f,%f\n", fsig2.s[0], fsig2.s[1], fsig2.s[2],
             fsig2.s[3], fsig2.s[4]);
      printf("tds: %f,%f,%f,%f,%f\n", tds.s[0], tds.s[1], tds.s[2], tds.s[3],
             tds.s[4]);
    }*/

    float val = d1[getIndex(&ti1, cb.v)];
    if (val > 0) {
      float8 tmp = c + dist * ray - int52float5(cb.v);
      uchar kek = 30 + 30 * (tmp.s0 + tmp.s1 + tmp.s2 + tmp.s3 + tmp.s4);
      *out++ = kek;
      *out++ = kek;
      *out++ = kek;
      *out = 255;
      return;
    }
    MinI minD = minMinI5(tds.v);
    if (minD.f >= 1) {
      return;
    }
    cb.s[minD.i] += fsig2.s[minD.i];
    dist = tds.s[minD.i];
    tds.s[minD.i] += ainvs.s[minD.i];
  }
}

