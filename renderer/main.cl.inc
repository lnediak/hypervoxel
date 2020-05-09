// AUTO-GENERATED FILE: DO NOT MODIFY
R"OPENCLPP(
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

float minmax[3][2];
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

float origin[3];
uint objDims[3];
uint hashMapSize;
uint hashMap[hashMapSize][3 + 1];

*/

bool rayBBox3( float pos0, float invdir0,
                    float min0, float max0,  float pos1, float invdir1,
                    float min1, float max1,  float pos2, float invdir2,
                    float min2, float max2, 
                    float *dist) {
  // maximum of the lower bounds
  float tminMax = 1e-8f;
  // minimum of the upper bounds
  float tmaxMin = FLT_MAX;
   float t00 = (min0 - pos0) * invdir0;
  float t10 = (max0 - pos0) * invdir0;
  tminMax = fmax(tminMax, fmin(t00, t10));
  tmaxMin = fmin(tmaxMin, fmax(t00, t10));
   float t01 = (min1 - pos1) * invdir1;
  float t11 = (max1 - pos1) * invdir1;
  tminMax = fmax(tminMax, fmin(t01, t11));
  tmaxMin = fmin(tmaxMin, fmax(t01, t11));
   float t02 = (min2 - pos2) * invdir2;
  float t12 = (max2 - pos2) * invdir2;
  tminMax = fmax(tminMax, fmin(t02, t12));
  tmaxMin = fmin(tmaxMin, fmax(t02, t12));
  
  *dist = tminMax;
  return tminMax < tmaxMin;
}

uint grey3(float f) {
  uint part = (f * 128.0) + 128.0;
  return 0xFF | (part << 8) | (part << 16) | (part << 24);
}

bool traverseObject3(__global uint *object,
                            float pos0, float dir0,
                           float invdir0,  float pos1, float dir1,
                           float invdir1,  float pos2, float dir2,
                           float invdir2, 
                           __global uchar * img, __global float *dist,
                           bool hasTerrain,  float scale0,
                            float scale1,
                            float scale2,
                           
                           __global float *gradVecs, uint numGradVecs,
                           uint numOctaves, float persistence) {
  __global float *fobject = (__global float *)object;
   float origin0 = *fobject++;
   float origin1 = *fobject++;
   float origin2 = *fobject++;
  
  object = (__global uint *)fobject;
   uint objDim0 = *object++;
   uint objDim1 = *object++;
   uint objDim2 = *object++;
  
  uint hashMapSize = *object++;
  uint maxHash = hashMapSize * (3 + 1);

  float currDist;
  if (!rayBBox3( pos0, invdir0, origin0,
                      origin0 + objDim0,  pos1, invdir1, origin1,
                      origin1 + objDim1,  pos2, invdir2, origin2,
                      origin2 + objDim2, 
                      &currDist)) {
    return false;
  }

  currDist += 1e-3;
   float currPos0 = pos0 + currDist * dir0;
  int currBlock0 = fastFloor(currPos0 - origin0);
  int dirdir0 = 2 * (invdir0 > 0) - 1;
   float currPos1 = pos1 + currDist * dir1;
  int currBlock1 = fastFloor(currPos1 - origin1);
  int dirdir1 = 2 * (invdir1 > 0) - 1;
   float currPos2 = pos2 + currDist * dir2;
  int currBlock2 = fastFloor(currPos2 - origin2);
  int dirdir2 = 2 * (invdir2 > 0) - 1;
  

  if (get_global_id(0) == 400) {
    //printf("%i\n", get_global_id(1));
  }
  while (true) {
    // printf("%i %i %i %i %i\n", get_global_id(0), get_global_id(1),
    // currBlock0, currBlock1, currBlock2);
    uint hashI =
        (3 + 1) *
        hash3( currBlock0,  currBlock1,  currBlock2,  0, hashMapSize);
    uint result;
    while (true) {
      if (!(result = object[hashI + 3])) {
        break;
      }
      if ( currBlock0 == object[hashI + 0] &&  currBlock1 == object[hashI + 1] &&  currBlock2 == object[hashI + 2] &&  true) {
        break;
      }
      hashI += 3 + 1;
      if (hashI >= maxHash) {
        hashI = 0;
      }
    }
    if (hasTerrain && !result) {
      result = getNoise3(
                    scale0, origin0 + currBlock0,  scale1, origin1 + currBlock1,  scale2, origin2 + currBlock2, 
                   gradVecs, numGradVecs, numOctaves, persistence) > 0
                   ? grey3(( currPos0 - origin0 - currBlock0 +  currPos1 - origin1 - currBlock1 +  currPos2 - origin2 - currBlock2 +  0.0) / 3)
                   : 0;
    }
    placeBehind(result, img);
    if (*(img + 3) == 0xFF) {
      break;
    }
    float dist = INFINITY;
     dist =
        fmin(dist, invdir0 * (currBlock0 + (invdir0 > 0) -
                                (currPos0 - origin0)));
     dist =
        fmin(dist, invdir1 * (currBlock1 + (invdir1 > 0) -
                                (currPos1 - origin1)));
     dist =
        fmin(dist, invdir2 * (currBlock2 + (invdir2 > 0) -
                                (currPos2 - origin2)));
    
    dist = fmax(dist, 0);
    currDist += dist + 1e-3;
    int change;
    uint changeMask = 0xFFFFFFFFU;
     currPos0 = pos0 + currDist * dir0;
    change = (fastFloor(currPos0 - origin0) - currBlock0) & changeMask;
    changeMask = ~changeMask | ((change > 0) - 1);
    currBlock0 += change;
    if (currBlock0 < 0 || currBlock0 >= (int)objDim0) {
      break;
    }
     currPos1 = pos1 + currDist * dir1;
    change = (fastFloor(currPos1 - origin1) - currBlock1) & changeMask;
    changeMask = ~changeMask | ((change > 0) - 1);
    currBlock1 += change;
    if (currBlock1 < 0 || currBlock1 >= (int)objDim1) {
      break;
    }
     currPos2 = pos2 + currDist * dir2;
    change = (fastFloor(currPos2 - origin2) - currBlock2) & changeMask;
    changeMask = ~changeMask | ((change > 0) - 1);
    currBlock2 += change;
    if (currBlock2 < 0 || currBlock2 >= (int)objDim2) {
      break;
    }
    
  }
  return false;
}

