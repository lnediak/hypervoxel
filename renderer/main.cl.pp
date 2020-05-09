#ifndef NON_REP_
#define NON_REP_

uchar mergeColorChannel(uint back, int bitshift, uchar front, uchar frontAlpha,
                        uchar combinedAlpha) {
  return (front * frontAlpha + ((back >> bitshift) & 0xFF) *
                                   (255 - frontAlpha) * (back & 0xFF) /
                                   255.0f) /
         combinedAlpha;
}

void placeBehind(uint color, __global uchar *img) {
  uchar frontAlpha = *(img + 3);
  uchar combinedAlpha =
      frontAlpha + (color & 0xFF) * (255 - frontAlpha) / 255.0f;
  *img = mergeColorChannel(color, 24, *img, frontAlpha, combinedAlpha);
  img++;
  *img = mergeColorChannel(color, 16, *img, frontAlpha, combinedAlpha);
  img++;
  *img = mergeColorChannel(color, 8, *img, frontAlpha, combinedAlpha);
  *++img = combinedAlpha;
}

#endif // NON_REP_

/*
BVH format:

float minmax[NUMDIMS][2];
int objType;

The struct is different for leaf nodes (nodes where objType > 0). The rest of
the struct for a leaf node is as follows:

int obj;
This is the value of objPtr - &obj, where objPtr is a pointer to the
object.

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



Object format:
// TODO: IMPLEMENT ROTATED OBJECTS

float origin[NUMDIMS];
uint objDims[NUMDIMS];
uint hashMapSize;
uint hashMap[hashMapSize][NUMDIMS + 1];

*/

bool rayBBoxNUMDIMS(REPEAT IND NUMDIMS float posIND, float invdirIND,
                    float minIND, float maxIND, END;
                    float *dist) {
  // maximum of the lower bounds
  float tminMax = 1e-8f;
  // minimum of the upper bounds
  float tmaxMin = FLT_MAX;
  REPEAT IND NUMDIMS float t0IND = (minIND - posIND) * invdirIND;
  float t1IND = (maxIND - posIND) * invdirIND;
  tminMax = fmax(tminMax, fmin(t0IND, t1IND));
  tmaxMin = fmin(tmaxMin, fmax(t0IND, t1IND));
  END;
  *dist = tminMax;
  return tminMax < tmaxMin;
}

uint greyNUMDIMS(float f) {
  uint part = (f * 128.0) + 128.0;
  return 0xFF | (part << 8) | (part << 16) | (part << 24);
}

