typedef struct PerlinOptions {

  float8 scale;
  __global float8 *gradVecs; /// actually float5, but yeah
  uint numGradVecsMask, numOctaves;
  float persistence;
} PerlinOptions;

typedef struct UInt32 {

  uint s[32];
} UInt32;

/// Perlin noise terrain generator
typedef struct TerrainGenerator {

  __global UInt32 *tc1; /// caching indices of corners, for tc2
  __global float *tc2;  /// caching noise values on a fat grid
  TerrainIndexer td1, td2;

  PerlinOptions o;
} TerrainGenerator;

#define TERGEN_CACHE_SCALE 16

/// both c1 and c2 should be filled with 0xFF (that makes the floats NaN)
void initTerrainGenerator(__global TerrainGenerator *g, __global UInt32 *c1,
                          __global float *c2, const SliceDirs *d,
                          PerlinOptions o) {
  g->tc1 = c1;
  g->tc2 = c2;
  SliceDirs dl;
  dl.c = d->c / TERGEN_CACHE_SCALE;
  dl.fm = d->fm / TERGEN_CACHE_SCALE;
  initTerrainIndexer(&g->td1, &dl, 1);
  initTerrainIndexer(&g->td2, &dl, 2);
  g->o = o;
}

uint hscram(uint v) {
  v *= 0xcc9e2d51;
  v = (v << 15) | (v >> 17);
  v *= 0x1b873593;
  return v;
}

uint hstep(uint t, uint v) {
  t ^= hscram(v);
  t = (t << 13) | (t >> 19);
  return t * 5 + 0xe6546b64;
}

uint int5Hash(int a, int b, int c, int d, int e) {
  return hstep(hstep(hstep(hstep(0, a), b), c), d) ^ hscram(e);
}

uint16 cornersIntHash5(int8 v) {
  return (uint16)(int5Hash(v.s0 + 0, v.s1 + 0, v.s2 + 0, v.s3 + 0, v.s4),
                  int5Hash(v.s0 + 0, v.s1 + 0, v.s2 + 0, v.s3 + 1, v.s4),
                  int5Hash(v.s0 + 0, v.s1 + 0, v.s2 + 1, v.s3 + 0, v.s4),
                  int5Hash(v.s0 + 0, v.s1 + 0, v.s2 + 1, v.s3 + 1, v.s4),

                  int5Hash(v.s0 + 0, v.s1 + 1, v.s2 + 0, v.s3 + 0, v.s4),
                  int5Hash(v.s0 + 0, v.s1 + 1, v.s2 + 0, v.s3 + 1, v.s4),
                  int5Hash(v.s0 + 0, v.s1 + 1, v.s2 + 1, v.s3 + 0, v.s4),
                  int5Hash(v.s0 + 0, v.s1 + 1, v.s2 + 1, v.s3 + 1, v.s4),

                  int5Hash(v.s0 + 1, v.s1 + 0, v.s2 + 0, v.s3 + 0, v.s4),
                  int5Hash(v.s0 + 1, v.s1 + 0, v.s2 + 0, v.s3 + 1, v.s4),
                  int5Hash(v.s0 + 1, v.s1 + 0, v.s2 + 1, v.s3 + 0, v.s4),
                  int5Hash(v.s0 + 1, v.s1 + 0, v.s2 + 1, v.s3 + 1, v.s4),

                  int5Hash(v.s0 + 1, v.s1 + 1, v.s2 + 0, v.s3 + 0, v.s4),
                  int5Hash(v.s0 + 1, v.s1 + 1, v.s2 + 0, v.s3 + 1, v.s4),
                  int5Hash(v.s0 + 1, v.s1 + 1, v.s2 + 1, v.s3 + 0, v.s4),
                  int5Hash(v.s0 + 1, v.s1 + 1, v.s2 + 1, v.s3 + 1, v.s4));
}

