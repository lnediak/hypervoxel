#ifndef NON_REP_
#define NON_REP_

uint mergeColorChannel(uint back, uint front, uint bitshift, float falpha) {
  return ((uint)(((front >> bitshift) & 0xFF) * falpha +
                 ((back >> bitshift) & 0xFF) * (1 - falpha)))
         << bitshift;
}

uint mergeColors(uint back, uint front) {
  float falpha = (front & 0xFF) / 255.0;
  return mergeColorChannel(back, front, 24, falpha) |
         mergeColorChannel(back, front, 16, falpha) |
         mergeColorChannel(back, front, 8, falpha) |
         max(front & 0xFF, back & 0xFF);
}

#endif // NON_REP_

#define DEBUG_DEF                                                              \
  bool debug =                                                                 \
      (get_global_id(0) == 0 || get_global_id(0) == get_global_size(0) - 1) && \
      (get_global_id(1) == 0 || get_global_id(1) == get_global_size(0) - 1)

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
bool traverseOctreeNUMDIMS(__global uint *octree,
                           REPEAT IND NUMDIMS float posIND, float dirIND,
                           float invdirIND, END;
                           __global uint * img, __global float *dist) {
  __global float *header = (__global float *)octree;
  REPEAT IND NUMDIMS float originIND = *header++;
  END;
  float sidelength = *header++;
  octree = (__global uint *)header;

  float tminMax = 1e-8;
  float tmaxMin = FLT_MAX;
  REPEAT IND NUMDIMS float t0IND = (originIND - posIND) * invdirIND;
  float t1IND = (originIND + sidelength - posIND) * invdirIND;
  tminMax = max(tminMax, min(t0IND, t1IND));
  tmaxMin = min(tmaxMin, max(t0IND, t1IND));
  END;
  if (tminMax >= tmaxMin) {
    return false;
  }

  float currDist = tminMax + 1e-8;
  REPEAT IND NUMDIMS float currPosIND = posIND + currDist * dirIND;
  END;

  struct StackEntry {

    bool processed;
    __global uint *node;
    size_t index;
    REPEAT IND NUMDIMS float middleIND;
    float farEndIND;
    END;
  } stack[64];
  size_t stackInd = 0;
  struct StackEntry baseEntry;
  baseEntry.processed = false;
  baseEntry.node = octree;
  REPEAT IND NUMDIMS baseEntry.middleIND = originIND + sidelength / 2;
  baseEntry.farEndIND = originIND + (invdirIND > 0) * sidelength;
  END;
  stack[stackInd++] = baseEntry;
  DEBUG_DEF;
  while (stackInd) {
    if (debug)
      printf("currDist: %f\n", currDist);
    struct StackEntry entry = stack[--stackInd];
    __global uint *node = entry.node;
    if (debug) {
      printf("Entry info: REPEAT IND NUMDIMS entry.middleIND: %f, "
             "entry.farEndIND: %f, currPosIND: %f\n END;",
             REPEAT IND NUMDIMS entry.middleIND, entry.farEndIND, currPosIND,
             END;
             0);
    }
    if (entry.processed) {
      float minStep = FLT_MAX;
      float val1, val2;
      REPEAT IND NUMDIMS val1 = (entry.middleIND - currPosIND) * invdirIND;
      val2 = (entry.farEndIND - currPosIND) * invdirIND;
      if (val1 > 1e-4) {
        minStep = min(minStep, val1);
      } else {
        if (val2 > 1e-4) {
          minStep = min(minStep, val2);
        } else {
          continue;
        }
      }
      END;
      if (debug)
        printf("Stepping from %f by %f, sidelength: %f\n", currDist, minStep,
               fabs(entry.farEnd0 - entry.middle0) * 2);
      currDist += minStep + 1e-4;
      REPEAT IND NUMDIMS currPosIND = posIND + currDist * dirIND;
      if (debug)
        printf("currPosIND: %f\n", currPosIND);
      END;
      if (REPEAT IND NUMDIMS(entry.farEndIND - currPosIND) * invdirIND < 0 ||
              END;
          false) {
        if (debug)
          printf("Exiting entry\n");
        continue;
      }
      entry.processed = false;
      stack[stackInd++] = entry;
    } else {
      if (*node++) {
        if (debug)
          printf("Reached leaf %i, sidelength: %f, currDist: %f\n", *(node - 1),
                 fabs(entry.farEnd0 - entry.middle0) * 2, currDist);
        // this is a leaf node
        *img = mergeColors(*node, *img);
        if ((*img & 0xFF) == 0xFF) {
          if (debug)
            printf("Exited function.\n");
          *dist = currDist;
          return true;
        }
      } else {
        float halfSidelength = sidelength / (2 << stackInd);
        entry.index =
            REPEAT IND NUMDIMS((currPosIND >= entry.middleIND) << IND) | END;
        0;
        if (debug)
          printf("Going down. currDist: %f\n", currDist);
        entry.processed = true;
        stack[stackInd++] = entry;
        struct StackEntry child;
        child.processed = false;
        child.node = node + node[entry.index];
        REPEAT IND NUMDIMS child.middleIND =
            1.5 * entry.middleIND - 0.5 * entry.farEndIND +
            (((entry.index & (1 << IND)) >> IND) ^ (invdirIND < 0)) *
                (entry.farEndIND - entry.middleIND);
        child.farEndIND =
            child.middleIND + (entry.farEndIND - entry.middleIND) / 2;
        END;
        stack[stackInd++] = child;
      }
    }
  }
  if (debug)
    printf("Exited function.\n");
  return false;
}