bool traverseBVH3(__global uint *bvh,  float pos0,
                        float dir0, float invdir0,  float pos1,
                        float dir1, float invdir1,  float pos2,
                        float dir2, float invdir2, 
                        __global uchar * img, __global float *dist,
                         float scale0,  float scale1,  float scale2, 
                        __global float *gradVecs, uint numGradVecs,
                        uint numOctaves, float persistence) {
  *img = *(img + 1) = *(img + 2) = *(img + 3) = 0;
  *dist = -1;
  float invdirArr[3];
   invdirArr[0] = invdir0;
   invdirArr[1] = invdir1;
   invdirArr[2] = invdir2;
  

  __global uint *stack[24];
  size_t stackInd = 0;
  stack[stackInd++] = bvh;
  float spam;
  while (stackInd) {
    __global uint *currBvh = stack[--stackInd];
    __global float *floatBVH = (__global float *)currBvh;
     float min0 = *floatBVH++;
    float max0 = *floatBVH++;
     float min1 = *floatBVH++;
    float max1 = *floatBVH++;
     float min2 = *floatBVH++;
    float max2 = *floatBVH++;
    
    if (!rayBBox3(
             pos0, invdir0, min0, max0,  pos1, invdir1, min1, max1,  pos2, invdir2, min2, max2,  &spam)) {
      continue;
    }
    currBvh = (__global uint *)floatBVH;
    uint objType = *currBvh++;
    if (objType) {
      if (traverseObject3(
              currBvh + *currBvh,  pos0, dir0, invdir0,
               pos1, dir1, invdir1,
               pos2, dir2, invdir2,
              
              img, dist, objType - 1,  scale0,  scale1,  scale2, 
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

__kernel void renderStd3(__global uint *bvh, __global float *pos,
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
   float pos0 = pos[0];
  float dir0 = forward[0] + xoff * right[0] + yoff * up[0];
   float pos1 = pos[1];
  float dir1 = forward[1] + xoff * right[1] + yoff * up[1];
   float pos2 = pos[2];
  float dir2 = forward[2] + xoff * right[2] + yoff * up[2];
  
  float norm = 0;
   norm += dir0 * dir0;
   norm += dir1 * dir1;
   norm += dir2 * dir2;
  
  if (norm < 1e-3f) {
    return;
  }
  norm = sqrt(norm);
   dir0 /= norm;
  float invdir0 = 1 / dir0;
   dir1 /= norm;
  float invdir1 = 1 / dir1;
   dir2 /= norm;
  float invdir2 = 1 / dir2;
  
  size_t posIndex = row * width + col;
  img += 4 * posIndex;
  dist += posIndex;
  traverseBVH3(bvh,  pos0, dir0, invdir0,  pos1, dir1, invdir1,  pos2, dir2, invdir2, 
                     img, dist,  scale[0],  scale[1],  scale[2], 
                     gradVecs, numGradVecs, numOctaves, persistence);
}

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

float minmax[4][2];
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

float origin[4];
uint objDims[4];
uint hashMapSize;
uint hashMap[hashMapSize][4 + 1];

*/

bool rayBBox4( float pos0, float invdir0,
                    float min0, float max0,  float pos1, float invdir1,
                    float min1, float max1,  float pos2, float invdir2,
                    float min2, float max2,  float pos3, float invdir3,
                    float min3, float max3, 
                    float *dist) {
  // maximum of the lower bounds
  float tminMax = 1e-8f;
  // minimum of the upper bounds
  float tmaxMin = FLT_MAX;
   float t00 = (min0 - pos0) * invdir0;
  float t10 = (max0 - pos0) * invdir0;
  tminMax = fmax(tminMax, fmin(t00, t10));
  tmaxMin = fmin(tmaxMin, fmax(t00, t10));
   float t01 = (min1 - pos1) * invdir1;
  float t11 = (max1 - pos1) * invdir1;
  tminMax = fmax(tminMax, fmin(t01, t11));
  tmaxMin = fmin(tmaxMin, fmax(t01, t11));
   float t02 = (min2 - pos2) * invdir2;
  float t12 = (max2 - pos2) * invdir2;
  tminMax = fmax(tminMax, fmin(t02, t12));
  tmaxMin = fmin(tmaxMin, fmax(t02, t12));
   float t03 = (min3 - pos3) * invdir3;
  float t13 = (max3 - pos3) * invdir3;
  tminMax = fmax(tminMax, fmin(t03, t13));
  tmaxMin = fmin(tmaxMin, fmax(t03, t13));
  
  *dist = tminMax;
  return tminMax < tmaxMin;
}

uint grey4(float f) {
  uint part = (f * 128.0) + 128.0;
  return 0xFF | (part << 8) | (part << 16) | (part << 24);
}

bool traverseObject4(__global uint *object,
                            float pos0, float dir0,
                           float invdir0,  float pos1, float dir1,
                           float invdir1,  float pos2, float dir2,
                           float invdir2,  float pos3, float dir3,
                           float invdir3, 
                           __global uchar * img, __global float *dist,
                           bool hasTerrain,  float scale0,
                            float scale1,
                            float scale2,
                            float scale3,
                           
                           __global float *gradVecs, uint numGradVecs,
                           uint numOctaves, float persistence) {
  __global float *fobject = (__global float *)object;
   float origin0 = *fobject++;
   float origin1 = *fobject++;
   float origin2 = *fobject++;
   float origin3 = *fobject++;
  
  object = (__global uint *)fobject;
   uint objDim0 = *object++;
   uint objDim1 = *object++;
   uint objDim2 = *object++;
   uint objDim3 = *object++;
  
  uint hashMapSize = *object++;
  uint maxHash = hashMapSize * (4 + 1);

  float currDist;
  if (!rayBBox4( pos0, invdir0, origin0,
                      origin0 + objDim0,  pos1, invdir1, origin1,
                      origin1 + objDim1,  pos2, invdir2, origin2,
                      origin2 + objDim2,  pos3, invdir3, origin3,
                      origin3 + objDim3, 
                      &currDist)) {
    return false;
  }

  currDist += 1e-3;
   float currPos0 = pos0 + currDist * dir0;
  int currBlock0 = fastFloor(currPos0 - origin0);
  int dirdir0 = 2 * (invdir0 > 0) - 1;
   float currPos1 = pos1 + currDist * dir1;
  int currBlock1 = fastFloor(currPos1 - origin1);
  int dirdir1 = 2 * (invdir1 > 0) - 1;
   float currPos2 = pos2 + currDist * dir2;
  int currBlock2 = fastFloor(currPos2 - origin2);
  int dirdir2 = 2 * (invdir2 > 0) - 1;
   float currPos3 = pos3 + currDist * dir3;
  int currBlock3 = fastFloor(currPos3 - origin3);
  int dirdir3 = 2 * (invdir3 > 0) - 1;
  

  if (get_global_id(0) == 400) {
    //printf("%i\n", get_global_id(1));
  }
  while (true) {
    // printf("%i %i %i %i %i\n", get_global_id(0), get_global_id(1),
    // currBlock0, currBlock1, currBlock2);
    uint hashI =
        (4 + 1) *
        hash4( currBlock0,  currBlock1,  currBlock2,  currBlock3,  0, hashMapSize);
    uint result;
    while (true) {
      if (!(result = object[hashI + 4])) {
        break;
      }
      if ( currBlock0 == object[hashI + 0] &&  currBlock1 == object[hashI + 1] &&  currBlock2 == object[hashI + 2] &&  currBlock3 == object[hashI + 3] &&  true) {
        break;
      }
      hashI += 4 + 1;
      if (hashI >= maxHash) {
        hashI = 0;
      }
    }
    if (hasTerrain && !result) {
      result = getNoise4(
                    scale0, origin0 + currBlock0,  scale1, origin1 + currBlock1,  scale2, origin2 + currBlock2,  scale3, origin3 + currBlock3, 
                   gradVecs, numGradVecs, numOctaves, persistence) > 0
                   ? grey4(( currPos0 - origin0 - currBlock0 +  currPos1 - origin1 - currBlock1 +  currPos2 - origin2 - currBlock2 +  currPos3 - origin3 - currBlock3 +  0.0) / 4)
                   : 0;
    }
    placeBehind(result, img);
    if (*(img + 3) == 0xFF) {
      break;
    }
    float dist = INFINITY;
     dist =
        fmin(dist, invdir0 * (currBlock0 + (invdir0 > 0) -
                                (currPos0 - origin0)));
     dist =
        fmin(dist, invdir1 * (currBlock1 + (invdir1 > 0) -
                                (currPos1 - origin1)));
     dist =
        fmin(dist, invdir2 * (currBlock2 + (invdir2 > 0) -
                                (currPos2 - origin2)));
     dist =
        fmin(dist, invdir3 * (currBlock3 + (invdir3 > 0) -
                                (currPos3 - origin3)));
    
    dist = fmax(dist, 0);
    currDist += dist + 1e-3;
    int change;
    uint changeMask = 0xFFFFFFFFU;
     currPos0 = pos0 + currDist * dir0;
    change = (fastFloor(currPos0 - origin0) - currBlock0) & changeMask;
    changeMask = ~changeMask | ((change > 0) - 1);
    currBlock0 += change;
    if (currBlock0 < 0 || currBlock0 >= (int)objDim0) {
      break;
    }
     currPos1 = pos1 + currDist * dir1;
    change = (fastFloor(currPos1 - origin1) - currBlock1) & changeMask;
    changeMask = ~changeMask | ((change > 0) - 1);
    currBlock1 += change;
    if (currBlock1 < 0 || currBlock1 >= (int)objDim1) {
      break;
    }
     currPos2 = pos2 + currDist * dir2;
    change = (fastFloor(currPos2 - origin2) - currBlock2) & changeMask;
    changeMask = ~changeMask | ((change > 0) - 1);
    currBlock2 += change;
    if (currBlock2 < 0 || currBlock2 >= (int)objDim2) {
      break;
    }
     currPos3 = pos3 + currDist * dir3;
    change = (fastFloor(currPos3 - origin3) - currBlock3) & changeMask;
    changeMask = ~changeMask | ((change > 0) - 1);
    currBlock3 += change;
    if (currBlock3 < 0 || currBlock3 >= (int)objDim3) {
      break;
    }
    
  }
  return false;
}

bool traverseBVH4(__global uint *bvh,  float pos0,
                        float dir0, float invdir0,  float pos1,
                        float dir1, float invdir1,  float pos2,
                        float dir2, float invdir2,  float pos3,
                        float dir3, float invdir3, 
                        __global uchar * img, __global float *dist,
                         float scale0,  float scale1,  float scale2,  float scale3, 
                        __global float *gradVecs, uint numGradVecs,
                        uint numOctaves, float persistence) {
  *img = *(img + 1) = *(img + 2) = *(img + 3) = 0;
  *dist = -1;
  float invdirArr[4];
   invdirArr[0] = invdir0;
   invdirArr[1] = invdir1;
   invdirArr[2] = invdir2;
   invdirArr[3] = invdir3;
  

  __global uint *stack[24];
  size_t stackInd = 0;
  stack[stackInd++] = bvh;
  float spam;
  while (stackInd) {
    __global uint *currBvh = stack[--stackInd];
    __global float *floatBVH = (__global float *)currBvh;
     float min0 = *floatBVH++;
    float max0 = *floatBVH++;
     float min1 = *floatBVH++;
    float max1 = *floatBVH++;
     float min2 = *floatBVH++;
    float max2 = *floatBVH++;
     float min3 = *floatBVH++;
    float max3 = *floatBVH++;
    
    if (!rayBBox4(
             pos0, invdir0, min0, max0,  pos1, invdir1, min1, max1,  pos2, invdir2, min2, max2,  pos3, invdir3, min3, max3,  &spam)) {
      continue;
    }
    currBvh = (__global uint *)floatBVH;
    uint objType = *currBvh++;
    if (objType) {
      if (traverseObject4(
              currBvh + *currBvh,  pos0, dir0, invdir0,
               pos1, dir1, invdir1,
               pos2, dir2, invdir2,
               pos3, dir3, invdir3,
              
              img, dist, objType - 1,  scale0,  scale1,  scale2,  scale3, 
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

__kernel void renderStd4(__global uint *bvh, __global float *pos,
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
   float pos0 = pos[0];
  float dir0 = forward[0] + xoff * right[0] + yoff * up[0];
   float pos1 = pos[1];
  float dir1 = forward[1] + xoff * right[1] + yoff * up[1];
   float pos2 = pos[2];
  float dir2 = forward[2] + xoff * right[2] + yoff * up[2];
   float pos3 = pos[3];
  float dir3 = forward[3] + xoff * right[3] + yoff * up[3];
  
  float norm = 0;
   norm += dir0 * dir0;
   norm += dir1 * dir1;
   norm += dir2 * dir2;
   norm += dir3 * dir3;
  
  if (norm < 1e-3f) {
    return;
  }
  norm = sqrt(norm);
   dir0 /= norm;
  float invdir0 = 1 / dir0;
   dir1 /= norm;
  float invdir1 = 1 / dir1;
   dir2 /= norm;
  float invdir2 = 1 / dir2;
   dir3 /= norm;
  float invdir3 = 1 / dir3;
  
  size_t posIndex = row * width + col;
  img += 4 * posIndex;
  dist += posIndex;
  traverseBVH4(bvh,  pos0, dir0, invdir0,  pos1, dir1, invdir1,  pos2, dir2, invdir2,  pos3, dir3, invdir3, 
                     img, dist,  scale[0],  scale[1],  scale[2],  scale[3], 
                     gradVecs, numGradVecs, numOctaves, persistence);
}

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

float minmax[5][2];
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

float origin[5];
uint objDims[5];
uint hashMapSize;
uint hashMap[hashMapSize][5 + 1];

*/

bool rayBBox5( float pos0, float invdir0,
                    float min0, float max0,  float pos1, float invdir1,
                    float min1, float max1,  float pos2, float invdir2,
                    float min2, float max2,  float pos3, float invdir3,
                    float min3, float max3,  float pos4, float invdir4,
                    float min4, float max4, 
                    float *dist) {
  // maximum of the lower bounds
  float tminMax = 1e-8f;
  // minimum of the upper bounds
  float tmaxMin = FLT_MAX;
   float t00 = (min0 - pos0) * invdir0;
  float t10 = (max0 - pos0) * invdir0;
  tminMax = fmax(tminMax, fmin(t00, t10));
  tmaxMin = fmin(tmaxMin, fmax(t00, t10));
   float t01 = (min1 - pos1) * invdir1;
  float t11 = (max1 - pos1) * invdir1;
  tminMax = fmax(tminMax, fmin(t01, t11));
  tmaxMin = fmin(tmaxMin, fmax(t01, t11));
   float t02 = (min2 - pos2) * invdir2;
  float t12 = (max2 - pos2) * invdir2;
  tminMax = fmax(tminMax, fmin(t02, t12));
  tmaxMin = fmin(tmaxMin, fmax(t02, t12));
   float t03 = (min3 - pos3) * invdir3;
  float t13 = (max3 - pos3) * invdir3;
  tminMax = fmax(tminMax, fmin(t03, t13));
  tmaxMin = fmin(tmaxMin, fmax(t03, t13));
   float t04 = (min4 - pos4) * invdir4;
  float t14 = (max4 - pos4) * invdir4;
  tminMax = fmax(tminMax, fmin(t04, t14));
  tmaxMin = fmin(tmaxMin, fmax(t04, t14));
  
  *dist = tminMax;
  return tminMax < tmaxMin;
}

uint grey5(float f) {
  uint part = (f * 128.0) + 128.0;
  return 0xFF | (part << 8) | (part << 16) | (part << 24);
}

bool traverseObject5(__global uint *object,
                            float pos0, float dir0,
                           float invdir0,  float pos1, float dir1,
                           float invdir1,  float pos2, float dir2,
                           float invdir2,  float pos3, float dir3,
                           float invdir3,  float pos4, float dir4,
                           float invdir4, 
                           __global uchar * img, __global float *dist,
                           bool hasTerrain,  float scale0,
                            float scale1,
                            float scale2,
                            float scale3,
                            float scale4,
                           
                           __global float *gradVecs, uint numGradVecs,
                           uint numOctaves, float persistence) {
  __global float *fobject = (__global float *)object;
   float origin0 = *fobject++;
   float origin1 = *fobject++;
   float origin2 = *fobject++;
   float origin3 = *fobject++;
   float origin4 = *fobject++;
  
  object = (__global uint *)fobject;
   uint objDim0 = *object++;
   uint objDim1 = *object++;
   uint objDim2 = *object++;
   uint objDim3 = *object++;
   uint objDim4 = *object++;
  
  uint hashMapSize = *object++;
  uint maxHash = hashMapSize * (5 + 1);

  float currDist;
  if (!rayBBox5( pos0, invdir0, origin0,
                      origin0 + objDim0,  pos1, invdir1, origin1,
                      origin1 + objDim1,  pos2, invdir2, origin2,
                      origin2 + objDim2,  pos3, invdir3, origin3,
                      origin3 + objDim3,  pos4, invdir4, origin4,
                      origin4 + objDim4, 
                      &currDist)) {
    return false;
  }

  currDist += 1e-3;
   float currPos0 = pos0 + currDist * dir0;
  int currBlock0 = fastFloor(currPos0 - origin0);
  int dirdir0 = 2 * (invdir0 > 0) - 1;
   float currPos1 = pos1 + currDist * dir1;
  int currBlock1 = fastFloor(currPos1 - origin1);
  int dirdir1 = 2 * (invdir1 > 0) - 1;
   float currPos2 = pos2 + currDist * dir2;
  int currBlock2 = fastFloor(currPos2 - origin2);
  int dirdir2 = 2 * (invdir2 > 0) - 1;
   float currPos3 = pos3 + currDist * dir3;
  int currBlock3 = fastFloor(currPos3 - origin3);
  int dirdir3 = 2 * (invdir3 > 0) - 1;
   float currPos4 = pos4 + currDist * dir4;
  int currBlock4 = fastFloor(currPos4 - origin4);
  int dirdir4 = 2 * (invdir4 > 0) - 1;
  

  if (get_global_id(0) == 400) {
    //printf("%i\n", get_global_id(1));
  }
  while (true) {
    // printf("%i %i %i %i %i\n", get_global_id(0), get_global_id(1),
    // currBlock0, currBlock1, currBlock2);
    uint hashI =
        (5 + 1) *
        hash5( currBlock0,  currBlock1,  currBlock2,  currBlock3,  currBlock4,  0, hashMapSize);
    uint result;
    while (true) {
      if (!(result = object[hashI + 5])) {
        break;
      }
      if ( currBlock0 == object[hashI + 0] &&  currBlock1 == object[hashI + 1] &&  currBlock2 == object[hashI + 2] &&  currBlock3 == object[hashI + 3] &&  currBlock4 == object[hashI + 4] &&  true) {
        break;
      }
      hashI += 5 + 1;
      if (hashI >= maxHash) {
        hashI = 0;
      }
    }
    if (hasTerrain && !result) {
      result = getNoise5(
                    scale0, origin0 + currBlock0,  scale1, origin1 + currBlock1,  scale2, origin2 + currBlock2,  scale3, origin3 + currBlock3,  scale4, origin4 + currBlock4, 
                   gradVecs, numGradVecs, numOctaves, persistence) > 0
                   ? grey5(( currPos0 - origin0 - currBlock0 +  currPos1 - origin1 - currBlock1 +  currPos2 - origin2 - currBlock2 +  currPos3 - origin3 - currBlock3 +  currPos4 - origin4 - currBlock4 +  0.0) / 5)
                   : 0;
    }
    placeBehind(result, img);
    if (*(img + 3) == 0xFF) {
      break;
    }
    float dist = INFINITY;
     dist =
        fmin(dist, invdir0 * (currBlock0 + (invdir0 > 0) -
                                (currPos0 - origin0)));
     dist =
        fmin(dist, invdir1 * (currBlock1 + (invdir1 > 0) -
                                (currPos1 - origin1)));
     dist =
        fmin(dist, invdir2 * (currBlock2 + (invdir2 > 0) -
                                (currPos2 - origin2)));
     dist =
        fmin(dist, invdir3 * (currBlock3 + (invdir3 > 0) -
                                (currPos3 - origin3)));
     dist =
        fmin(dist, invdir4 * (currBlock4 + (invdir4 > 0) -
                                (currPos4 - origin4)));
    
    dist = fmax(dist, 0);
    currDist += dist + 1e-3;
    int change;
    uint changeMask = 0xFFFFFFFFU;
     currPos0 = pos0 + currDist * dir0;
    change = (fastFloor(currPos0 - origin0) - currBlock0) & changeMask;
    changeMask = ~changeMask | ((change > 0) - 1);
    currBlock0 += change;
    if (currBlock0 < 0 || currBlock0 >= (int)objDim0) {
      break;
    }
     currPos1 = pos1 + currDist * dir1;
    change = (fastFloor(currPos1 - origin1) - currBlock1) & changeMask;
    changeMask = ~changeMask | ((change > 0) - 1);
    currBlock1 += change;
    if (currBlock1 < 0 || currBlock1 >= (int)objDim1) {
      break;
    }
     currPos2 = pos2 + currDist * dir2;
    change = (fastFloor(currPos2 - origin2) - currBlock2) & changeMask;
    changeMask = ~changeMask | ((change > 0) - 1);
    currBlock2 += change;
    if (currBlock2 < 0 || currBlock2 >= (int)objDim2) {
      break;
    }
     currPos3 = pos3 + currDist * dir3;
    change = (fastFloor(currPos3 - origin3) - currBlock3) & changeMask;
    changeMask = ~changeMask | ((change > 0) - 1);
    currBlock3 += change;
    if (currBlock3 < 0 || currBlock3 >= (int)objDim3) {
      break;
    }
     currPos4 = pos4 + currDist * dir4;
    change = (fastFloor(currPos4 - origin4) - currBlock4) & changeMask;
    changeMask = ~changeMask | ((change > 0) - 1);
    currBlock4 += change;
    if (currBlock4 < 0 || currBlock4 >= (int)objDim4) {
      break;
    }
    
  }
  return false;
}

bool traverseBVH5(__global uint *bvh,  float pos0,
                        float dir0, float invdir0,  float pos1,
                        float dir1, float invdir1,  float pos2,
                        float dir2, float invdir2,  float pos3,
                        float dir3, float invdir3,  float pos4,
                        float dir4, float invdir4, 
                        __global uchar * img, __global float *dist,
                         float scale0,  float scale1,  float scale2,  float scale3,  float scale4, 
                        __global float *gradVecs, uint numGradVecs,
                        uint numOctaves, float persistence) {
  *img = *(img + 1) = *(img + 2) = *(img + 3) = 0;
  *dist = -1;
  float invdirArr[5];
   invdirArr[0] = invdir0;
   invdirArr[1] = invdir1;
   invdirArr[2] = invdir2;
   invdirArr[3] = invdir3;
   invdirArr[4] = invdir4;
  

  __global uint *stack[24];
  size_t stackInd = 0;
  stack[stackInd++] = bvh;
  float spam;
  while (stackInd) {
    __global uint *currBvh = stack[--stackInd];
    __global float *floatBVH = (__global float *)currBvh;
     float min0 = *floatBVH++;
    float max0 = *floatBVH++;
     float min1 = *floatBVH++;
    float max1 = *floatBVH++;
     float min2 = *floatBVH++;
    float max2 = *floatBVH++;
     float min3 = *floatBVH++;
    float max3 = *floatBVH++;
     float min4 = *floatBVH++;
    float max4 = *floatBVH++;
    
    if (!rayBBox5(
             pos0, invdir0, min0, max0,  pos1, invdir1, min1, max1,  pos2, invdir2, min2, max2,  pos3, invdir3, min3, max3,  pos4, invdir4, min4, max4,  &spam)) {
      continue;
    }
    currBvh = (__global uint *)floatBVH;
    uint objType = *currBvh++;
    if (objType) {
      if (traverseObject5(
              currBvh + *currBvh,  pos0, dir0, invdir0,
               pos1, dir1, invdir1,
               pos2, dir2, invdir2,
               pos3, dir3, invdir3,
               pos4, dir4, invdir4,
              
              img, dist, objType - 1,  scale0,  scale1,  scale2,  scale3,  scale4, 
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

__kernel void renderStd5(__global uint *bvh, __global float *pos,
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
   float pos0 = pos[0];
  float dir0 = forward[0] + xoff * right[0] + yoff * up[0];
   float pos1 = pos[1];
  float dir1 = forward[1] + xoff * right[1] + yoff * up[1];
   float pos2 = pos[2];
  float dir2 = forward[2] + xoff * right[2] + yoff * up[2];
   float pos3 = pos[3];
  float dir3 = forward[3] + xoff * right[3] + yoff * up[3];
   float pos4 = pos[4];
  float dir4 = forward[4] + xoff * right[4] + yoff * up[4];
  
  float norm = 0;
   norm += dir0 * dir0;
   norm += dir1 * dir1;
   norm += dir2 * dir2;
   norm += dir3 * dir3;
   norm += dir4 * dir4;
  
  if (norm < 1e-3f) {
    return;
  }
  norm = sqrt(norm);
   dir0 /= norm;
  float invdir0 = 1 / dir0;
   dir1 /= norm;
  float invdir1 = 1 / dir1;
   dir2 /= norm;
  float invdir2 = 1 / dir2;
   dir3 /= norm;
  float invdir3 = 1 / dir3;
   dir4 /= norm;
  float invdir4 = 1 / dir4;
  
  size_t posIndex = row * width + col;
  img += 4 * posIndex;
  dist += posIndex;
  traverseBVH5(bvh,  pos0, dir0, invdir0,  pos1, dir1, invdir1,  pos2, dir2, invdir2,  pos3, dir3, invdir3,  pos4, dir4, invdir4, 
                     img, dist,  scale[0],  scale[1],  scale[2],  scale[3],  scale[4], 
                     gradVecs, numGradVecs, numOctaves, persistence);
}

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

float minmax[6][2];
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

float origin[6];
uint objDims[6];
uint hashMapSize;
uint hashMap[hashMapSize][6 + 1];

*/

bool rayBBox6( float pos0, float invdir0,
                    float min0, float max0,  float pos1, float invdir1,
                    float min1, float max1,  float pos2, float invdir2,
                    float min2, float max2,  float pos3, float invdir3,
                    float min3, float max3,  float pos4, float invdir4,
                    float min4, float max4,  float pos5, float invdir5,
                    float min5, float max5, 
                    float *dist) {
  // maximum of the lower bounds
  float tminMax = 1e-8f;
  // minimum of the upper bounds
  float tmaxMin = FLT_MAX;
   float t00 = (min0 - pos0) * invdir0;
  float t10 = (max0 - pos0) * invdir0;
  tminMax = fmax(tminMax, fmin(t00, t10));
  tmaxMin = fmin(tmaxMin, fmax(t00, t10));
   float t01 = (min1 - pos1) * invdir1;
  float t11 = (max1 - pos1) * invdir1;
  tminMax = fmax(tminMax, fmin(t01, t11));
  tmaxMin = fmin(tmaxMin, fmax(t01, t11));
   float t02 = (min2 - pos2) * invdir2;
  float t12 = (max2 - pos2) * invdir2;
  tminMax = fmax(tminMax, fmin(t02, t12));
  tmaxMin = fmin(tmaxMin, fmax(t02, t12));
   float t03 = (min3 - pos3) * invdir3;
  float t13 = (max3 - pos3) * invdir3;
  tminMax = fmax(tminMax, fmin(t03, t13));
  tmaxMin = fmin(tmaxMin, fmax(t03, t13));
   float t04 = (min4 - pos4) * invdir4;
  float t14 = (max4 - pos4) * invdir4;
  tminMax = fmax(tminMax, fmin(t04, t14));
  tmaxMin = fmin(tmaxMin, fmax(t04, t14));
   float t05 = (min5 - pos5) * invdir5;
  float t15 = (max5 - pos5) * invdir5;
  tminMax = fmax(tminMax, fmin(t05, t15));
  tmaxMin = fmin(tmaxMin, fmax(t05, t15));
  
  *dist = tminMax;
  return tminMax < tmaxMin;
}

uint grey6(float f) {
  uint part = (f * 128.0) + 128.0;
  return 0xFF | (part << 8) | (part << 16) | (part << 24);
}

bool traverseObject6(__global uint *object,
                            float pos0, float dir0,
                           float invdir0,  float pos1, float dir1,
                           float invdir1,  float pos2, float dir2,
                           float invdir2,  float pos3, float dir3,
                           float invdir3,  float pos4, float dir4,
                           float invdir4,  float pos5, float dir5,
                           float invdir5, 
                           __global uchar * img, __global float *dist,
                           bool hasTerrain,  float scale0,
                            float scale1,
                            float scale2,
                            float scale3,
                            float scale4,
                            float scale5,
                           
                           __global float *gradVecs, uint numGradVecs,
                           uint numOctaves, float persistence) {
  __global float *fobject = (__global float *)object;
   float origin0 = *fobject++;
   float origin1 = *fobject++;
   float origin2 = *fobject++;
   float origin3 = *fobject++;
   float origin4 = *fobject++;
   float origin5 = *fobject++;
  
  object = (__global uint *)fobject;
   uint objDim0 = *object++;
   uint objDim1 = *object++;
   uint objDim2 = *object++;
   uint objDim3 = *object++;
   uint objDim4 = *object++;
   uint objDim5 = *object++;
  
  uint hashMapSize = *object++;
  uint maxHash = hashMapSize * (6 + 1);

  float currDist;
  if (!rayBBox6( pos0, invdir0, origin0,
                      origin0 + objDim0,  pos1, invdir1, origin1,
                      origin1 + objDim1,  pos2, invdir2, origin2,
                      origin2 + objDim2,  pos3, invdir3, origin3,
                      origin3 + objDim3,  pos4, invdir4, origin4,
                      origin4 + objDim4,  pos5, invdir5, origin5,
                      origin5 + objDim5, 
                      &currDist)) {
    return false;
  }

  currDist += 1e-3;
   float currPos0 = pos0 + currDist * dir0;
  int currBlock0 = fastFloor(currPos0 - origin0);
  int dirdir0 = 2 * (invdir0 > 0) - 1;
   float currPos1 = pos1 + currDist * dir1;
  int currBlock1 = fastFloor(currPos1 - origin1);
  int dirdir1 = 2 * (invdir1 > 0) - 1;
   float currPos2 = pos2 + currDist * dir2;
  int currBlock2 = fastFloor(currPos2 - origin2);
  int dirdir2 = 2 * (invdir2 > 0) - 1;
   float currPos3 = pos3 + currDist * dir3;
  int currBlock3 = fastFloor(currPos3 - origin3);
  int dirdir3 = 2 * (invdir3 > 0) - 1;
   float currPos4 = pos4 + currDist * dir4;
  int currBlock4 = fastFloor(currPos4 - origin4);
  int dirdir4 = 2 * (invdir4 > 0) - 1;
   float currPos5 = pos5 + currDist * dir5;
  int currBlock5 = fastFloor(currPos5 - origin5);
  int dirdir5 = 2 * (invdir5 > 0) - 1;
  

  if (get_global_id(0) == 400) {
    //printf("%i\n", get_global_id(1));
  }
  while (true) {
    // printf("%i %i %i %i %i\n", get_global_id(0), get_global_id(1),
    // currBlock0, currBlock1, currBlock2);
    uint hashI =
        (6 + 1) *
        hash6( currBlock0,  currBlock1,  currBlock2,  currBlock3,  currBlock4,  currBlock5,  0, hashMapSize);
    uint result;
    while (true) {
      if (!(result = object[hashI + 6])) {
        break;
      }
      if ( currBlock0 == object[hashI + 0] &&  currBlock1 == object[hashI + 1] &&  currBlock2 == object[hashI + 2] &&  currBlock3 == object[hashI + 3] &&  currBlock4 == object[hashI + 4] &&  currBlock5 == object[hashI + 5] &&  true) {
        break;
      }
      hashI += 6 + 1;
      if (hashI >= maxHash) {
        hashI = 0;
      }
    }
    if (hasTerrain && !result) {
      result = getNoise6(
                    scale0, origin0 + currBlock0,  scale1, origin1 + currBlock1,  scale2, origin2 + currBlock2,  scale3, origin3 + currBlock3,  scale4, origin4 + currBlock4,  scale5, origin5 + currBlock5, 
                   gradVecs, numGradVecs, numOctaves, persistence) > 0
                   ? grey6(( currPos0 - origin0 - currBlock0 +  currPos1 - origin1 - currBlock1 +  currPos2 - origin2 - currBlock2 +  currPos3 - origin3 - currBlock3 +  currPos4 - origin4 - currBlock4 +  currPos5 - origin5 - currBlock5 +  0.0) / 6)
                   : 0;
    }
    placeBehind(result, img);
    if (*(img + 3) == 0xFF) {
      break;
    }
    float dist = INFINITY;
     dist =
        fmin(dist, invdir0 * (currBlock0 + (invdir0 > 0) -
                                (currPos0 - origin0)));
     dist =
        fmin(dist, invdir1 * (currBlock1 + (invdir1 > 0) -
                                (currPos1 - origin1)));
     dist =
        fmin(dist, invdir2 * (currBlock2 + (invdir2 > 0) -
                                (currPos2 - origin2)));
     dist =
        fmin(dist, invdir3 * (currBlock3 + (invdir3 > 0) -
                                (currPos3 - origin3)));
     dist =
        fmin(dist, invdir4 * (currBlock4 + (invdir4 > 0) -
                                (currPos4 - origin4)));
     dist =
        fmin(dist, invdir5 * (currBlock5 + (invdir5 > 0) -
                                (currPos5 - origin5)));
    
    dist = fmax(dist, 0);
    currDist += dist + 1e-3;
    int change;
    uint changeMask = 0xFFFFFFFFU;
     currPos0 = pos0 + currDist * dir0;
    change = (fastFloor(currPos0 - origin0) - currBlock0) & changeMask;
    changeMask = ~changeMask | ((change > 0) - 1);
    currBlock0 += change;
    if (currBlock0 < 0 || currBlock0 >= (int)objDim0) {
      break;
    }
     currPos1 = pos1 + currDist * dir1;
    change = (fastFloor(currPos1 - origin1) - currBlock1) & changeMask;
    changeMask = ~changeMask | ((change > 0) - 1);
    currBlock1 += change;
    if (currBlock1 < 0 || currBlock1 >= (int)objDim1) {
      break;
    }
     currPos2 = pos2 + currDist * dir2;
    change = (fastFloor(currPos2 - origin2) - currBlock2) & changeMask;
    changeMask = ~changeMask | ((change > 0) - 1);
    currBlock2 += change;
    if (currBlock2 < 0 || currBlock2 >= (int)objDim2) {
      break;
    }
     currPos3 = pos3 + currDist * dir3;
    change = (fastFloor(currPos3 - origin3) - currBlock3) & changeMask;
    changeMask = ~changeMask | ((change > 0) - 1);
    currBlock3 += change;
    if (currBlock3 < 0 || currBlock3 >= (int)objDim3) {
      break;
    }
     currPos4 = pos4 + currDist * dir4;
    change = (fastFloor(currPos4 - origin4) - currBlock4) & changeMask;
    changeMask = ~changeMask | ((change > 0) - 1);
    currBlock4 += change;
    if (currBlock4 < 0 || currBlock4 >= (int)objDim4) {
      break;
    }
     currPos5 = pos5 + currDist * dir5;
    change = (fastFloor(currPos5 - origin5) - currBlock5) & changeMask;
    changeMask = ~changeMask | ((change > 0) - 1);
    currBlock5 += change;
    if (currBlock5 < 0 || currBlock5 >= (int)objDim5) {
      break;
    }
    
  }
  return false;
}

bool traverseBVH6(__global uint *bvh,  float pos0,
                        float dir0, float invdir0,  float pos1,
                        float dir1, float invdir1,  float pos2,
                        float dir2, float invdir2,  float pos3,
                        float dir3, float invdir3,  float pos4,
                        float dir4, float invdir4,  float pos5,
                        float dir5, float invdir5, 
                        __global uchar * img, __global float *dist,
                         float scale0,  float scale1,  float scale2,  float scale3,  float scale4,  float scale5, 
                        __global float *gradVecs, uint numGradVecs,
                        uint numOctaves, float persistence) {
  *img = *(img + 1) = *(img + 2) = *(img + 3) = 0;
  *dist = -1;
  float invdirArr[6];
   invdirArr[0] = invdir0;
   invdirArr[1] = invdir1;
   invdirArr[2] = invdir2;
   invdirArr[3] = invdir3;
   invdirArr[4] = invdir4;
   invdirArr[5] = invdir5;
  

  __global uint *stack[24];
  size_t stackInd = 0;
  stack[stackInd++] = bvh;
  float spam;
  while (stackInd) {
    __global uint *currBvh = stack[--stackInd];
    __global float *floatBVH = (__global float *)currBvh;
     float min0 = *floatBVH++;
    float max0 = *floatBVH++;
     float min1 = *floatBVH++;
    float max1 = *floatBVH++;
     float min2 = *floatBVH++;
    float max2 = *floatBVH++;
     float min3 = *floatBVH++;
    float max3 = *floatBVH++;
     float min4 = *floatBVH++;
    float max4 = *floatBVH++;
     float min5 = *floatBVH++;
    float max5 = *floatBVH++;
    
    if (!rayBBox6(
             pos0, invdir0, min0, max0,  pos1, invdir1, min1, max1,  pos2, invdir2, min2, max2,  pos3, invdir3, min3, max3,  pos4, invdir4, min4, max4,  pos5, invdir5, min5, max5,  &spam)) {
      continue;
    }
    currBvh = (__global uint *)floatBVH;
    uint objType = *currBvh++;
    if (objType) {
      if (traverseObject6(
              currBvh + *currBvh,  pos0, dir0, invdir0,
               pos1, dir1, invdir1,
               pos2, dir2, invdir2,
               pos3, dir3, invdir3,
               pos4, dir4, invdir4,
               pos5, dir5, invdir5,
              
              img, dist, objType - 1,  scale0,  scale1,  scale2,  scale3,  scale4,  scale5, 
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

__kernel void renderStd6(__global uint *bvh, __global float *pos,
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
   float pos0 = pos[0];
  float dir0 = forward[0] + xoff * right[0] + yoff * up[0];
   float pos1 = pos[1];
  float dir1 = forward[1] + xoff * right[1] + yoff * up[1];
   float pos2 = pos[2];
  float dir2 = forward[2] + xoff * right[2] + yoff * up[2];
   float pos3 = pos[3];
  float dir3 = forward[3] + xoff * right[3] + yoff * up[3];
   float pos4 = pos[4];
  float dir4 = forward[4] + xoff * right[4] + yoff * up[4];
   float pos5 = pos[5];
  float dir5 = forward[5] + xoff * right[5] + yoff * up[5];
  
  float norm = 0;
   norm += dir0 * dir0;
   norm += dir1 * dir1;
   norm += dir2 * dir2;
   norm += dir3 * dir3;
   norm += dir4 * dir4;
   norm += dir5 * dir5;
  
  if (norm < 1e-3f) {
    return;
  }
  norm = sqrt(norm);
   dir0 /= norm;
  float invdir0 = 1 / dir0;
   dir1 /= norm;
  float invdir1 = 1 / dir1;
   dir2 /= norm;
  float invdir2 = 1 / dir2;
   dir3 /= norm;
  float invdir3 = 1 / dir3;
   dir4 /= norm;
  float invdir4 = 1 / dir4;
   dir5 /= norm;
  float invdir5 = 1 / dir5;
  
  size_t posIndex = row * width + col;
  img += 4 * posIndex;
  dist += posIndex;
  traverseBVH6(bvh,  pos0, dir0, invdir0,  pos1, dir1, invdir1,  pos2, dir2, invdir2,  pos3, dir3, invdir3,  pos4, dir4, invdir4,  pos5, dir5, invdir5, 
                     img, dist,  scale[0],  scale[1],  scale[2],  scale[3],  scale[4],  scale[5], 
                     gradVecs, numGradVecs, numOctaves, persistence);
}

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

float minmax[7][2];
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

float origin[7];
uint objDims[7];
uint hashMapSize;
uint hashMap[hashMapSize][7 + 1];

*/

bool rayBBox7( float pos0, float invdir0,
                    float min0, float max0,  float pos1, float invdir1,
                    float min1, float max1,  float pos2, float invdir2,
                    float min2, float max2,  float pos3, float invdir3,
                    float min3, float max3,  float pos4, float invdir4,
                    float min4, float max4,  float pos5, float invdir5,
                    float min5, float max5,  float pos6, float invdir6,
                    float min6, float max6, 
                    float *dist) {
  // maximum of the lower bounds
  float tminMax = 1e-8f;
  // minimum of the upper bounds
  float tmaxMin = FLT_MAX;
   float t00 = (min0 - pos0) * invdir0;
  float t10 = (max0 - pos0) * invdir0;
  tminMax = fmax(tminMax, fmin(t00, t10));
  tmaxMin = fmin(tmaxMin, fmax(t00, t10));
   float t01 = (min1 - pos1) * invdir1;
  float t11 = (max1 - pos1) * invdir1;
  tminMax = fmax(tminMax, fmin(t01, t11));
  tmaxMin = fmin(tmaxMin, fmax(t01, t11));
   float t02 = (min2 - pos2) * invdir2;
  float t12 = (max2 - pos2) * invdir2;
  tminMax = fmax(tminMax, fmin(t02, t12));
  tmaxMin = fmin(tmaxMin, fmax(t02, t12));
   float t03 = (min3 - pos3) * invdir3;
  float t13 = (max3 - pos3) * invdir3;
  tminMax = fmax(tminMax, fmin(t03, t13));
  tmaxMin = fmin(tmaxMin, fmax(t03, t13));
   float t04 = (min4 - pos4) * invdir4;
  float t14 = (max4 - pos4) * invdir4;
  tminMax = fmax(tminMax, fmin(t04, t14));
  tmaxMin = fmin(tmaxMin, fmax(t04, t14));
   float t05 = (min5 - pos5) * invdir5;
  float t15 = (max5 - pos5) * invdir5;
  tminMax = fmax(tminMax, fmin(t05, t15));
  tmaxMin = fmin(tmaxMin, fmax(t05, t15));
   float t06 = (min6 - pos6) * invdir6;
  float t16 = (max6 - pos6) * invdir6;
  tminMax = fmax(tminMax, fmin(t06, t16));
  tmaxMin = fmin(tmaxMin, fmax(t06, t16));
  
  *dist = tminMax;
  return tminMax < tmaxMin;
}

uint grey7(float f) {
  uint part = (f * 128.0) + 128.0;
  return 0xFF | (part << 8) | (part << 16) | (part << 24);
}

bool traverseObject7(__global uint *object,
                            float pos0, float dir0,
                           float invdir0,  float pos1, float dir1,
                           float invdir1,  float pos2, float dir2,
                           float invdir2,  float pos3, float dir3,
                           float invdir3,  float pos4, float dir4,
                           float invdir4,  float pos5, float dir5,
                           float invdir5,  float pos6, float dir6,
                           float invdir6, 
                           __global uchar * img, __global float *dist,
                           bool hasTerrain,  float scale0,
                            float scale1,
                            float scale2,
                            float scale3,
                            float scale4,
                            float scale5,
                            float scale6,
                           
                           __global float *gradVecs, uint numGradVecs,
                           uint numOctaves, float persistence) {
  __global float *fobject = (__global float *)object;
   float origin0 = *fobject++;
   float origin1 = *fobject++;
   float origin2 = *fobject++;
   float origin3 = *fobject++;
   float origin4 = *fobject++;
   float origin5 = *fobject++;
   float origin6 = *fobject++;
  
  object = (__global uint *)fobject;
   uint objDim0 = *object++;
   uint objDim1 = *object++;
   uint objDim2 = *object++;
   uint objDim3 = *object++;
   uint objDim4 = *object++;
   uint objDim5 = *object++;
   uint objDim6 = *object++;
  
  uint hashMapSize = *object++;
  uint maxHash = hashMapSize * (7 + 1);

  float currDist;
  if (!rayBBox7( pos0, invdir0, origin0,
                      origin0 + objDim0,  pos1, invdir1, origin1,
                      origin1 + objDim1,  pos2, invdir2, origin2,
                      origin2 + objDim2,  pos3, invdir3, origin3,
                      origin3 + objDim3,  pos4, invdir4, origin4,
                      origin4 + objDim4,  pos5, invdir5, origin5,
                      origin5 + objDim5,  pos6, invdir6, origin6,
                      origin6 + objDim6, 
                      &currDist)) {
    return false;
  }

  currDist += 1e-3;
   float currPos0 = pos0 + currDist * dir0;
  int currBlock0 = fastFloor(currPos0 - origin0);
  int dirdir0 = 2 * (invdir0 > 0) - 1;
   float currPos1 = pos1 + currDist * dir1;
  int currBlock1 = fastFloor(currPos1 - origin1);
  int dirdir1 = 2 * (invdir1 > 0) - 1;
   float currPos2 = pos2 + currDist * dir2;
  int currBlock2 = fastFloor(currPos2 - origin2);
  int dirdir2 = 2 * (invdir2 > 0) - 1;
   float currPos3 = pos3 + currDist * dir3;
  int currBlock3 = fastFloor(currPos3 - origin3);
  int dirdir3 = 2 * (invdir3 > 0) - 1;
   float currPos4 = pos4 + currDist * dir4;
  int currBlock4 = fastFloor(currPos4 - origin4);
  int dirdir4 = 2 * (invdir4 > 0) - 1;
   float currPos5 = pos5 + currDist * dir5;
  int currBlock5 = fastFloor(currPos5 - origin5);
  int dirdir5 = 2 * (invdir5 > 0) - 1;
   float currPos6 = pos6 + currDist * dir6;
  int currBlock6 = fastFloor(currPos6 - origin6);
  int dirdir6 = 2 * (invdir6 > 0) - 1;
  

  if (get_global_id(0) == 400) {
    //printf("%i\n", get_global_id(1));
  }
  while (true) {
    // printf("%i %i %i %i %i\n", get_global_id(0), get_global_id(1),
    // currBlock0, currBlock1, currBlock2);
    uint hashI =
        (7 + 1) *
        hash7( currBlock0,  currBlock1,  currBlock2,  currBlock3,  currBlock4,  currBlock5,  currBlock6,  0, hashMapSize);
    uint result;
    while (true) {
      if (!(result = object[hashI + 7])) {
        break;
      }
      if ( currBlock0 == object[hashI + 0] &&  currBlock1 == object[hashI + 1] &&  currBlock2 == object[hashI + 2] &&  currBlock3 == object[hashI + 3] &&  currBlock4 == object[hashI + 4] &&  currBlock5 == object[hashI + 5] &&  currBlock6 == object[hashI + 6] &&  true) {
        break;
      }
      hashI += 7 + 1;
      if (hashI >= maxHash) {
        hashI = 0;
      }
    }
    if (hasTerrain && !result) {
      result = getNoise7(
                    scale0, origin0 + currBlock0,  scale1, origin1 + currBlock1,  scale2, origin2 + currBlock2,  scale3, origin3 + currBlock3,  scale4, origin4 + currBlock4,  scale5, origin5 + currBlock5,  scale6, origin6 + currBlock6, 
                   gradVecs, numGradVecs, numOctaves, persistence) > 0
                   ? grey7(( currPos0 - origin0 - currBlock0 +  currPos1 - origin1 - currBlock1 +  currPos2 - origin2 - currBlock2 +  currPos3 - origin3 - currBlock3 +  currPos4 - origin4 - currBlock4 +  currPos5 - origin5 - currBlock5 +  currPos6 - origin6 - currBlock6 +  0.0) / 7)
                   : 0;
    }
    placeBehind(result, img);
    if (*(img + 3) == 0xFF) {
      break;
    }
    float dist = INFINITY;
     dist =
        fmin(dist, invdir0 * (currBlock0 + (invdir0 > 0) -
                                (currPos0 - origin0)));
     dist =
        fmin(dist, invdir1 * (currBlock1 + (invdir1 > 0) -
                                (currPos1 - origin1)));
     dist =
        fmin(dist, invdir2 * (currBlock2 + (invdir2 > 0) -
                                (currPos2 - origin2)));
     dist =
        fmin(dist, invdir3 * (currBlock3 + (invdir3 > 0) -
                                (currPos3 - origin3)));
     dist =
        fmin(dist, invdir4 * (currBlock4 + (invdir4 > 0) -
                                (currPos4 - origin4)));
     dist =
        fmin(dist, invdir5 * (currBlock5 + (invdir5 > 0) -
                                (currPos5 - origin5)));
     dist =
        fmin(dist, invdir6 * (currBlock6 + (invdir6 > 0) -
                                (currPos6 - origin6)));
    
    dist = fmax(dist, 0);
    currDist += dist + 1e-3;
    int change;
    uint changeMask = 0xFFFFFFFFU;
     currPos0 = pos0 + currDist * dir0;
    change = (fastFloor(currPos0 - origin0) - currBlock0) & changeMask;
    changeMask = ~changeMask | ((change > 0) - 1);
    currBlock0 += change;
    if (currBlock0 < 0 || currBlock0 >= (int)objDim0) {
      break;
    }
     currPos1 = pos1 + currDist * dir1;
    change = (fastFloor(currPos1 - origin1) - currBlock1) & changeMask;
    changeMask = ~changeMask | ((change > 0) - 1);
    currBlock1 += change;
    if (currBlock1 < 0 || currBlock1 >= (int)objDim1) {
      break;
    }
     currPos2 = pos2 + currDist * dir2;
    change = (fastFloor(currPos2 - origin2) - currBlock2) & changeMask;
    changeMask = ~changeMask | ((change > 0) - 1);
    currBlock2 += change;
    if (currBlock2 < 0 || currBlock2 >= (int)objDim2) {
      break;
    }
     currPos3 = pos3 + currDist * dir3;
    change = (fastFloor(currPos3 - origin3) - currBlock3) & changeMask;
    changeMask = ~changeMask | ((change > 0) - 1);
    currBlock3 += change;
    if (currBlock3 < 0 || currBlock3 >= (int)objDim3) {
      break;
    }
     currPos4 = pos4 + currDist * dir4;
    change = (fastFloor(currPos4 - origin4) - currBlock4) & changeMask;
    changeMask = ~changeMask | ((change > 0) - 1);
    currBlock4 += change;
    if (currBlock4 < 0 || currBlock4 >= (int)objDim4) {
      break;
    }
     currPos5 = pos5 + currDist * dir5;
    change = (fastFloor(currPos5 - origin5) - currBlock5) & changeMask;
    changeMask = ~changeMask | ((change > 0) - 1);
    currBlock5 += change;
    if (currBlock5 < 0 || currBlock5 >= (int)objDim5) {
      break;
    }
     currPos6 = pos6 + currDist * dir6;
    change = (fastFloor(currPos6 - origin6) - currBlock6) & changeMask;
    changeMask = ~changeMask | ((change > 0) - 1);
    currBlock6 += change;
    if (currBlock6 < 0 || currBlock6 >= (int)objDim6) {
      break;
    }
    
  }
  return false;
}

bool traverseBVH7(__global uint *bvh,  float pos0,
                        float dir0, float invdir0,  float pos1,
                        float dir1, float invdir1,  float pos2,
                        float dir2, float invdir2,  float pos3,
                        float dir3, float invdir3,  float pos4,
                        float dir4, float invdir4,  float pos5,
                        float dir5, float invdir5,  float pos6,
                        float dir6, float invdir6, 
                        __global uchar * img, __global float *dist,
                         float scale0,  float scale1,  float scale2,  float scale3,  float scale4,  float scale5,  float scale6, 
                        __global float *gradVecs, uint numGradVecs,
                        uint numOctaves, float persistence) {
  *img = *(img + 1) = *(img + 2) = *(img + 3) = 0;
  *dist = -1;
  float invdirArr[7];
   invdirArr[0] = invdir0;
   invdirArr[1] = invdir1;
   invdirArr[2] = invdir2;
   invdirArr[3] = invdir3;
   invdirArr[4] = invdir4;
   invdirArr[5] = invdir5;
   invdirArr[6] = invdir6;
  

  __global uint *stack[24];
  size_t stackInd = 0;
  stack[stackInd++] = bvh;
  float spam;
  while (stackInd) {
    __global uint *currBvh = stack[--stackInd];
    __global float *floatBVH = (__global float *)currBvh;
     float min0 = *floatBVH++;
    float max0 = *floatBVH++;
     float min1 = *floatBVH++;
    float max1 = *floatBVH++;
     float min2 = *floatBVH++;
    float max2 = *floatBVH++;
     float min3 = *floatBVH++;
    float max3 = *floatBVH++;
     float min4 = *floatBVH++;
    float max4 = *floatBVH++;
     float min5 = *floatBVH++;
    float max5 = *floatBVH++;
     float min6 = *floatBVH++;
    float max6 = *floatBVH++;
    
    if (!rayBBox7(
             pos0, invdir0, min0, max0,  pos1, invdir1, min1, max1,  pos2, invdir2, min2, max2,  pos3, invdir3, min3, max3,  pos4, invdir4, min4, max4,  pos5, invdir5, min5, max5,  pos6, invdir6, min6, max6,  &spam)) {
      continue;
    }
    currBvh = (__global uint *)floatBVH;
    uint objType = *currBvh++;
    if (objType) {
      if (traverseObject7(
              currBvh + *currBvh,  pos0, dir0, invdir0,
               pos1, dir1, invdir1,
               pos2, dir2, invdir2,
               pos3, dir3, invdir3,
               pos4, dir4, invdir4,
               pos5, dir5, invdir5,
               pos6, dir6, invdir6,
              
              img, dist, objType - 1,  scale0,  scale1,  scale2,  scale3,  scale4,  scale5,  scale6, 
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

__kernel void renderStd7(__global uint *bvh, __global float *pos,
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
   float pos0 = pos[0];
  float dir0 = forward[0] + xoff * right[0] + yoff * up[0];
   float pos1 = pos[1];
  float dir1 = forward[1] + xoff * right[1] + yoff * up[1];
   float pos2 = pos[2];
  float dir2 = forward[2] + xoff * right[2] + yoff * up[2];
   float pos3 = pos[3];
  float dir3 = forward[3] + xoff * right[3] + yoff * up[3];
   float pos4 = pos[4];
  float dir4 = forward[4] + xoff * right[4] + yoff * up[4];
   float pos5 = pos[5];
  float dir5 = forward[5] + xoff * right[5] + yoff * up[5];
   float pos6 = pos[6];
  float dir6 = forward[6] + xoff * right[6] + yoff * up[6];
  
  float norm = 0;
   norm += dir0 * dir0;
   norm += dir1 * dir1;
   norm += dir2 * dir2;
   norm += dir3 * dir3;
   norm += dir4 * dir4;
   norm += dir5 * dir5;
   norm += dir6 * dir6;
  
  if (norm < 1e-3f) {
    return;
  }
  norm = sqrt(norm);
   dir0 /= norm;
  float invdir0 = 1 / dir0;
   dir1 /= norm;
  float invdir1 = 1 / dir1;
   dir2 /= norm;
  float invdir2 = 1 / dir2;
   dir3 /= norm;
  float invdir3 = 1 / dir3;
   dir4 /= norm;
  float invdir4 = 1 / dir4;
   dir5 /= norm;
  float invdir5 = 1 / dir5;
   dir6 /= norm;
  float invdir6 = 1 / dir6;
  
  size_t posIndex = row * width + col;
  img += 4 * posIndex;
  dist += posIndex;
  traverseBVH7(bvh,  pos0, dir0, invdir0,  pos1, dir1, invdir1,  pos2, dir2, invdir2,  pos3, dir3, invdir3,  pos4, dir4, invdir4,  pos5, dir5, invdir5,  pos6, dir6, invdir6, 
                     img, dist,  scale[0],  scale[1],  scale[2],  scale[3],  scale[4],  scale[5],  scale[6], 
                     gradVecs, numGradVecs, numOctaves, persistence);
}

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

float minmax[8][2];
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

float origin[8];
uint objDims[8];
uint hashMapSize;
uint hashMap[hashMapSize][8 + 1];

*/

bool rayBBox8( float pos0, float invdir0,
                    float min0, float max0,  float pos1, float invdir1,
                    float min1, float max1,  float pos2, float invdir2,
                    float min2, float max2,  float pos3, float invdir3,
                    float min3, float max3,  float pos4, float invdir4,
                    float min4, float max4,  float pos5, float invdir5,
                    float min5, float max5,  float pos6, float invdir6,
                    float min6, float max6,  float pos7, float invdir7,
                    float min7, float max7, 
                    float *dist) {
  // maximum of the lower bounds
  float tminMax = 1e-8f;
  // minimum of the upper bounds
  float tmaxMin = FLT_MAX;
   float t00 = (min0 - pos0) * invdir0;
  float t10 = (max0 - pos0) * invdir0;
  tminMax = fmax(tminMax, fmin(t00, t10));
  tmaxMin = fmin(tmaxMin, fmax(t00, t10));
   float t01 = (min1 - pos1) * invdir1;
  float t11 = (max1 - pos1) * invdir1;
  tminMax = fmax(tminMax, fmin(t01, t11));
  tmaxMin = fmin(tmaxMin, fmax(t01, t11));
   float t02 = (min2 - pos2) * invdir2;
  float t12 = (max2 - pos2) * invdir2;
  tminMax = fmax(tminMax, fmin(t02, t12));
  tmaxMin = fmin(tmaxMin, fmax(t02, t12));
   float t03 = (min3 - pos3) * invdir3;
  float t13 = (max3 - pos3) * invdir3;
  tminMax = fmax(tminMax, fmin(t03, t13));
  tmaxMin = fmin(tmaxMin, fmax(t03, t13));
   float t04 = (min4 - pos4) * invdir4;
  float t14 = (max4 - pos4) * invdir4;
  tminMax = fmax(tminMax, fmin(t04, t14));
  tmaxMin = fmin(tmaxMin, fmax(t04, t14));
   float t05 = (min5 - pos5) * invdir5;
  float t15 = (max5 - pos5) * invdir5;
  tminMax = fmax(tminMax, fmin(t05, t15));
  tmaxMin = fmin(tmaxMin, fmax(t05, t15));
   float t06 = (min6 - pos6) * invdir6;
  float t16 = (max6 - pos6) * invdir6;
  tminMax = fmax(tminMax, fmin(t06, t16));
  tmaxMin = fmin(tmaxMin, fmax(t06, t16));
   float t07 = (min7 - pos7) * invdir7;
  float t17 = (max7 - pos7) * invdir7;
  tminMax = fmax(tminMax, fmin(t07, t17));
  tmaxMin = fmin(tmaxMin, fmax(t07, t17));
  
  *dist = tminMax;
  return tminMax < tmaxMin;
}

uint grey8(float f) {
  uint part = (f * 128.0) + 128.0;
  return 0xFF | (part << 8) | (part << 16) | (part << 24);
}

bool traverseObject8(__global uint *object,
                            float pos0, float dir0,
                           float invdir0,  float pos1, float dir1,
                           float invdir1,  float pos2, float dir2,
                           float invdir2,  float pos3, float dir3,
                           float invdir3,  float pos4, float dir4,
                           float invdir4,  float pos5, float dir5,
                           float invdir5,  float pos6, float dir6,
                           float invdir6,  float pos7, float dir7,
                           float invdir7, 
                           __global uchar * img, __global float *dist,
                           bool hasTerrain,  float scale0,
                            float scale1,
                            float scale2,
                            float scale3,
                            float scale4,
                            float scale5,
                            float scale6,
                            float scale7,
                           
                           __global float *gradVecs, uint numGradVecs,
                           uint numOctaves, float persistence) {
  __global float *fobject = (__global float *)object;
   float origin0 = *fobject++;
   float origin1 = *fobject++;
   float origin2 = *fobject++;
   float origin3 = *fobject++;
   float origin4 = *fobject++;
   float origin5 = *fobject++;
   float origin6 = *fobject++;
   float origin7 = *fobject++;
  
  object = (__global uint *)fobject;
   uint objDim0 = *object++;
   uint objDim1 = *object++;
   uint objDim2 = *object++;
   uint objDim3 = *object++;
   uint objDim4 = *object++;
   uint objDim5 = *object++;
   uint objDim6 = *object++;
   uint objDim7 = *object++;
  
  uint hashMapSize = *object++;
  uint maxHash = hashMapSize * (8 + 1);

  float currDist;
  if (!rayBBox8( pos0, invdir0, origin0,
                      origin0 + objDim0,  pos1, invdir1, origin1,
                      origin1 + objDim1,  pos2, invdir2, origin2,
                      origin2 + objDim2,  pos3, invdir3, origin3,
                      origin3 + objDim3,  pos4, invdir4, origin4,
                      origin4 + objDim4,  pos5, invdir5, origin5,
                      origin5 + objDim5,  pos6, invdir6, origin6,
                      origin6 + objDim6,  pos7, invdir7, origin7,
                      origin7 + objDim7, 
                      &currDist)) {
    return false;
  }

  currDist += 1e-3;
   float currPos0 = pos0 + currDist * dir0;
  int currBlock0 = fastFloor(currPos0 - origin0);
  int dirdir0 = 2 * (invdir0 > 0) - 1;
   float currPos1 = pos1 + currDist * dir1;
  int currBlock1 = fastFloor(currPos1 - origin1);
  int dirdir1 = 2 * (invdir1 > 0) - 1;
   float currPos2 = pos2 + currDist * dir2;
  int currBlock2 = fastFloor(currPos2 - origin2);
  int dirdir2 = 2 * (invdir2 > 0) - 1;
   float currPos3 = pos3 + currDist * dir3;
  int currBlock3 = fastFloor(currPos3 - origin3);
  int dirdir3 = 2 * (invdir3 > 0) - 1;
   float currPos4 = pos4 + currDist * dir4;
  int currBlock4 = fastFloor(currPos4 - origin4);
  int dirdir4 = 2 * (invdir4 > 0) - 1;
   float currPos5 = pos5 + currDist * dir5;
  int currBlock5 = fastFloor(currPos5 - origin5);
  int dirdir5 = 2 * (invdir5 > 0) - 1;
   float currPos6 = pos6 + currDist * dir6;
  int currBlock6 = fastFloor(currPos6 - origin6);
  int dirdir6 = 2 * (invdir6 > 0) - 1;
   float currPos7 = pos7 + currDist * dir7;
  int currBlock7 = fastFloor(currPos7 - origin7);
  int dirdir7 = 2 * (invdir7 > 0) - 1;
  

  if (get_global_id(0) == 400) {
    //printf("%i\n", get_global_id(1));
  }
  while (true) {
    // printf("%i %i %i %i %i\n", get_global_id(0), get_global_id(1),
    // currBlock0, currBlock1, currBlock2);
    uint hashI =
        (8 + 1) *
        hash8( currBlock0,  currBlock1,  currBlock2,  currBlock3,  currBlock4,  currBlock5,  currBlock6,  currBlock7,  0, hashMapSize);
    uint result;
    while (true) {
      if (!(result = object[hashI + 8])) {
        break;
      }
      if ( currBlock0 == object[hashI + 0] &&  currBlock1 == object[hashI + 1] &&  currBlock2 == object[hashI + 2] &&  currBlock3 == object[hashI + 3] &&  currBlock4 == object[hashI + 4] &&  currBlock5 == object[hashI + 5] &&  currBlock6 == object[hashI + 6] &&  currBlock7 == object[hashI + 7] &&  true) {
        break;
      }
      hashI += 8 + 1;
      if (hashI >= maxHash) {
        hashI = 0;
      }
    }
    if (hasTerrain && !result) {
      result = getNoise8(
                    scale0, origin0 + currBlock0,  scale1, origin1 + currBlock1,  scale2, origin2 + currBlock2,  scale3, origin3 + currBlock3,  scale4, origin4 + currBlock4,  scale5, origin5 + currBlock5,  scale6, origin6 + currBlock6,  scale7, origin7 + currBlock7, 
                   gradVecs, numGradVecs, numOctaves, persistence) > 0
                   ? grey8(( currPos0 - origin0 - currBlock0 +  currPos1 - origin1 - currBlock1 +  currPos2 - origin2 - currBlock2 +  currPos3 - origin3 - currBlock3 +  currPos4 - origin4 - currBlock4 +  currPos5 - origin5 - currBlock5 +  currPos6 - origin6 - currBlock6 +  currPos7 - origin7 - currBlock7 +  0.0) / 8)
                   : 0;
    }
    placeBehind(result, img);
    if (*(img + 3) == 0xFF) {
      break;
    }
    float dist = INFINITY;
     dist =
        fmin(dist, invdir0 * (currBlock0 + (invdir0 > 0) -
                                (currPos0 - origin0)));
     dist =
        fmin(dist, invdir1 * (currBlock1 + (invdir1 > 0) -
                                (currPos1 - origin1)));
     dist =
        fmin(dist, invdir2 * (currBlock2 + (invdir2 > 0) -
                                (currPos2 - origin2)));
     dist =
        fmin(dist, invdir3 * (currBlock3 + (invdir3 > 0) -
                                (currPos3 - origin3)));
     dist =
        fmin(dist, invdir4 * (currBlock4 + (invdir4 > 0) -
                                (currPos4 - origin4)));
     dist =
        fmin(dist, invdir5 * (currBlock5 + (invdir5 > 0) -
                                (currPos5 - origin5)));
     dist =
        fmin(dist, invdir6 * (currBlock6 + (invdir6 > 0) -
                                (currPos6 - origin6)));
     dist =
        fmin(dist, invdir7 * (currBlock7 + (invdir7 > 0) -
                                (currPos7 - origin7)));
    
    dist = fmax(dist, 0);
    currDist += dist + 1e-3;
    int change;
    uint changeMask = 0xFFFFFFFFU;
     currPos0 = pos0 + currDist * dir0;
    change = (fastFloor(currPos0 - origin0) - currBlock0) & changeMask;
    changeMask = ~changeMask | ((change > 0) - 1);
    currBlock0 += change;
    if (currBlock0 < 0 || currBlock0 >= (int)objDim0) {
      break;
    }
     currPos1 = pos1 + currDist * dir1;
    change = (fastFloor(currPos1 - origin1) - currBlock1) & changeMask;
    changeMask = ~changeMask | ((change > 0) - 1);
    currBlock1 += change;
    if (currBlock1 < 0 || currBlock1 >= (int)objDim1) {
      break;
    }
     currPos2 = pos2 + currDist * dir2;
    change = (fastFloor(currPos2 - origin2) - currBlock2) & changeMask;
    changeMask = ~changeMask | ((change > 0) - 1);
    currBlock2 += change;
    if (currBlock2 < 0 || currBlock2 >= (int)objDim2) {
      break;
    }
     currPos3 = pos3 + currDist * dir3;
    change = (fastFloor(currPos3 - origin3) - currBlock3) & changeMask;
    changeMask = ~changeMask | ((change > 0) - 1);
    currBlock3 += change;
    if (currBlock3 < 0 || currBlock3 >= (int)objDim3) {
      break;
    }
     currPos4 = pos4 + currDist * dir4;
    change = (fastFloor(currPos4 - origin4) - currBlock4) & changeMask;
    changeMask = ~changeMask | ((change > 0) - 1);
    currBlock4 += change;
    if (currBlock4 < 0 || currBlock4 >= (int)objDim4) {
      break;
    }
     currPos5 = pos5 + currDist * dir5;
    change = (fastFloor(currPos5 - origin5) - currBlock5) & changeMask;
    changeMask = ~changeMask | ((change > 0) - 1);
    currBlock5 += change;
    if (currBlock5 < 0 || currBlock5 >= (int)objDim5) {
      break;
    }
     currPos6 = pos6 + currDist * dir6;
    change = (fastFloor(currPos6 - origin6) - currBlock6) & changeMask;
    changeMask = ~changeMask | ((change > 0) - 1);
    currBlock6 += change;
    if (currBlock6 < 0 || currBlock6 >= (int)objDim6) {
      break;
    }
     currPos7 = pos7 + currDist * dir7;
    change = (fastFloor(currPos7 - origin7) - currBlock7) & changeMask;
    changeMask = ~changeMask | ((change > 0) - 1);
    currBlock7 += change;
    if (currBlock7 < 0 || currBlock7 >= (int)objDim7) {
      break;
    }
    
  }
  return false;
}

bool traverseBVH8(__global uint *bvh,  float pos0,
                        float dir0, float invdir0,  float pos1,
                        float dir1, float invdir1,  float pos2,
                        float dir2, float invdir2,  float pos3,
                        float dir3, float invdir3,  float pos4,
                        float dir4, float invdir4,  float pos5,
                        float dir5, float invdir5,  float pos6,
                        float dir6, float invdir6,  float pos7,
                        float dir7, float invdir7, 
                        __global uchar * img, __global float *dist,
                         float scale0,  float scale1,  float scale2,  float scale3,  float scale4,  float scale5,  float scale6,  float scale7, 
                        __global float *gradVecs, uint numGradVecs,
                        uint numOctaves, float persistence) {
  *img = *(img + 1) = *(img + 2) = *(img + 3) = 0;
  *dist = -1;
  float invdirArr[8];
   invdirArr[0] = invdir0;
   invdirArr[1] = invdir1;
   invdirArr[2] = invdir2;
   invdirArr[3] = invdir3;
   invdirArr[4] = invdir4;
   invdirArr[5] = invdir5;
   invdirArr[6] = invdir6;
   invdirArr[7] = invdir7;
  

  __global uint *stack[24];
  size_t stackInd = 0;
  stack[stackInd++] = bvh;
  float spam;
  while (stackInd) {
    __global uint *currBvh = stack[--stackInd];
    __global float *floatBVH = (__global float *)currBvh;
     float min0 = *floatBVH++;
    float max0 = *floatBVH++;
     float min1 = *floatBVH++;
    float max1 = *floatBVH++;
     float min2 = *floatBVH++;
    float max2 = *floatBVH++;
     float min3 = *floatBVH++;
    float max3 = *floatBVH++;
     float min4 = *floatBVH++;
    float max4 = *floatBVH++;
     float min5 = *floatBVH++;
    float max5 = *floatBVH++;
     float min6 = *floatBVH++;
    float max6 = *floatBVH++;
     float min7 = *floatBVH++;
    float max7 = *floatBVH++;
    
    if (!rayBBox8(
             pos0, invdir0, min0, max0,  pos1, invdir1, min1, max1,  pos2, invdir2, min2, max2,  pos3, invdir3, min3, max3,  pos4, invdir4, min4, max4,  pos5, invdir5, min5, max5,  pos6, invdir6, min6, max6,  pos7, invdir7, min7, max7,  &spam)) {
      continue;
    }
    currBvh = (__global uint *)floatBVH;
    uint objType = *currBvh++;
    if (objType) {
      if (traverseObject8(
              currBvh + *currBvh,  pos0, dir0, invdir0,
               pos1, dir1, invdir1,
               pos2, dir2, invdir2,
               pos3, dir3, invdir3,
               pos4, dir4, invdir4,
               pos5, dir5, invdir5,
               pos6, dir6, invdir6,
               pos7, dir7, invdir7,
              
              img, dist, objType - 1,  scale0,  scale1,  scale2,  scale3,  scale4,  scale5,  scale6,  scale7, 
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

__kernel void renderStd8(__global uint *bvh, __global float *pos,
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
   float pos0 = pos[0];
  float dir0 = forward[0] + xoff * right[0] + yoff * up[0];
   float pos1 = pos[1];
  float dir1 = forward[1] + xoff * right[1] + yoff * up[1];
   float pos2 = pos[2];
  float dir2 = forward[2] + xoff * right[2] + yoff * up[2];
   float pos3 = pos[3];
  float dir3 = forward[3] + xoff * right[3] + yoff * up[3];
   float pos4 = pos[4];
  float dir4 = forward[4] + xoff * right[4] + yoff * up[4];
   float pos5 = pos[5];
  float dir5 = forward[5] + xoff * right[5] + yoff * up[5];
   float pos6 = pos[6];
  float dir6 = forward[6] + xoff * right[6] + yoff * up[6];
   float pos7 = pos[7];
  float dir7 = forward[7] + xoff * right[7] + yoff * up[7];
  
  float norm = 0;
   norm += dir0 * dir0;
   norm += dir1 * dir1;
   norm += dir2 * dir2;
   norm += dir3 * dir3;
   norm += dir4 * dir4;
   norm += dir5 * dir5;
   norm += dir6 * dir6;
   norm += dir7 * dir7;
  
  if (norm < 1e-3f) {
    return;
  }
  norm = sqrt(norm);
   dir0 /= norm;
  float invdir0 = 1 / dir0;
   dir1 /= norm;
  float invdir1 = 1 / dir1;
   dir2 /= norm;
  float invdir2 = 1 / dir2;
   dir3 /= norm;
  float invdir3 = 1 / dir3;
   dir4 /= norm;
  float invdir4 = 1 / dir4;
   dir5 /= norm;
  float invdir5 = 1 / dir5;
   dir6 /= norm;
  float invdir6 = 1 / dir6;
   dir7 /= norm;
  float invdir7 = 1 / dir7;
  
  size_t posIndex = row * width + col;
  img += 4 * posIndex;
  dist += posIndex;
  traverseBVH8(bvh,  pos0, dir0, invdir0,  pos1, dir1, invdir1,  pos2, dir2, invdir2,  pos3, dir3, invdir3,  pos4, dir4, invdir4,  pos5, dir5, invdir5,  pos6, dir6, invdir6,  pos7, dir7, invdir7, 
                     img, dist,  scale[0],  scale[1],  scale[2],  scale[3],  scale[4],  scale[5],  scale[6],  scale[7], 
                     gradVecs, numGradVecs, numOctaves, persistence);
}

)OPENCLPP"