float16 terGenHelp(__global float8 *g, uint16 i, float8 v0, float8 v1,
                   float val4) {
  float4 v0l = v0.lo;
  float4 v1l = v1.lo;
  float16 t;
  float8 gg0 = g[i.s0];
  t.s0 = dot(gg0.lo, v0l) + gg0.s4 * val4;
  float8 gg1 = g[i.s1];
  t.s1 = dot(gg1.s012, v0l.s012) + gg1.s3 * v1l.s3 + gg1.s4 * val4;
  float8 gg2 = g[i.s2];
  t.s2 = dot(gg2.s013, v0l.s013) + gg2.s2 * v1l.s2 + gg2.s4 * val4;
  float8 gg3 = g[i.s3];
  t.s3 = dot(gg3.s01, v0l.s01) + dot(gg3.s23, v1l.s23) + gg3.s4 * val4;

  float8 gg4 = g[i.s4];
  t.s4 = dot(gg4.s023, v0l.s023) + gg4.s1 * v1l.s1 + gg4.s4 * val4;
  float8 gg5 = g[i.s5];
  t.s5 = dot(gg3.s02, v0l.s02) + dot(gg3.s13, v1l.s13) + gg3.s4 * val4;
  float8 gg6 = g[i.s6];
  t.s6 = dot(gg3.s03, v0l.s03) + dot(gg3.s12, v1l.s12) + gg3.s4 * val4;
  float8 gg7 = g[i.s7];
  t.s7 = gg7.s0 * v0l.s0 + dot(gg7.s123, v1l.s123) + gg7.s4 * val4;

  float8 gg8 = g[i.s8];
  t.s8 = gg7.s0 * v1l.s0 + dot(gg7.s123, v0l.s123) + gg7.s4 * val4;
  float8 gg9 = g[i.s9];
  t.s9 = dot(gg9.s03, v1l.s03) + dot(gg9.s12, v0l.s12) + gg9.s4 * val4;
  float8 ggA = g[i.sA];
  t.sA = dot(ggA.s02, v1l.s02) + dot(ggA.s13, v0l.s13) + ggA.s4 * val4;
  float8 ggB = g[i.sB];
  t.sB = dot(ggB.s023, v1l.s023) + ggB.s1 * v0l.s1 + ggB.s4 * val4;

  float8 ggC = g[i.sC];
  t.sC = dot(ggC.s01, v1l.s01) + dot(ggC.s23, v0l.s23) + ggC.s4 * val4;
  float8 ggD = g[i.sD];
  t.sD = dot(ggD.s013, v1l.s013) + ggD.s2 * v0l.s2 + ggD.s4 * val4;
  float8 ggE = g[i.sE];
  t.sE = dot(ggE.s012, v1l.s012) + ggE.s3 * v0l.s3 + ggE.s4 * val4;
  float8 ggF = g[i.sF];
  t.sF = dot(ggF.lo, v1l) + ggF.s4 * val4;

  return t;
}

float superLerp(float16 f0, float16 f1, float8 l0, float8 l1) {
  float16 fl16 = f0 * l0.s4 + f1 * l1.s4;
  float8 fl8 = fl16.lo * l0.s0 + fl16.hi * l1.s0;
  float4 fl4 = fl8.lo * l0.s1 + fl8.hi * l1.s1;
  float2 fl2 = fl4.lo * l0.s2 + fl4.hi * l1.s2;
  return fl2.lo * l0.s3 + fl2.hi * l1.s3;
}

float getTerrainVal(const __global TerrainGenerator *g, float8 v) {
  float total = 0;
  float amplitude = 1;
  v /= g->o.scale;
  for (uint i = g->o.numOctaves; i--;) {
    int8 vi = fastRound5(v);
    float8 v0 = v - int52float5(vi);
    float8 v1 = v0 - 1;
    float8 l0 = v0 * v0 * (3 - 2 * v0);
    float8 l1 = 1 - l0; // == v1 * v1 * (3 - 2 * v1)

    uint16 i0 = (cornersIntHash5(vi) ^ i) & g->o.numGradVecsMask;
    float16 f0 = terGenHelp(g->o.gradVecs, i0, v0, v1, v0.s4);
    vi.s4++;
    uint16 i1 = (cornersIntHash5(vi) ^ i) & g->o.numGradVecsMask;
    float16 f1 = terGenHelp(g->o.gradVecs, i1, v0, v1, v1.s4);

    float fl = superLerp(f0, f1, l0, l1);

    total += fl * amplitude;
    amplitude *= g->o.persistence;
    v *= 2;
  }
  return total;
}

float terGenCacheHelp(__global TerrainGenerator *g, __global uint *i, int8 vs,
                      float8 v) {
  uint ii = *i;
  if (ii == 0xFFFFFFFFU) {
    *i = ii = getIndex(&g->td2, vs);
  }
  float fv = g->tc2[ii];
  if (isnan(fv)) {
    g->tc2[ii] = fv = getTerrainVal(g, v);
  }
  return fv;
}