bool rayBBoxNUMDIMS(REPEAT IND NUMDIMS float posIND, float invdirIND,
                    float minIND, float maxIND, END;
                    int unused) {
  (void)unused;
  // maximum of the lower bounds
  float tminMax = 1e-8;
  // minimum of the upper bounds
  float tmaxMin = FLT_MAX;
  REPEAT IND NUMDIMS float t0IND = (minIND - posIND) * invdirIND;
  float t1IND = (maxIND - posIND) * invdirIND;
  tminMax = max(tminMax, min(t0IND, t1IND));
  tmaxMin = min(tmaxMin, max(t0IND, t1IND));
  END;
  return tminMax < tmaxMin;
}

bool traverseBVHNUMDIMS(__global uint *bvh, REPEAT IND NUMDIMS float posIND,
                        float dirIND, float invdirIND, END;
                        __global uint * img, __global float *dist) {
  float invdirArr[NUMDIMS];
  REPEAT IND NUMDIMS invdirArr[IND] = invdirIND;
  END;

  __global uint *stack[256];
  size_t stackInd = 0;
  stack[stackInd++] = bvh;
  DEBUG_DEF;
  if (debug) {
    REPEAT IND NUMDIMS printf("invdirIND: %f\n", invdirIND);
    END;
  }
  while (stackInd) {
    if (debug)
      printf("traverseBVHNUMDIMS: %lu\n", stackInd);
    __global uint *currBvh = stack[--stackInd];
    __global float *floatBVH = (__global float *)currBvh;
    REPEAT IND NUMDIMS float minIND = *floatBVH++;
    float maxIND = *floatBVH++;
    END;
    if (!rayBBoxNUMDIMS(
            REPEAT IND NUMDIMS posIND, invdirIND, minIND, maxIND, END; 0)) {
      if (debug)
        printf("quack: %lu\n", stackInd);
      continue;
    }
    currBvh = (__global uint *)floatBVH;
    if (*currBvh++) {
      if (debug)
        printf("Entering traverseOctreeNUMDIMS\n");
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
  if (debug)
    printf("Exiting traverseBVHNUMDIMS\n\n\n\n\n");
  return false;
}

__kernel void renderStdNUMDIMS(__global uint *bvh, __global float *pos,
                               __global float *forward, __global float *right,
                               __global float *up, __global uint *img,
                               __global float *dist) {
  *img = 0;
  *dist = -1;
  size_t row = get_global_id(0);
  size_t col = get_global_id(1);
  size_t height = get_global_size(0);
  size_t width = get_global_size(1);
  float xoff = 2 * (col + 0.0) / width - 1;
  float yoff = 2 * (row + 0.0) / height - 1;
  REPEAT IND NUMDIMS float posIND = pos[IND];
  float dirIND = forward[IND] + xoff * right[IND] + yoff * up[IND];
  END;
  float norm = 0;
  REPEAT IND NUMDIMS norm += dirIND * dirIND;
  END;
  if (norm < 1e-4) {
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

