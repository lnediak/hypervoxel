#ifndef NON_REP_
#define NON_REP_

uint mergeColorChannel(uint back, uint front, uint bitshift, float falpha) {
  return ((uint)(((front >> bitshift) & 0xFF) * falpha +
                 ((back >> bitshift) & 0xFF) * (1 - falpha)))
         << bitshift;
}

uint mergeColors(uint back, uint front) {
  float falpha = (front & 0xFF) / 255.0f;
  return mergeColorChannel(back, front, 24, falpha) |
         mergeColorChannel(back, front, 16, falpha) |
         mergeColorChannel(back, front, 8, falpha) |
         max(front & 0xFF, back & 0xFF);
}

#endif // NON_REP_

/*
BVH format:

float minmax[NUMDIMS][2];
int isOctree;

The struct is different for leaf nodes (nodes where isOctree). The rest of the
struct for a leaf node is as follows:

int octree;
This is the value of octreePtr - &octree, where octreePtr is a pointer to the
octree.

The rest of the struct for a non-leaf node is as follows:

int left;
This is the value of leftBvh - &left, where leftBvh is a pointer to the struct
for the left node.

int right;
This is the value of rightBvh - &right, where rightBvh is a pointer to the
struct for the right node.

int splitInd;
This is the axis in which left and right have the greatest distance between
each other. Along this axis, left must be on the left of right.



Octree format:
// TODO: IMPLEMENT ROTATED OCTREES

Header:

float origin[NUMDIMS];
float sidelength;
This is the sidelength of the root node.

The root node is immediately after the header.

Node:

int isLeaf;
If isLeaf, then the following immediately follows:

int color;

Otherwise, the following immediately follows:

int children[2 ** NUMDIMS];
The index of the node with minimum coordinates is 0, while changing the 2 **
IND bit would change only the coordinate on axis IND.

Each value in children in equal to childPtr - children, where childPtr is a
pointer to the corresponding node.

*/
typedef struct StackEntryNUMDIMS {

  bool indexed;
  __global uint *node;
  size_t index;
  REPEAT IND NUMDIMS float middleIND;
  float farEndIND;
  END;
} StackEntryNUMDIMS;

void makeChildNUMDIMS(const StackEntryNUMDIMS *parent, size_t index,
                      StackEntryNUMDIMS *child) {
  child->indexed = false;
  child->node = parent->node + 1 + parent->node[index + 1];
  REPEAT IND NUMDIMS child->middleIND =
      1.5f * parent->middleIND - 0.5f * parent->farEndIND +
      (((index >> IND) & 1) ^ (parent->farEndIND < parent->middleIND)) *
          (parent->farEndIND - parent->middleIND);
  child->farEndIND =
      child->middleIND + (parent->farEndIND - parent->middleIND) / 2;
  END;
}

bool traverseOctreeNUMDIMS(__global uint *octree,
                           REPEAT IND NUMDIMS float posIND, float dirIND,
                           float invdirIND, END;
                           __global uint * img, __global float *dist) {
  __global float *header = (__global float *)octree;
  REPEAT IND NUMDIMS float originIND = *header++;
  END;
  float sidelength = *header++;
  octree = (__global uint *)header;

  float tminMax = 1e-8f;
  float tmaxMin = FLT_MAX;
  REPEAT IND NUMDIMS float t0IND = (originIND - posIND) * invdirIND;
  float t1IND = (originIND + sidelength - posIND) * invdirIND;
  tminMax = fmax(tminMax, fmin(t0IND, t1IND));
  tmaxMin = fmin(tmaxMin, fmax(t0IND, t1IND));
  END;
  if (tminMax >= tmaxMin) {
    return false;
  }

  float currDist = tminMax + 1e-3f;
  REPEAT IND NUMDIMS float currPosIND = posIND + currDist * dirIND;
  END;

  StackEntryNUMDIMS stack[12];
  size_t stackInd = 0;
  StackEntryNUMDIMS baseEntry;
  baseEntry.indexed = false;
  baseEntry.node = octree;
  REPEAT IND NUMDIMS baseEntry.middleIND = originIND + sidelength / 2;
  baseEntry.farEndIND = originIND + (invdirIND > 0) * sidelength;
  END;
  stack[stackInd++] = baseEntry;
  bool debug = true;
  __global float *distp = dist;
  size_t i = 0;
  while (stackInd && i < 100) {
    i++;
    StackEntryNUMDIMS entry = stack[--stackInd];
    /*
    if (debug) {
      printf("currDist: %f, entry.indexed: %i, entry.index: %lu\n", currDist,
             (int)entry.indexed, entry.index);
      REPEAT IND NUMDIMS printf(
          "currPosIND: %f, entry.middleIND: %f, entry.farEndIND: %f\n",
          currPosIND, entry.middleIND, entry.farEndIND);
      END;
      if (*entry.node) {
        printf("Leaf node\n");
      } else {
        printf("\n");
      }
    }
    */
    /*
    *distp++ = currDist;
    REPEAT IND NUMDIMS *distp++ = currPosIND;
    *distp++ = invdirIND;
    *distp++ = entry.middleIND;
    *distp++ = entry.farEndIND;
    END;
    *distp++ = *entry.node;
    *distp++ = stackInd;
    */
    if (*entry.node) {
      // this is a leaf node
      *img = mergeColors(*(entry.node + 1), *img);
      if ((*img & 0xFF) == 0xFF) {
        *dist = currDist;
        return true;
      }
      float minStep = FLT_MAX;
      REPEAT IND NUMDIMS minStep =
          fmin(minStep, (entry.farEndIND - currPosIND) * invdirIND);
      END;
      if (minStep <= -1e-3f) {
        continue;
      }
      currDist += minStep + 1e-3f;
      REPEAT IND NUMDIMS currPosIND = posIND + currDist * dirIND;
      END;
    } else {
      size_t newIndex = REPEAT IND NUMDIMS(
                            ((currPosIND > entry.middleIND) ||
                             (currPosIND == entry.middleIND && (invdirIND > 0)))
                            << IND) |
                        END;
      0;
      if (entry.indexed) {
        size_t diffIndex = newIndex ^ entry.index;
        if (diffIndex) {
          entry.index = newIndex;
          if (REPEAT IND NUMDIMS((entry.farEndIND - currPosIND) * invdirIND >
                                 0) &&
                  END;
              true) {
            stack[stackInd++] = entry;
          }
          size_t tmpIndex = newIndex;
          StackEntryNUMDIMS child;
          REPEAT IND NUMDIMS if ((1 << IND) & diffIndex) {
            makeChildNUMDIMS(&entry, tmpIndex, &child);
            stack[stackInd++] = child;
            tmpIndex ^= (1 << IND);
          }
          END;
          continue;
        }
      }
      entry.index = newIndex;
      entry.indexed = true;
      if (REPEAT IND NUMDIMS((entry.farEndIND - currPosIND) * invdirIND >
                             0) &&
              END;
          true) {
        stack[stackInd++] = entry;
      }
      StackEntryNUMDIMS child;
      makeChildNUMDIMS(&entry, entry.index, &child);
      stack[stackInd++] = child;
    }
  }
  return false;
}

