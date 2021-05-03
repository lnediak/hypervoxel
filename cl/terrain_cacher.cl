typedef struct TerrainCacher {

  uchar d1, d2, d3; /// isolated dimensions
  uchar d4, d5;     /// other dimensions
  float3 m4, m5;    /// row vectors for dotting
  float3 b4, b5;    /// base vector offset for getting min
  float o4, o5;     /// base vector offset to get final coord
  float8 a, b, c;   /// originals

  int3 cs;           /// starting coord in isolated dimensions
  size_t xs, ys, zs; /// x-stride, y-stride, z-stride
  size_t s4;         /// stride for other dimension 4

  __global ushort2 *data;
} TerrainCacher;

void initTerrainCacher(TerrainCacher *c, __global ushort2 *data,
                       const SliceDirs *d) {
  c->data = data;

  float8 rmod = d->fm * d->rm * d->r;
  float8 umod = d->fm * d->um * d->u;
  float8 tmp = d->c + d->fm * d->f;
  float8 tmpr = tmp + rmod;
  float8 tmpru = tmpr + umod;
  float8 tmprnu = tmpr - umod;
  float8 tmpnr = tmp - rmod;
  float8 tmpnru = tmpnr + umod;
  float8 tmpnrnu = tmpnr - umod;
  int8 lowest =
      getFloor8(min(d->c, min(tmpru, min(tmprnu, min(tmpnru, tmpnrnu)))) - 1.1);
  int8 higest =
      getFloor8(max(d->c, max(tmpru, max(tmprnu, max(tmpnru, tmpnrnu)))) + 2.1);
  int8 diffs = highest - lowest;

  float8 a = c->a = d->r;
  float8 b = c->b = d->u;
  float8 c = c->c = d->f;
  size_t mDataS = 0x7FFFFFFFFFFFFFFFL;
  const int N = 5;
  for (int d1 = 0; d1 < N - 2; d1++) {
    for (int d2 = a + 1; d2 < N - 1; d2++) {
      for (int d3 = b + 1; d3 < N; d3++) {
        float det = a[0] * b[1] * c[2] + a[1] * b[2] * c[0] +
                    a[2] * b[0] * c[1] - a[0] * b[2] * c[1] -
                    a[1] * b[0] * c[2] - a[2] * b[1] * c[0];
        if (-1e-4 < det && det < 1e-4) {
          continue;
        }
        float3 c1 = float3(b[1] * c[2] - b[2] * c[1], a[2] * c[1] - a[1] * c[2],
                           a[1] * b[2] - a[2] * b[1]) /
                    det;
        float3 c2 = float3(b[0] * c[2] - b[2] * c[0], a[2] * c[0] - a[0] * c[2],
                           a[0] * b[2] - a[2] * b[0]) /
                    det;
        float3 c3 = float3(b[0] * c[1] - b[1] * c[0], a[1] * c[0] - a[0] * c[1],
                           a[0] * b[1] - a[1] * b[0]) /
                    det;

        int d4 = 0;
        for (; d4 < N; d4++) {
          if (d4 != d1 && d4 != d2 && d4 != d3) {
            break;
          }
        }
        int d5 = d4 + 1;
        for (; d5 < N; d5++) {
          if (d5 != d1 && d5 != d2 && d5 != d3) {
            break;
          }
        }
        float3 m4 = float3(a[d4] * c1[0] + b[d4] * c1[1] + c[d4] * c1[2],
                           a[d4] * c2[0] + b[d4] * c2[1] + c[d4] * c2[2],
                           a[d4] * c3[0] + b[d4] * c3[1] + c[d4] * c3[2]);
        float3 m5 = float3(a[d5] * c1[0] + b[d5] * c1[1] + c[d5] * c1[2],
                           a[d5] * c2[0] + b[d5] * c2[1] + c[d5] * c2[2],
                           a[d5] * c3[0] + b[d5] * c3[1] + c[d5] * c3[2]);
        size_t w4 = getFloor(abs(m4[0]) + abs(m4[1]) + abs(m4[2]) + 1.00001);
        size_t w5 = getFloor(abs(m5[0]) + abs(m5[1]) + abs(m5[2]) + 1.00001);
        size_t wz = diffs[d3];
        size_t wy = diffs[d2];
        size_t wx = diffs[d1];
        size_t tmp = wx * wy * wz * w4 * w5;
        if (tmp < mDataS) {
          mDataS = tmp;
          c->d1 = d1;
          c->d2 = d2;
          c->d3 = d3;
          c->d4 = d4;
          c->d5 = d5;
          c->m4 = m4;
          c->m5 = m5;
          c->o4 = d->c[d4];
          c->o5 = d->c[d5];

          c->cs = int3(lowest[d1], lowest[d2], lowest[d3]);
          c->s4 = w5;
          c->zs = w4 * w5;
          c->ys = wz * d->zs;
          c->xs = wy * d->ys;
        }
      }
    }
  }
  float3 defBase = float3(d->c[c->d1], d->c[c->d2], d->c[c->d3]);
  c->b4 =
      defBase + float3(c->m4[c->d1] < 0, c->m4[c->d2] < 0, c->m4[c->d3] < 0);
  c->b5 =
      defBase + float3(c->m5[c->d1] < 0, c->m5[c->d2] < 0, c->m5[c->d3] < 0);
}

/// 5 dimensions lol
__global ushort2 *getLoc(const TerrainCacher *d, int8 v) {
  float3 vf = float3(v[d->d1], v[d->d2], v[d->d3]);
  float3 vf4 = vf - d->b4;
  int v4c = getHarshFloor(vf4[0] * d->m4[0] + vf4[1] * d->m4[1] +
                          vf4[2] * d->m4[2] + d->o4);
  float3 vf5 = vf - d->b5;
  int v5c = getHarshFloor(vf5[0] * d->m5[0] + vf5[1] * d->m5[1] +
                          vf5[2] * d->m5[2] + d->o5);
  return &data[(v[d->d1] - d->cs[0]) * d->xs + (v[d->d2] - d->cs[1]) * d->ys +
               (v[d->d3] - d->cs[2]) * d->zs + (v[d->d4] - v4c) * d->s4 +
               (v[d->d5] - v5c)];
}

