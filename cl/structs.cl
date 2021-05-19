typedef struct FacesEdge {

  float3 a, b;
} FacesEdge;

typedef struct FacesEntry {

  ushort colors[5];
  FacesEdge edges[5][8][6];
} FacesEntry;

typedef struct FacesCacher {

  __global ushort2 *tc;
  TerrainIndexer td;

  ushort maxLen1;
  volatile __local uint *len;
  __global FacesEntry *e;
};

void initFacesCacher(FacesCacher *f, __global ushort2 *c, const SliceDirs *d,
                     size_t maxSize, volatile __local uint *len,
                     __global void *e) {
  f->tc = c;
  initTerrainCacher(&f->td, d, 1);
  f->maxLen = maxSize / sizeof(FacesEntry) - 1;
  *len = 0;
  f->len = len;
  f->e = e;
}

__global ushort2 *getTerrainEntry(FacesCacher *f, int8 v) {
  return f->tc + getIndex(&f->td, v);
}

FacesEdge *getEdge(FacesCacher *f, int8 v, ushort color, uchar face, uchar d1,
                   uchar d2) {
  __global ushort2 *lp = getTerrainEntry(f, v);
  uint ind = lp->y;
  if (ind == 0xFFFF) {
    ind = atomic_add(f->len, 1);
    if (ind >= maxLen) {
      atomic_sub(f->len, 1);
      return NULL;
    }
    lp->y = ind;
  }
  FacesEntry *e = &f->e[ind];
  e->colors[face] = color;
  uchar d1f = d1 >= 5 ? d1 - 5 : d1 + 5;
  return &e->edges[face][d1 - (d1 > face) - (d1 > face + 5)]
                  [d2 - (d2 > face) - (d2 > face + 5) - (d2 > d1) - (d2 > d1f)];
}

bool addEdge(FacesCacher *f, TerrainGenerator *g, int8 v, uchar d1, uchar d2,
             float3 a, float3 b) {
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

  ushort front = generateTerrain(g, vns.v);
  vns.s[d1] += mod1;
  ushort s1 = generateTerrain(g, vns.v);
  vns.s[d2] += mod2;
  ushort s1 = generateTerrain(g, vns.v);
  vns.s[d1] -= mod1;
  ushort back = generateTerrain(g, vns.v);
  vns.s[d2] -= mod2;

  if ();//TODO: PLEASE  CONTINUE FROM HERE KEK
}