bool traverseObjectNUMDIMS(__global uint *object,
                           REPEAT IND NUMDIMS float posIND, float dirIND,
                           float invdirIND, END;
                           __global uchar * img, __global float *dist,
                           bool hasTerrain, REPEAT IND NUMDIMS float scaleIND,
                           END;
                           __global float *gradVecs, uint numGradVecs,
                           uint numOctaves, float persistence) {
  __global float *fobject = (__global float *)object;
  REPEAT IND NUMDIMS float originIND = *fobject++;
  END;
  object = (__global uint *)fobject;
  REPEAT IND NUMDIMS uint objDimIND = *object++;
  END;
  uint hashMapSize = *object++;
  uint maxHash = hashMapSize * (NUMDIMS + 1);

  float currDist;
  if (!rayBBoxNUMDIMS(REPEAT IND NUMDIMS posIND, invdirIND, originIND,
                      originIND + objDimIND, END;
                      &currDist)) {
    return false;
  }

  currDist += 1e-3;
  REPEAT IND NUMDIMS float currPosIND = posIND + currDist * dirIND;
  int currBlockIND = fastFloor(currPosIND - originIND);
  int dirdirIND = 2 * (invdirIND > 0) - 1;
  END;

  if (get_global_id(0) == 400) {
    //printf("%i\n", get_global_id(1));
  }
  while (true) {
    // printf("%i %i %i %i %i\n", get_global_id(0), get_global_id(1),
    // currBlock0, currBlock1, currBlock2);
    uint hashI =
        (NUMDIMS + 1) *
        hashNUMDIMS(REPEAT IND NUMDIMS currBlockIND, END; 0, hashMapSize);
    uint result;
    while (true) {
      if (!(result = object[hashI + NUMDIMS])) {
        break;
      }
      if (REPEAT IND NUMDIMS currBlockIND == object[hashI + IND] && END; true) {
        break;
      }
      hashI += NUMDIMS + 1;
      if (hashI >= maxHash) {
        hashI = 0;
      }
    }
    if (hasTerrain && !result) {
      result = getNoiseNUMDIMS(
                   REPEAT IND NUMDIMS scaleIND, originIND + currBlockIND, END;
                   gradVecs, numGradVecs, numOctaves, persistence) > 0
                   ? greyNUMDIMS((REPEAT IND NUMDIMS currPosIND - originIND - currBlockIND + END; 0.0) / NUMDIMS)
                   : 0;
    }
    placeBehind(result, img);
    if (*(img + 3) == 0xFF) {
      break;
    }
    float dist = INFINITY;
    REPEAT IND NUMDIMS dist =
        fmin(dist, invdirIND * (currBlockIND + (invdirIND > 0) -
                                (currPosIND - originIND)));
    END;
    dist = fmax(dist, 0);
    currDist += dist + 1e-3;
    int change;
    uint changeMask = 0xFFFFFFFFU;
    REPEAT IND NUMDIMS currPosIND = posIND + currDist * dirIND;
    change = (fastFloor(currPosIND - originIND) - currBlockIND) & changeMask;
    changeMask = ~changeMask | ((change > 0) - 1);
    currBlockIND += change;
    if (currBlockIND < 0 || currBlockIND >= (int)objDimIND) {
      break;
    }
    END;
  }
  return false;
}

bool traverseBVHNUMDIMS(__global uint *bvh, REPEAT IND NUMDIMS float posIND,
                        float dirIND, float invdirIND, END;
                        __global uchar * img, __global float *dist,
                        REPEAT IND NUMDIMS float scaleIND, END;
                        __global float *gradVecs, uint numGradVecs,
                        uint numOctaves, float persistence) {
  *img = *(img + 1) = *(img + 2) = *(img + 3) = 0;
  *dist = -1;
  float invdirArr[NUMDIMS];
  REPEAT IND NUMDIMS invdirArr[IND] = invdirIND;
  END;

  __global uint *stack[24];
  size_t stackInd = 0;
  stack[stackInd++] = bvh;
  float spam;
  while (stackInd) {
    __global uint *currBvh = stack[--stackInd];
    __global float *floatBVH = (__global float *)currBvh;
    REPEAT IND NUMDIMS float minIND = *floatBVH++;
    float maxIND = *floatBVH++;
    END;
    if (!rayBBoxNUMDIMS(
            REPEAT IND NUMDIMS posIND, invdirIND, minIND, maxIND, END; &spam)) {
      continue;
    }
    currBvh = (__global uint *)floatBVH;
    uint objType = *currBvh++;
    if (objType) {
      if (traverseObjectNUMDIMS(
              currBvh + *currBvh, REPEAT IND NUMDIMS posIND, dirIND, invdirIND,
              END;
              img, dist, objType - 1, REPEAT IND NUMDIMS scaleIND, END;
              gradVecs, numGradVecs, numOctaves, persistence)) {
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
                               __global float *up, __global uchar *img,
                               __global float *dist, __global float *scale,
                               __global float *gradVecs, uint numGradVecs,
                               uint numOctaves, float persistence) {
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
  img += 4 * posIndex;
  dist += posIndex;
  traverseBVHNUMDIMS(bvh, REPEAT IND NUMDIMS posIND, dirIND, invdirIND, END;
                     img, dist, REPEAT IND NUMDIMS scale[IND], END;
                     gradVecs, numGradVecs, numOctaves, persistence);
}

