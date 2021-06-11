typedef struct FacesEdge {

  float3 a, b;
} FacesEdge;

typedef struct FacesEntry {

  ushort colors[5];
  FacesEdge edges[5][8];
} FacesEntry;

/// also caches terrain generator outputs
typedef struct FacesCacher {

  /// indexed by td, .x=color, .y=index in e below
  /// index of 0xFFFF means unassigned
  __global ushort2 *tc;
  TerrainIndexer td;

  uint maxLen;
  volatile __global uint *len;
  __global FacesEntry *e;
} FacesCacher;

/// c should be filled with 0xFF
void initFacesCacher(__global FacesCacher *f, __global ushort2 *c,
                     const SliceDirs *d, size_t maxSize,
                     volatile __global uint *len, __global void *e) {
  f->tc = c;
  initTerrainIndexer(&f->td, d, 1);
  f->maxLen = maxSize / sizeof(FacesEntry) - 1;
  if (f->maxLen > 0xFFFF) {
    f->maxLen = 0xFFFF;
  }
  *len = 0;
  f->len = len;
  f->e = e;
}

__global ushort2 *getTerrainEntry(__global FacesCacher *f, int8 v) {
  return f->tc + getIndex(&f->td, v);
}

__global FacesEdge *getEdge(__global FacesCacher *f, int8 v, ushort color,
                            uchar face, uchar d) {
  __global ushort2 *lp = getTerrainEntry(f, v);
  uint ind = lp->y;
  __global FacesEntry *e;
  if (ind == 0xFFFF) {
    ind = atomic_add(f->len, 1);
    if (ind >= f->maxLen) {
      atomic_sub(f->len, 1);
      return 0;
    }
    lp->y = ind;
  }
  e = &f->e[ind];
  e->colors[face] = color;
  return &e->edges[face][d - (d > face) - (d > face + 5)];
}

ushort getTerrain(__global FacesCacher *f, __global TerrainGenerator *g,
                  int8 v) {
  __global ushort2 *lp = getTerrainEntry(f, v);
  ushort r = lp->x;
  if (r == 0xFFFFU) {
    return lp->x = generateTerrain(g, v);
  }
  return r;
}

bool addEdgeHelper(__global FacesEdge *ptr, FacesEdge toass) {
  if (!ptr) {
    return false;
  }
  *ptr = toass;
  return true;
}

bool addEdge(__global FacesCacher *f, __global TerrainGenerator *g, int8 v,
             uchar d1, uchar d2, float3 a, float3 b) {
  int mod1 = 1, mod2 = 1;
  Int8NoScam vns;
  vns.v = v;
  Float8NoScam cam;
  cam.v = f->td.c;
  if (cam.s[d1] < vns.s[d1]) {
    vns.s[d1]--;
  } else {
    mod1 = -1;
  }
  if (cam.s[d2] < vns.s[d2]) {
    vns.s[d2]--;
  } else {
    mod2 = -1;
  }
  int8 fi = vns.v;
  vns.s[d1] += mod1;
  int8 s1i = vns.v;
  vns.s[d2] += mod2;
  int8 s2i = vns.v;
  vns.s[d1] -= mod1;
  int8 bi = vns.v;

  ushort front = getTerrain(f, g, fi);
  ushort s1 = getTerrain(f, g, s1i);
  ushort s2 = getTerrain(f, g, s2i);
  ushort back = getTerrain(f, g, bi);

  bool fv = !(front & 0x8000U);
  bool s1v = !(s1 & 0x8000U);
  bool s2v = !(s2 & 0x8000U);
  bool bv = !(back & 0x8000U);

  FacesEdge edg = {a, b};

  if (!fv) {
    if (s1v) {
      if (!addEdgeHelper(getEdge(f, s1i, s1, d1, d2), edg)) {
        return false;
      }
    }
    if (s2v) {
      if (!addEdgeHelper(getEdge(f, s2i, s2, d2, d1), edg)) {
        return false;
      }
    }
  }
  if (bv) {
    if (!s1v) {
      if (!addEdgeHelper(getEdge(f, bv, back, d2, d1), edg)) {
        return false;
      }
    }
    if (!s2v) {
      if (!addEdgeHelper(getEdge(f, bv, back, d1, d2), edg)) {
        return false;
      }
    }
  }
  return true;
}

