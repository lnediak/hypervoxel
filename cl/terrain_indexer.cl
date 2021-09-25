typedef struct TerrainIndexer {

  int d1, d2, d3; /// isolated dimensions
  int d4, d5;     /// other dimensions
  float3 m4, m5;  /// row vectors for dotting
  float3 b4, b5;  /// base vector offset for getting min
  float o4, o5;   /// base vector offset to get final coord

  int3 cs;           /// starting coord in isolated dimensions
  size_t xs, ys, zs; /// x-stride, y-stride, z-stride
  size_t s4;         /// stride for other dimension 4
} TerrainIndexer;

void initTerrainIndexer(TerrainIndexer *d, __global float *fd) {
  __global int *id = (__global int *)fd;
  d->d1 = *id++;
  d->d2 = *id++;
  d->d3 = *id++;
  d->d4 = *id++;
  d->d5 = *id++;

  fd = (__global float *)id;
  d->m4.s0 = *fd++;
  d->m4.s1 = *fd++;
  d->m4.s2 = *fd++;
  d->m5.s0 = *fd++;
  d->m5.s1 = *fd++;
  d->m5.s2 = *fd++;

  d->b4.s0 = *fd++;
  d->b4.s1 = *fd++;
  d->b4.s2 = *fd++;
  d->b5.s0 = *fd++;
  d->b5.s1 = *fd++;
  d->b5.s2 = *fd++;

  d->o4 = *fd++;
  d->o5 = *fd++;

  id = (__global float *)fd;
  d->cs.s0 = *id++;
  d->cs.s1 = *id++;
  d->cs.s2 = *id++;
  d->xs = *id++;
  d->ys = *id++;
  d->zs = *id++;
  d->s4 = *id++;
}

typedef union Int8NoScam {

  int8 v;
  int s[8];
} Int8NoScam;

typedef union Float8NoScam {

  float8 v;
  float s[8];
} Float8NoScam;

void getV4cV5c(const TerrainIndexer *d, int v1, int v2, int v3, int *v4c,
               int *v5c) {
  float3 vf = (float3)(v1, v2, v3);
  *v4c = harshFloor(dot(vf - d->b4, d->m4) + d->o4);
  *v5c = harshFloor(dot(vf - d->b5, d->m5) + d->o5);
}

size_t getIndex(const TerrainIndexer *d, int8 v) {
  Int8NoScam vv;
  vv.v = v;
  int v4c, v5c;
  getV4cV5c(d, vv.s[d->d1], vv.s[d->d2], vv.s[d->d3], &v4c, &v5c);
  size_t kek = (vv.s[d->d1] - d->cs.s0) * d->xs +
               (vv.s[d->d2] - d->cs.s1) * d->ys +
               (vv.s[d->d3] - d->cs.s2) * d->zs + (vv.s[d->d4] - v4c) * d->s4 +
               (vv.s[d->d5] - v5c);
  return kek;
}

int8 getI5(const TerrainIndexer *d, size_t i) {
  int8 i5;
  i -= d->xs * (i5.s0 = i / d->xs);
  i -= d->ys * (i5.s1 = i / d->ys);
  i -= d->zs * (i5.s2 = i / d->zs);
  i -= d->s4 * (i5.s3 = i / d->s4);
  i5.s4 = i;
  return i5;
}

int8 getCoord55(const TerrainIndexer *d, int8 i5) {
  Int8NoScam ret;
  int v1 = ret.s[d->d1] = i5.s0 + d->cs.s0;
  int v2 = ret.s[d->d2] = i5.s1 + d->cs.s1;
  int v3 = ret.s[d->d3] = i5.s2 + d->cs.s2;
  int v4c, v5c;
  getV4cV5c(d, v1, v2, v3, &v4c, &v5c);
  ret.s[d->d4] = i5.s3 + v4c;
  ret.s[d->d5] = i5.s4 + v5c;
  return ret.v;
}

int8 getCoord5(const TerrainIndexer *d, size_t i) {
  return getCoord55(d, getI5(d, i));
}

