typedef struct FacesEdge {

  float3 a, b;
} FacesEdge;

typedef struct FacesEntry {

  ushort colors[5];
  FacesEdge edges[5][8][6];
} FacesEntry;

typedef struct FacesCacher {

  ushort maxLen1;
  volatile __local uint *len;
  __global FacesEntry *e;
};

void initFacesCacher(FacesCacher *f, size_t maxSize, volatile __local uint *len,
                     __global void *e) {
  f->maxLen = maxSize / sizeof(FacesEntry) - 1;
  *len = 0;
  f->len = len;
  f->e = e;
}

FacesEdge *getEdge(FacesCacher *f, const TerrainCacher *d, int8 v, ushort color,
                   uchar face, uchar d1, uchar d2) {
  __global ushort2 *lp = getLoc(d, v);
  uint ind = lp->y;
  if (ind == 0xFFFF) {
    ind = atomic_add(f->len, 1);
    if (ind >= maxLen) {
      atomic_sub(f->len, 1);
      return NULL;
    }
  }
  FacesEntry *e = &f->e[ind];
  e->colors[face] = color;
  uchar d1m = d1 >= 5 ? d1 - 5 : d1;
  uchar d2m = d2 >= 5 ? d2 - 5 : d2;
  return &e->edges[face][d1 - (d1m > face)][d2 - (d2m > face) - (d2m > d1m)];
}

