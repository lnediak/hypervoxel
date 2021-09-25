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

  int8 v = getCoord5(&ti1, index);
  float8 l = int52float5(v) / TERGEN_CACHE_SCALE;
  int8 sv = fastFloor5(l);
  size_t si = getIndex(&tis, sv);

  /*printf("%d\n", si);
  printf("%d,%d,%d,%d,%d(orig)", v.s0, v.s1, v.s2, v.s3, v.s4);
  printf("%d,%d,%d,%d,%d(sv)", sv.s0, sv.s1, sv.s2, sv.s3, sv.s4);
  int8 svret = getCoord5(&tis, si);
  printf("%d,%d,%d,%d,%d(sv-return)\n", svret.s0, svret.s1, svret.s2, svret.s3,
         svret.s4);*/

  __global const float16 *d32 = ds + si * 2;

  /*printf("d32[0]: %f,%f  %f,%f    %f,%f  %f,%f      %f,%f  %f,%f     "
         "%f,%f   %f,%f\n",
         d32[0].s0, d32[0].s1, d32[0].s2, d32[0].s3, d32[0].s4, d32[0].s5,
         d32[0].s6, d32[0].s7, d32[0].s8, d32[0].s9, d32[0].s10, d32[0].s11,
         d32[0].s12, d32[0].s13, d32[0].s14, d32[0].s15);*/

  l -= int52float5(sv);

  // printf("l: %f,%f,%f,%f,%f\n", l.s0, l.s1, l.s2, l.s3, l.s4);

  l = l * l * (3 - 2 * l);
  float16 e16 = d32[0] + (d32[1] - d32[0]) * l.s4;
  float8 e8 = e16.lo + (e16.hi - e16.lo) * l.s3;
  float4 e4 = e8.lo + (e8.hi - e8.lo) * l.s2;
  float2 e2 = e4.lo + (e4.hi - e4.lo) * l.s1;
  d1[index] = e2.lo + (e2.hi - e2.lo) * l.s0;
}

