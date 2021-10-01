typedef struct MinI {
  float f;
  int i;
} MinI;

MinI minMinI(MinI a, MinI b) {
  int val = a.f > b.f;
  a.f = min(a.f, b.f);
  a.i = val ? b.i : a.i;
  return a;
}

MinI minMinI5(float8 v) {
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
  return minMinI(x, y);
}

float8 deriveRay(const SliceDirs *sd) {
  float ium = 0.5f + get_global_id(0) - get_global_size(0) / 2;
  ium /= get_global_size(0);
  float irm = 0.5f + get_global_id(1) - get_global_size(1) / 2;
  irm /= get_global_size(1);

  float8 ray = sd->f + irm * sd->rm * sd->r + ium * sd->um * sd->u;
  float nrm = ray.s0 * ray.s0 + ray.s1 * ray.s1 + ray.s2 * ray.s2 +
              ray.s3 * ray.s3 + ray.s4 * ray.s4;
  return ray * (sd->fm / sqrt(nrm));
}

__kernel void renderTerrain(__global const float *fsd,
                            __global const float *fti1,
                            __global const float *d1, __global uchar *out) {
  out += 4 * (get_global_id(0) * get_global_size(1) + get_global_id(1));

  SliceDirs sd;
  initSliceDirs(&sd, fsd);
  float8 c = sd.c;

  float8 ray = deriveRay(&sd);

  TerrainIndexer ti1;
  initTerrainIndexer(&ti1, fti1);

  int8 cb = fastFloor5(c); // current block
  float8 invs = 1.f / ray;
  float8 ainvs = fabs(invs);

  // OpenCL > operator is so scuffed
  // it is -1 when true, not 1
  int8 fsig0 = invs > 0;
  float8 fsig1 = -0.00001f - 1.00002f * int52float5(fsig0);
  int8 fsig2 = -1 - 2 * fsig0;

  float8 tds = (fsig1 + int52float5(cb) - c) * invs;
  float dist = 0;
  while (true) {
    float val = d1[getIndex(&ti1, cb)];
    //float val = (cb.s0 + cb.s1 + cb.s2 + cb.s3 + cb.s4 + 23456) & 8191;
    //val -= 8190;
    if (val > 0) {
      float8 tmp = c + dist * ray - int52float5(cb);
      uchar kek = 30 + 30 * (tmp.s0 + tmp.s1 + tmp.s2 + tmp.s3 + tmp.s4);
      *out++ = kek;
      *out++ = kek;
      *out++ = kek;
      *out = 255;
      return;
    }
    float8 weird = fmin(fmin(tds.s12340567, tds.s23401567),
                        fmin(tds.s34012567, tds.s40123567));
    dist = fmin(tds.s0, weird.s0);
    if (dist >= 1) {
      return;
    }
    // same thing here, -1 if true. hence -= instead of +=
    int8 mask = tds <= weird;
    cb -= mask * fsig2;
    tds -= int52float5(mask) * ainvs;
  }
}

