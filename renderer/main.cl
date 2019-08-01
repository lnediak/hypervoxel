DEFINE RENDER(NUMDIMS);
void normalize(REPEAT(NUMDIMS)(float *vecIND, ); int) {
  float norm = 0;
  REPEAT(NUMDIMS)(norm += (*vecIND) * (*vecIND););
  norm = sqrt(norm);
  REPEAT(NUMDIMS)(*vecIND /= norm;);
}

bool rayBBoxNUMDIMS(REPEAT(NUMDIMS)(float posIND, float invdirIND, float minIND,
                                    float maxIND, );
                    int) {
  // maximum of the lower bounds
  float tminMax = 1e-8;
  // minimum of the upper bounds
  float tmaxMin = FLT_MAX;
  REPEAT(NUMDIMS)
  (float t0IND = (minIND - posIND) * invdirIND;
   float t1IND = (maxIND - posIND) * invdirIND;
   tminMax = max(tminMax, min(t0IND, t1IND));
   tmaxMin = min(tmaxMin, max(t0IND, t1IND)););
  return tminMax < tmaxMin;
}

bool traverseBVHNUMDIMS(int *bvh, REPEAT(NUMDIMS)(float posIND, float dirIND,
                                                  float invdirIND, );
                        unsigned char *color, float *dist) {
#define STACK_LEN 1024
  struct StackEntry {

    int *bvh;
    bool *out;
    bool hasParams;
    bool param1;
    bool param2;
  } * stack[STACK_LEN];
  bool returnValue;
  size_t stackInd = 0;
  stack[stackInd].bvh = bvh;
  stack[stackInd].out = &returnValue;
  stack[stackInd++].hasParams = false;
  while (stackInd) {
    StackEntry *entry = &stack[--stackInd];
    if (hasParams) {
      *entry->out = param1 || param2;
    } else {
      float *floatBVH = (float *)entry->bvh;
      REPEAT(NUMDIMS)
      (float minIND = *floatBVH++; float maxIND = *floatBVH++;);
      if (!rayBBoxNUMDIMS(REPEAT(NUMDIMS)(posIND, invdirIND, minIND, maxIND, );
                          0)) {
        *entry->out = false;
        continue;
      }
      float isOctree = *floatBVH;
      int *currBvh = (int *)++floatBVH;
      if (isOctree > 0) {
        *entry->out = traverseOctreeNUMDIMS(
            currBvh, REPEAT(NUMDIMS)(posIND, dirIND, invdirIND, ); color, dist);
        continue;
      }

      int left = *currBvh++;
      int right = *currBvh++;
      stackInd++;
      StackEntry lentry = {currBvh + left, &entry->param1, false, false,
                           false};
      StackEntry rentry = {currBvh + right, &entry->param2, false, false,
                           false};
      stack[stackInd++] = lentry;
      stack[stackInd++] = rentry;
    }
  }
  return returnValue;
}

__kernel void intersectRaysNUMDIMS(float *aosRays, );

__kernel void renderNUMDIMS(unsigned char *bvh, float *vectors,
                            unsigned char *img, float *dist) {
  size_t row = get_global_id(0);
  size_t col = get_global_id(1);
  size_t height = get_global_size(0);
  size_t width = get_global_size(1);
  float xoff = (col + 0.0) / width - 0.5;
  float yoff = (row + 0.0) / height - 0.5;
  REPEAT(NUMDIMS)
  (float posIND = vectors[IND];
   float dirIND = vectors[NUMDIMS + IND] + xoff * vectors[2 * NUMDIMS + IND] +
                  yoff * vectors[3 * NUMDIMS + IND];);
  normalize(REPEAT(NUMDIMS)(&dirIND, ); 0);
  size_t poff = row * width + col;
  img += poff * 4;
  dist += poff;
}
ENDEFINE