float16 terGenCacheHelp16(__global TerrainGenerator *g, __global uint *i,
                          int8 vs, int v4, float8 v) {
  return (float16)(
      terGenCacheHelp(g, i + 0, vs + (int8)(0, 0, 0, 0, v4, 0, 0, 0),
                      v + TERGEN_CACHE_SCALE *
                              (float8)(0, 0, 0, 0, v4, 0, 0, 0)),
      terGenCacheHelp(g, i + 1, vs + (int8)(0, 0, 0, 1, v4, 0, 0, 0),
                      v + TERGEN_CACHE_SCALE *
                              (float8)(0, 0, 0, 1, v4, 0, 0, 0)),
      terGenCacheHelp(g, i + 2, vs + (int8)(0, 0, 1, 0, v4, 0, 0, 0),
                      v + TERGEN_CACHE_SCALE *
                              (float8)(0, 0, 1, 0, v4, 0, 0, 0)),
      terGenCacheHelp(g, i + 3, vs + (int8)(0, 0, 1, 1, v4, 0, 0, 0),
                      v + TERGEN_CACHE_SCALE *
                              (float8)(0, 0, 1, 1, v4, 0, 0, 0)),

      terGenCacheHelp(g, i + 4, vs + (int8)(0, 1, 0, 0, v4, 0, 0, 0),
                      v + TERGEN_CACHE_SCALE *
                              (float8)(0, 1, 0, 0, v4, 0, 0, 0)),
      terGenCacheHelp(g, i + 5, vs + (int8)(0, 1, 0, 1, v4, 0, 0, 0),
                      v + TERGEN_CACHE_SCALE *
                              (float8)(0, 1, 0, 1, v4, 0, 0, 0)),
      terGenCacheHelp(g, i + 6, vs + (int8)(0, 1, 1, 0, v4, 0, 0, 0),
                      v + TERGEN_CACHE_SCALE *
                              (float8)(0, 1, 1, 0, v4, 0, 0, 0)),
      terGenCacheHelp(g, i + 7, vs + (int8)(0, 1, 1, 1, v4, 0, 0, 0),
                      v + TERGEN_CACHE_SCALE *
                              (float8)(0, 1, 1, 1, v4, 0, 0, 0)),

      terGenCacheHelp(g, i + 8, vs + (int8)(1, 0, 0, 0, v4, 0, 0, 0),
                      v + TERGEN_CACHE_SCALE *
                              (float8)(1, 0, 0, 0, v4, 0, 0, 0)),
      terGenCacheHelp(g, i + 9, vs + (int8)(1, 0, 0, 1, v4, 0, 0, 0),
                      v + TERGEN_CACHE_SCALE *
                              (float8)(1, 0, 0, 1, v4, 0, 0, 0)),
      terGenCacheHelp(g, i + 10, vs + (int8)(1, 0, 1, 0, v4, 0, 0, 0),
                      v + TERGEN_CACHE_SCALE *
                              (float8)(1, 0, 1, 0, v4, 0, 0, 0)),
      terGenCacheHelp(g, i + 11, vs + (int8)(1, 0, 1, 1, v4, 0, 0, 0),
                      v + TERGEN_CACHE_SCALE *
                              (float8)(1, 0, 1, 1, v4, 0, 0, 0)),

      terGenCacheHelp(g, i + 12, vs + (int8)(1, 1, 0, 0, v4, 0, 0, 0),
                      v + TERGEN_CACHE_SCALE *
                              (float8)(1, 1, 0, 0, v4, 0, 0, 0)),
      terGenCacheHelp(g, i + 13, vs + (int8)(1, 1, 0, 1, v4, 0, 0, 0),
                      v + TERGEN_CACHE_SCALE *
                              (float8)(1, 1, 0, 1, v4, 0, 0, 0)),
      terGenCacheHelp(g, i + 14, vs + (int8)(1, 1, 1, 0, v4, 0, 0, 0),
                      v + TERGEN_CACHE_SCALE *
                              (float8)(1, 1, 1, 0, v4, 0, 0, 0)),
      terGenCacheHelp(g, i + 15, vs + (int8)(1, 1, 1, 1, v4, 0, 0, 0),
                      v + TERGEN_CACHE_SCALE *
                              (float8)(1, 1, 1, 1, v4, 0, 0, 0)));
}

ushort generateTerrain(__global TerrainGenerator *g, int8 v) {
  int8 vs = v / TERGEN_CACHE_SCALE;
  float8 vf = int52float5(vs * TERGEN_CACHE_SCALE);
  float8 l0 = (int52float5(v) - vf) / TERGEN_CACHE_SCALE;
  float8 l1 = 1 - l0;
  __global UInt32 *inds = &g->tc1[getIndex(&g->td1, vs)];
  vs -= 1;
  float16 lo = terGenCacheHelp16(g, inds->s, vs, 0, vf);
  float16 hi = terGenCacheHelp16(g, inds->s + 16, vs, 1, vf);
  float fl = superLerp(lo, hi, l0, l1);

  return 0x7FFFU * (fl > 0.3);
}

