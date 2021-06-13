typedef struct TerrainIndexer {

  uchar d1, d2, d3; /// isolated dimensions
  uchar d4, d5;     /// other dimensions
  float3 m4, m5;    /// row vectors for dotting
  float3 b4, b5;    /// base vector offset for getting min
  float o4, o5;     /// base vector offset to get final coord
  float8 c;         /// camera

  int3 cs;           /// starting coord in isolated dimensions
  size_t xs, ys, zs; /// x-stride, y-stride, z-stride
  size_t s4;         /// stride for other dimension 4
} TerrainIndexer;

typedef union Int8NoScam {

  int8 v;
  int s[8];
} Int8NoScam;

typedef union Float8NoScam {

  float8 v;
  float s[8];
} Float8NoScam;

void initTerrainIndexer(__global TerrainIndexer *d, const SliceDirs *sd,
                        int sidel) {
  printf("r: %f, %f, %f, %f, %f\n", sd->r.s0, sd->r.s1, sd->r.s2, sd->r.s3,
         sd->r.s4);
  printf("u: %f, %f, %f, %f, %f\n", sd->u.s0, sd->u.s1, sd->u.s2, sd->u.s3,
         sd->u.s4);
  printf("f: %f, %f, %f, %f, %f\n", sd->f.s0, sd->f.s1, sd->f.s2, sd->f.s3,
         sd->f.s4);

  float8 rmod = sd->fm * sd->rm * sd->r;
  float8 umod = sd->fm * sd->um * sd->u;
  float8 tmp = sd->c + sd->fm * sd->f;
  float8 tmpr = tmp + rmod;
  float8 tmpru = tmpr + umod;
  float8 tmprnu = tmpr - umod;
  float8 tmpnr = tmp - rmod;
  float8 tmpnru = tmpnr + umod;
  float8 tmpnrnu = tmpnr - umod;
  Int8NoScam lowest;
  lowest.v = getFloor8(
      min(sd->c, min(tmpru, min(tmprnu, min(tmpnru, tmpnrnu)))) - 1e-4f);
  int8 highest = getFloor8(
      max(sd->c, max(tmpru, max(tmprnu, max(tmpnru, tmpnrnu)))) + 1.0001f);
  Int8NoScam diffs;
  diffs.v = highest - lowest.v;

  Float8NoScam a, b, c;
  a.v = sd->r;
  b.v = sd->u;
  c.v = sd->f;
  size_t mDataS = -1; // basically SIZE_T_MAX
  const int N = 5;
  for (int d1 = 0; d1 < N - 2; d1++) {
    for (int d2 = d1 + 1; d2 < N - 1; d2++) {
      for (int d3 = d2 + 1; d3 < N; d3++) {
        float det = a.s[d1] * b.s[d2] * c.s[d3] + a.s[d2] * b.s[d3] * c.s[d1] +
                    a.s[d3] * b.s[d1] * c.s[d2] - a.s[d1] * b.s[d3] * c.s[d2] -
                    a.s[d2] * b.s[d1] * c.s[d3] - a.s[d3] * b.s[d2] * c.s[d1];
        if (-1e-4 < det && det < 1e-4) {
          continue;
        }
        printf("d1, d2, d3: det: %i, %i, %i: %f\n", d1, d2, d3, det);
        // columns of the inverse matrix
        float3 c1 = (float3)(b.s[d2] * c.s[d3] - b.s[d3] * c.s[d2],
                             a.s[d3] * c.s[d2] - a.s[d2] * c.s[d3],
                             a.s[d2] * b.s[d3] - a.s[d3] * b.s[d2]) /
                    det;
        float3 c2 = (float3)(b.s[d3] * c.s[d1] - b.s[d1] * c.s[d3],
                             a.s[d1] * c.s[d3] - a.s[d3] * c.s[d1],
                             a.s[d3] * b.s[d1] - a.s[d1] * b.s[d3]) /
                    det;
        float3 c3 = (float3)(b.s[d1] * c.s[d2] - b.s[d2] * c.s[d1],
                             a.s[d2] * c.s[d1] - a.s[d1] * c.s[d2],
                             a.s[d1] * b.s[d2] - a.s[d2] * b.s[d1]) /
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
        // rows of a 2x3 matrix product
        float3 m4 =
            (float3)(a.s[d4] * c1.s0 + b.s[d4] * c1.s1 + c.s[d4] * c1.s2,
                     a.s[d4] * c2.s0 + b.s[d4] * c2.s1 + c.s[d4] * c2.s2,
                     a.s[d4] * c3.s0 + b.s[d4] * c3.s1 + c.s[d4] * c3.s2);
        float3 m5 =
            (float3)(a.s[d5] * c1.s0 + b.s[d5] * c1.s1 + c.s[d5] * c1.s2,
                     a.s[d5] * c2.s0 + b.s[d5] * c2.s1 + c.s[d5] * c2.s2,
                     a.s[d5] * c3.s0 + b.s[d5] * c3.s1 + c.s[d5] * c3.s2);
        size_t w4 = getFloor(sidel * (fabs(m4.s0) + fabs(m4.s1) + fabs(m4.s2)) +
                             1.00001);
        size_t w5 = getFloor(sidel * (fabs(m5.s0) + fabs(m5.s1) + fabs(m5.s2)) +
                             1.00001);
        size_t wz = diffs.s[d3];
        size_t wy = diffs.s[d2];
        size_t wx = diffs.s[d1];
        size_t tmp = wx * wy * wz * w4 * w5;

        printf("c1; c2; c3: %f, %f, %f; %f, %f, %f; %f, %f, %f\n", c1.x, c1.y,
               c1.z, c2.x, c2.y, c2.z, c3.x, c3.y, c3.z);
        printf("m4; m5: %f, %f, %f; %f, %f, %f\n", m4.x, m4.y, m4.z, m5.x, m5.y,
               m5.z);
        printf("wx, wy, wz, w4, w5, tmp: %i, %i, %i, %i, %i, %i\n", (int)wx,
               (int)wy, (int)wz, (int)w4, (int)w5, (int)tmp);

        if (tmp < mDataS) {
          mDataS = tmp;
          d->d1 = d1;
          d->d2 = d2;
          d->d3 = d3;
          d->d4 = d4;
          d->d5 = d5;
          d->m4 = m4;
          d->m5 = m5;

          d->cs = (int3)(lowest.s[d1], lowest.s[d2], lowest.s[d3]);
          d->s4 = w5;
          d->zs = w4 * w5;
          d->ys = wz * d->zs;
          d->xs = wy * d->ys;
        }
      }
    }
  }

  Float8NoScam cam;
  cam.v = d->c = sd->c;

  d->o4 = cam.s[d->d4];
  d->o5 = cam.s[d->d5];

  float3 defBase = (float3)(cam.s[d->d1], cam.s[d->d2], cam.s[d->d3]);
  d->b4 = defBase - sidel * (float3)(d->m4.s0 < 0, d->m4.s1 < 0, d->m4.s2 < 0);
  d->b5 = defBase - sidel * (float3)(d->m5.s0 < 0, d->m5.s1 < 0, d->m5.s2 < 0);
  printf("d1, d2, d3: %i, %i, %i\n", (int)d->d1, (int)d->d2, (int)d->d3);
}

