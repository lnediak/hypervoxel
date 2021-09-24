#define TERGEN_CACHE_SCALE 16

__kernel void lerpTerrain(unsigned sz, __global const float *ftis,
                          __global const float *fti1,
                          __global const float16 *ds, __global float *d1) {
  TerrainIndexer tis, ti1;
  initTerrainIndexer(&tis, ftis);
  initTerrainIndexer(&ti1, fti1);

  size_t index = get_global_id(0);
  if (index >= sz) {
    return;
  }

  int8 v = getCoord5(&ti, index);
  int8 sv = v / TERGEN_CACHE_SCALE;
  size_t si = getI5(&tis, sv);
  __global const float16 *d32 = ds + si * 2;

  float8 l = int2float5(v) / TERGEN_CACHE_SCALE - sv;
  l = l * l * (3 - 2 * l);
  float16 e16 = d32[0] + (d32[1] - d32[0]) * l.s4;
  float8 e8 = e16.lo + (e16.hi - e16.lo) * l.s3;
  float4 e4 = e8.lo + (e8.hi - e8.lo) * l.s2;
  float2 e2 = e4.lo + (e4.hi - e4.lo) * l.s1;
  d1[index] = e2.lo + (e2.hi - e2.lo) * l.s0;
}