bool rayBBoxNUMDIMS(REPEAT IND NUMDIMS float posIND, float invdirIND,
                    float minIND, float maxIND, END;
                    int unused) {
  (void)unused;
  // maximum of the lower bounds
  float tminMax = 1e-8f;
  // minimum of the upper bounds
  float tmaxMin = FLT_MAX;
  REPEAT IND NUMDIMS float t0IND = (minIND - posIND) * invdirIND;
  float t1IND = (maxIND - posIND) * invdirIND;
  tminMax = fmax(tminMax, fmin(t0IND, t1IND));
  tmaxMin = fmin(tmaxMin, fmax(t0IND, t1IND));
  END;
  return tminMax < tmaxMin;
}

bool traverseBVHNUMDIMS(__global uint *bvh, REPEAT IND NUMDIMS float posIND,
                        float dirIND, float invdirIND, END;
                        __global uint * img, __global float *dist) {
  *img = 0;
  *dist = -1;
  float invdirArr[NUMDIMS];
  REPEAT IND NUMDIMS invdirArr[IND] = invdirIND;
  END;

  __global uint *stack[24];
  size_t stackInd = 0;
  stack[stackInd++] = bvh;
  while (stackInd) {
    __global uint *currBvh = stack[--stackInd];
    __global float *floatBVH = (__global float *)currBvh;
    REPEAT IND NUMDIMS float minIND = *floatBVH++;
    float maxIND = *floatBVH++;
    END;
    if (!rayBBoxNUMDIMS(
            REPEAT IND NUMDIMS posIND, invdirIND, minIND, maxIND, END; 0)) {
      continue;
    }
    currBvh = (__global uint *)floatBVH;
    if (*currBvh++) {
      if (traverseOctreeNUMDIMS(currBvh + *currBvh, REPEAT IND NUMDIMS posIND,
                                dirIND, invdirIND, END;
                                img, dist)) {
        return true;
      }
      continue;
    }

    __global uint *lbvh = currBvh + *currBvh;
    currBvh++;
    __global uint *rbvh = currBvh + *currBvh;
    currBvh++;
    int splitInd = *currBvh++;
    if (invdirArr[splitInd] < 0) {
      __global uint *tmp;
      tmp = lbvh;
      lbvh = rbvh;
      rbvh = tmp;
    }
    stack[stackInd++] = rbvh;
    stack[stackInd++] = lbvh;
  }
  return false;
}

__kernel void renderStdNUMDIMS(__global uint *bvh, __global float *pos,
                               __global float *forward, __global float *right,
                               __global float *up, __global uint *img,
                               __global float *dist) {
  size_t row = get_global_id(0);
  size_t col = get_global_id(1);
  size_t height = get_global_size(0);
  size_t width = get_global_size(1);
  /*size_t row = *img;
  size_t col = *(img + 1);
  size_t height = *(img + 2);
  size_t width = *(img + 3);*/
  float xoff = 2 * (col + 0.0f) / width - 1;
  float yoff = 2 * (row + 0.0f) / height - 1;
  REPEAT IND NUMDIMS float posIND = pos[IND];
  float dirIND = forward[IND] + xoff * right[IND] + yoff * up[IND];
  END;
  float norm = 0;
  REPEAT IND NUMDIMS norm += dirIND * dirIND;
  END;
  if (norm < 1e-3f) {
    return;
  }
  norm = sqrt(norm);
  REPEAT IND NUMDIMS dirIND /= norm;
  float invdirIND = 1 / dirIND;
  END;
  size_t posIndex = row * width + col;
  img += posIndex;
  dist += posIndex;
  traverseBVHNUMDIMS(bvh, REPEAT IND NUMDIMS posIND, dirIND, invdirIND, END;
                     img, dist);
}