/// 5 dimensions lol
size_t getIndex(const __global TerrainIndexer *d, int8 v) {
  Int8NoScam vv;
  vv.v = v;
  float3 vf = (float3)(vv.s[d->d1], vv.s[d->d2], vv.s[d->d3]);
  float3 vf4 = vf - d->b4;
  int v4c = getHarshFloor(dot(vf - d->b4, d->m4) + d->o4);
  float3 vf5 = vf - d->b5;
  int v5c = getHarshFloor(dot(vf - d->b5, d->m5) + d->o5);
  int kek = (vv.s[d->d1] - d->cs.s0) * d->xs +
            (vv.s[d->d2] - d->cs.s1) * d->ys +
            (vv.s[d->d3] - d->cs.s2) * d->zs + (vv.s[d->d4] - v4c) * d->s4 +
            (vv.s[d->d5] - v5c);

  // FIXME: DEBUG SHIT
  if (kek < 0) {
    printf("TerrainIndexer: d1, d2, d3: %i, %i, %i\n", (int)(d->d1),
           (int)(d->d2), (int)(d->d3));
    printf("v: %i, %i, %i, %i, %i\n", v.s0, v.s1, v.s2, v.s3, v.s4);
    printf("d->cs: %i, %i, %i\n", d->cs.s0, d->cs.s1, d->cs.s2);
    printf("pre-v4c, pre-v5c: %f, %f\n", dot(vf - d->b4, d->m4) + d->o4, dot(vf - d->b5, d->m5) + d->o5);
    printf("v4c, v5c: %i, %i\n", v4c, v5c);
    printf("getIndex of %i, %i, %i, %i, %i: %i\n", v.s0, v.s1, v.s2, v.s3, v.s4,
           kek);
  }

  return kek;
}

