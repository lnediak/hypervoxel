#ifndef offsetof
#define offsetof(T, a) ((size_t)(&((T *)0)->a))
#endif

__kernel void getOffs(__global uint *offs) {
  offs[0] = sizeof(FacesEntry);
  offs[1] = sizeof(FacesEdge);
  offs[2] = offsetof(FacesEntry, edges);
  offs[3] = offsetof(FacesEdge, b);
  offs[4] = sizeof(FacesCacher);
  offs[5] = sizeof(TerrainGenerator);
}

/// number of work-items should be size/128
__kernel void clearBuf(__global ulong16 *buf) {
  buf[get_global_id(0)] = 0xFFFFFFFFFFFFFFFFU;
}

/**
  fs format:
  float5 c, r, u, f;
  float fm, rm, um;

  noting that float5 just means five floats in a row

  us2s needs size to fit normal (sidel=1, no scaling) TerrainIndexed buf
  data is user-specified: sz is max byte-size, len will contain size in entries
  ui32s needs size to fit scaled (sidel=1, though) TerrainIndexed buf
  floats2 needs size to fit scaled and weird (sidel=2) TerrainIndexed buf

  o format:
  float5 scale;
  uint numGradVecsMask, numOctaves;
  float persistence;
  float8 gradVecs[numGradVecsMask + 1];

  Do only run this with one work-item, and after running clearBuf on us2s,
  ui32s, and floats2.
*/
__kernel void initGlobalsForMain(__global FacesCacher *f,
                                 __global TerrainGenerator *g,
                                 __global float *fs, __global ushort2 *us2s,
                                 uint sz, __global uint *len,
                                 __global void *data, __global void *ui32s,
                                 __global float *floats2, __global float *o) {
  SliceDirs d;
  d.c = (float8)(fs[0], fs[1], fs[2], fs[3], fs[4], 0, 0, 0);
  d.r = (float8)(fs[5], fs[6], fs[7], fs[8], fs[9], 0, 0, 0);
  d.u = (float8)(fs[10], fs[11], fs[12], fs[13], fs[14], 0, 0, 0);
  d.f = (float8)(fs[15], fs[16], fs[17], fs[18], fs[19], 0, 0, 0);
  d.fm = fs[20];
  d.rm = fs[21];
  d.um = fs[22];
  initFacesCacher(f, us2s, &d, sz, len, data);

  PerlinOptions pops;
  pops.scale = (float8)(o[0], o[1], o[2], o[3], o[4], 0, 0, 0);
  pops.numGradVecsMask = *(__global uint *)&o[5];
  pops.numOctaves = *(__global uint *)&o[6];
  pops.persistence = o[7];
  pops.gradVecs = (__global float8 *)&o[8];
  initTerrainGenerator(g, ui32s, floats2, &d, pops);
}

/**
  lp format:
  float5 a, b;
  float3 a3, b3;
  ushort d1, d2;
  - and repeat get_global_size(0) times

  noting that float5 just means five floats in a row

  Do only call this just after initGlobalsForMain.
  After this, one can read from data and len as entered in initGlobalsForMain,
  using the output of getOffs.
*/
__kernel void mainKernel(__global FacesCacher *f, __global TerrainGenerator *g,
                         __global float *lp, float dist1, float dist2) {
  lp += get_global_id(0) * 17;
  Line l;
  l.a = (float8)(lp[0], lp[1], lp[2], lp[3], lp[4], 0, 0, 0);
  l.b = (float8)(lp[5], lp[6], lp[7], lp[8], lp[9], 0, 0, 0);
  l.a3 = (float3)(lp[10], lp[11], lp[12]);
  l.b3 = (float3)(lp[13], lp[14], lp[15]);
  __global ushort *n = (__global ushort *)&lp[16];
  l.d1 = n[0];
  l.d2 = n[1];
  followLine(&l, f, g, dist1, dist2);
}

