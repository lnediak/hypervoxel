// AUTO-GENERATED FILE: DO NOT MODIFY
R"OPENCLPP(
#ifndef NON_REP_
#define NON_REP_

uchar mergeColorChannel(uint back, int bitshift, uchar front, uchar frontAlpha, uchar combinedAlpha) {
  return (front * frontAlpha + ((back >> bitshift) & 0xFF) * (255 - frontAlpha) * (back & 0xFF) / 255.0f) / combinedAlpha;
}

void placeBehind(uint color, __global uchar *img) {
  uchar frontAlpha = *(img + 3);
  uchar combinedAlpha = frontAlpha + (color & 0xFF) * (255 - frontAlpha) / 255.0f;
  *img = mergeColorChannel(color, 24, *img, frontAlpha, combinedAlpha);
  img++;
  *img = mergeColorChannel(color, 16, *img, frontAlpha, combinedAlpha);
  img++;
  *img = mergeColorChannel(color, 8, *img, frontAlpha, combinedAlpha);
  img++;
  *img = combinedAlpha;
}

#endif // NON_REP_

/*
BVH format:

float minmax[3][2];
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

float origin[3];
float sidelength;
This is the sidelength of the root node.

The root node is immediately after the header.

Node:

int isLeaf;
If isLeaf, then the following immediately follows:

int color;

Otherwise, the following immediately follows:

int children[2 ** 3];
The index of the node with minimum coordinates is 0, while changing the 2 **
IND bit would change only the coordinate on axis IND.

Each value in children in equal to childPtr - children, where childPtr is a
pointer to the corresponding node.

*/
typedef struct StackEntry3 {

  bool indexed;
  __global uint *node;
  size_t index;
   float middle0;
  float farEnd0;
   float middle1;
  float farEnd1;
   float middle2;
  float farEnd2;
  
} StackEntry3;

void makeChild3(const StackEntry3 *parent, size_t index,
                      StackEntry3 *child) {
  child->indexed = false;
  child->node = parent->node + 1 + parent->node[index + 1];
   child->middle0 =
      1.5f * parent->middle0 - 0.5f * parent->farEnd0 +
      (((index >> 0) & 1) ^ (parent->farEnd0 < parent->middle0)) *
          (parent->farEnd0 - parent->middle0);
  child->farEnd0 =
      child->middle0 + (parent->farEnd0 - parent->middle0) / 2;
   child->middle1 =
      1.5f * parent->middle1 - 0.5f * parent->farEnd1 +
      (((index >> 1) & 1) ^ (parent->farEnd1 < parent->middle1)) *
          (parent->farEnd1 - parent->middle1);
  child->farEnd1 =
      child->middle1 + (parent->farEnd1 - parent->middle1) / 2;
   child->middle2 =
      1.5f * parent->middle2 - 0.5f * parent->farEnd2 +
      (((index >> 2) & 1) ^ (parent->farEnd2 < parent->middle2)) *
          (parent->farEnd2 - parent->middle2);
  child->farEnd2 =
      child->middle2 + (parent->farEnd2 - parent->middle2) / 2;
  
}

bool traverseOctree3(__global uint *octree,
                            float pos0, float dir0,
                           float invdir0,  float pos1, float dir1,
                           float invdir1,  float pos2, float dir2,
                           float invdir2, 
                           __global uchar * img, __global float *dist) {
  __global float *header = (__global float *)octree;
   float origin0 = *header++;
   float origin1 = *header++;
   float origin2 = *header++;
  
  float sidelength = *header++;
  octree = (__global uint *)header;

  float tminMax = 1e-8f;
  float tmaxMin = FLT_MAX;
   float t00 = (origin0 - pos0) * invdir0;
  float t10 = (origin0 + sidelength - pos0) * invdir0;
  tminMax = fmax(tminMax, fmin(t00, t10));
  tmaxMin = fmin(tmaxMin, fmax(t00, t10));
   float t01 = (origin1 - pos1) * invdir1;
  float t11 = (origin1 + sidelength - pos1) * invdir1;
  tminMax = fmax(tminMax, fmin(t01, t11));
  tmaxMin = fmin(tmaxMin, fmax(t01, t11));
   float t02 = (origin2 - pos2) * invdir2;
  float t12 = (origin2 + sidelength - pos2) * invdir2;
  tminMax = fmax(tminMax, fmin(t02, t12));
  tmaxMin = fmin(tmaxMin, fmax(t02, t12));
  
  if (tminMax >= tmaxMin) {
    return false;
  }

  float currDist = tminMax + 1e-3f;
   float currPos0 = pos0 + currDist * dir0;
   float currPos1 = pos1 + currDist * dir1;
   float currPos2 = pos2 + currDist * dir2;
  

  StackEntry3 stack[12];
  size_t stackInd = 0;
  StackEntry3 baseEntry;
  baseEntry.indexed = false;
  baseEntry.node = octree;
   baseEntry.middle0 = origin0 + sidelength / 2;
  baseEntry.farEnd0 = origin0 + (invdir0 > 0) * sidelength;
   baseEntry.middle1 = origin1 + sidelength / 2;
  baseEntry.farEnd1 = origin1 + (invdir1 > 0) * sidelength;
   baseEntry.middle2 = origin2 + sidelength / 2;
  baseEntry.farEnd2 = origin2 + (invdir2 > 0) * sidelength;
  
  stack[stackInd++] = baseEntry;
  bool debug = true;
  __global float *distp = dist;
  size_t i = 0;
  while (stackInd && i < 100) {
    i++;
    StackEntry3 entry = stack[--stackInd];
    /*
    if (debug) {
      printf("currDist: %f, entry.indexed: %i, entry.index: %lu\n", currDist,
             (int)entry.indexed, entry.index);
       printf(
          "currPos0: %f, entry.middle0: %f, entry.farEnd0: %f\n",
          currPos0, entry.middle0, entry.farEnd0);
       printf(
          "currPos1: %f, entry.middle1: %f, entry.farEnd1: %f\n",
          currPos1, entry.middle1, entry.farEnd1);
       printf(
          "currPos2: %f, entry.middle2: %f, entry.farEnd2: %f\n",
          currPos2, entry.middle2, entry.farEnd2);
      
      if (*entry.node) {
        printf("Leaf node\n");
      } else {
        printf("\n");
      }
    }
    */
    /*
    *distp++ = currDist;
     *distp++ = currPos0;
    *distp++ = invdir0;
    *distp++ = entry.middle0;
    *distp++ = entry.farEnd0;
     *distp++ = currPos1;
    *distp++ = invdir1;
    *distp++ = entry.middle1;
    *distp++ = entry.farEnd1;
     *distp++ = currPos2;
    *distp++ = invdir2;
    *distp++ = entry.middle2;
    *distp++ = entry.farEnd2;
    
    *distp++ = *entry.node;
    *distp++ = stackInd;
    */
    if (*entry.node) {
      // this is a leaf node
      placeBehind(*(entry.node + 1), img);
      if (*(img + 3) == 0xFF) {
        *dist = currDist;
        return true;
      }
      float minStep = FLT_MAX;
       minStep =
          fmin(minStep, (entry.farEnd0 - currPos0) * invdir0);
       minStep =
          fmin(minStep, (entry.farEnd1 - currPos1) * invdir1);
       minStep =
          fmin(minStep, (entry.farEnd2 - currPos2) * invdir2);
      
      if (minStep <= -1e-3f) {
        continue;
      }
      currDist += minStep + 1e-3f;
       currPos0 = pos0 + currDist * dir0;
       currPos1 = pos1 + currDist * dir1;
       currPos2 = pos2 + currDist * dir2;
      
    } else {
      size_t newIndex = (
                            ((currPos0 > entry.middle0) ||
                             (currPos0 == entry.middle0 && (invdir0 > 0)))
                            << 0) |
                        (
                            ((currPos1 > entry.middle1) ||
                             (currPos1 == entry.middle1 && (invdir1 > 0)))
                            << 1) |
                        (
                            ((currPos2 > entry.middle2) ||
                             (currPos2 == entry.middle2 && (invdir2 > 0)))
                            << 2) |
                        
      0;
      if (entry.indexed) {
        size_t diffIndex = newIndex ^ entry.index;
        if (diffIndex) {
          entry.index = newIndex;
          if (((entry.farEnd0 - currPos0) * invdir0 >
                                 0) &&
                  ((entry.farEnd1 - currPos1) * invdir1 >
                                 0) &&
                  ((entry.farEnd2 - currPos2) * invdir2 >
                                 0) &&
                  
              true) {
            stack[stackInd++] = entry;
          }
          size_t tmpIndex = newIndex;
          StackEntry3 child;
           if ((1 << 0) & diffIndex) {
            makeChild3(&entry, tmpIndex, &child);
            stack[stackInd++] = child;
            tmpIndex ^= (1 << 0);
          }
           if ((1 << 1) & diffIndex) {
            makeChild3(&entry, tmpIndex, &child);
            stack[stackInd++] = child;
            tmpIndex ^= (1 << 1);
          }
           if ((1 << 2) & diffIndex) {
            makeChild3(&entry, tmpIndex, &child);
            stack[stackInd++] = child;
            tmpIndex ^= (1 << 2);
          }
          
          continue;
        }
      }
      entry.index = newIndex;
      entry.indexed = true;
      if (((entry.farEnd0 - currPos0) * invdir0 > 0) &&
              ((entry.farEnd1 - currPos1) * invdir1 > 0) &&
              ((entry.farEnd2 - currPos2) * invdir2 > 0) &&
              
          true) {
        stack[stackInd++] = entry;
      }
      StackEntry3 child;
      makeChild3(&entry, entry.index, &child);
      stack[stackInd++] = child;
    }
  }
  return false;
}

bool rayBBox3( float pos0, float invdir0,
                    float min0, float max0,  float pos1, float invdir1,
                    float min1, float max1,  float pos2, float invdir2,
                    float min2, float max2, 
                    int unused) {
  (void)unused;
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
  
  return tminMax < tmaxMin;
}

bool traverseBVH3(__global uint *bvh,  float pos0,
                        float dir0, float invdir0,  float pos1,
                        float dir1, float invdir1,  float pos2,
                        float dir2, float invdir2, 
                        __global uchar * img, __global float *dist) {
  *img = *(img + 1) = *(img + 2) = *(img + 3) = 0;
  *dist = -1;
  float invdirArr[3];
   invdirArr[0] = invdir0;
   invdirArr[1] = invdir1;
   invdirArr[2] = invdir2;
  

  __global uint *stack[24];
  size_t stackInd = 0;
  stack[stackInd++] = bvh;
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
             pos0, invdir0, min0, max0,  pos1, invdir1, min1, max1,  pos2, invdir2, min2, max2,  0)) {
      continue;
    }
    currBvh = (__global uint *)floatBVH;
    if (*currBvh++) {
      if (traverseOctree3(currBvh + *currBvh,  pos0,
                                dir0, invdir0,  pos1,
                                dir1, invdir1,  pos2,
                                dir2, invdir2, 
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

__kernel void renderStd3(__global uint *bvh, __global float *pos,
                               __global float *forward, __global float *right,
                               __global float *up, __global uchar *img,
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
                     img, dist);
}

#ifndef NON_REP_
#define NON_REP_

uchar mergeColorChannel(uint back, int bitshift, uchar front, uchar frontAlpha, uchar combinedAlpha) {
  return (front * frontAlpha + ((back >> bitshift) & 0xFF) * (255 - frontAlpha) * (back & 0xFF) / 255.0f) / combinedAlpha;
}

void placeBehind(uint color, __global uchar *img) {
  uchar frontAlpha = *(img + 3);
  uchar combinedAlpha = frontAlpha + (color & 0xFF) * (255 - frontAlpha) / 255.0f;
  *img = mergeColorChannel(color, 24, *img, frontAlpha, combinedAlpha);
  img++;
  *img = mergeColorChannel(color, 16, *img, frontAlpha, combinedAlpha);
  img++;
  *img = mergeColorChannel(color, 8, *img, frontAlpha, combinedAlpha);
  img++;
  *img = combinedAlpha;
}

#endif // NON_REP_

/*
BVH format:

float minmax[4][2];
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

float origin[4];
float sidelength;
This is the sidelength of the root node.

The root node is immediately after the header.

Node:

int isLeaf;
If isLeaf, then the following immediately follows:

int color;

Otherwise, the following immediately follows:

int children[2 ** 4];
The index of the node with minimum coordinates is 0, while changing the 2 **
IND bit would change only the coordinate on axis IND.

Each value in children in equal to childPtr - children, where childPtr is a
pointer to the corresponding node.

*/
typedef struct StackEntry4 {

  bool indexed;
  __global uint *node;
  size_t index;
   float middle0;
  float farEnd0;
   float middle1;
  float farEnd1;
   float middle2;
  float farEnd2;
   float middle3;
  float farEnd3;
  
} StackEntry4;

void makeChild4(const StackEntry4 *parent, size_t index,
                      StackEntry4 *child) {
  child->indexed = false;
  child->node = parent->node + 1 + parent->node[index + 1];
   child->middle0 =
      1.5f * parent->middle0 - 0.5f * parent->farEnd0 +
      (((index >> 0) & 1) ^ (parent->farEnd0 < parent->middle0)) *
          (parent->farEnd0 - parent->middle0);
  child->farEnd0 =
      child->middle0 + (parent->farEnd0 - parent->middle0) / 2;
   child->middle1 =
      1.5f * parent->middle1 - 0.5f * parent->farEnd1 +
      (((index >> 1) & 1) ^ (parent->farEnd1 < parent->middle1)) *
          (parent->farEnd1 - parent->middle1);
  child->farEnd1 =
      child->middle1 + (parent->farEnd1 - parent->middle1) / 2;
   child->middle2 =
      1.5f * parent->middle2 - 0.5f * parent->farEnd2 +
      (((index >> 2) & 1) ^ (parent->farEnd2 < parent->middle2)) *
          (parent->farEnd2 - parent->middle2);
  child->farEnd2 =
      child->middle2 + (parent->farEnd2 - parent->middle2) / 2;
   child->middle3 =
      1.5f * parent->middle3 - 0.5f * parent->farEnd3 +
      (((index >> 3) & 1) ^ (parent->farEnd3 < parent->middle3)) *
          (parent->farEnd3 - parent->middle3);
  child->farEnd3 =
      child->middle3 + (parent->farEnd3 - parent->middle3) / 2;
  
}

bool traverseOctree4(__global uint *octree,
                            float pos0, float dir0,
                           float invdir0,  float pos1, float dir1,
                           float invdir1,  float pos2, float dir2,
                           float invdir2,  float pos3, float dir3,
                           float invdir3, 
                           __global uchar * img, __global float *dist) {
  __global float *header = (__global float *)octree;
   float origin0 = *header++;
   float origin1 = *header++;
   float origin2 = *header++;
   float origin3 = *header++;
  
  float sidelength = *header++;
  octree = (__global uint *)header;

  float tminMax = 1e-8f;
  float tmaxMin = FLT_MAX;
   float t00 = (origin0 - pos0) * invdir0;
  float t10 = (origin0 + sidelength - pos0) * invdir0;
  tminMax = fmax(tminMax, fmin(t00, t10));
  tmaxMin = fmin(tmaxMin, fmax(t00, t10));
   float t01 = (origin1 - pos1) * invdir1;
  float t11 = (origin1 + sidelength - pos1) * invdir1;
  tminMax = fmax(tminMax, fmin(t01, t11));
  tmaxMin = fmin(tmaxMin, fmax(t01, t11));
   float t02 = (origin2 - pos2) * invdir2;
  float t12 = (origin2 + sidelength - pos2) * invdir2;
  tminMax = fmax(tminMax, fmin(t02, t12));
  tmaxMin = fmin(tmaxMin, fmax(t02, t12));
   float t03 = (origin3 - pos3) * invdir3;
  float t13 = (origin3 + sidelength - pos3) * invdir3;
  tminMax = fmax(tminMax, fmin(t03, t13));
  tmaxMin = fmin(tmaxMin, fmax(t03, t13));
  
  if (tminMax >= tmaxMin) {
    return false;
  }

  float currDist = tminMax + 1e-3f;
   float currPos0 = pos0 + currDist * dir0;
   float currPos1 = pos1 + currDist * dir1;
   float currPos2 = pos2 + currDist * dir2;
   float currPos3 = pos3 + currDist * dir3;
  

  StackEntry4 stack[12];
  size_t stackInd = 0;
  StackEntry4 baseEntry;
  baseEntry.indexed = false;
  baseEntry.node = octree;
   baseEntry.middle0 = origin0 + sidelength / 2;
  baseEntry.farEnd0 = origin0 + (invdir0 > 0) * sidelength;
   baseEntry.middle1 = origin1 + sidelength / 2;
  baseEntry.farEnd1 = origin1 + (invdir1 > 0) * sidelength;
   baseEntry.middle2 = origin2 + sidelength / 2;
  baseEntry.farEnd2 = origin2 + (invdir2 > 0) * sidelength;
   baseEntry.middle3 = origin3 + sidelength / 2;
  baseEntry.farEnd3 = origin3 + (invdir3 > 0) * sidelength;
  
  stack[stackInd++] = baseEntry;
  bool debug = true;
  __global float *distp = dist;
  size_t i = 0;
  while (stackInd && i < 100) {
    i++;
    StackEntry4 entry = stack[--stackInd];
    /*
    if (debug) {
      printf("currDist: %f, entry.indexed: %i, entry.index: %lu\n", currDist,
             (int)entry.indexed, entry.index);
       printf(
          "currPos0: %f, entry.middle0: %f, entry.farEnd0: %f\n",
          currPos0, entry.middle0, entry.farEnd0);
       printf(
          "currPos1: %f, entry.middle1: %f, entry.farEnd1: %f\n",
          currPos1, entry.middle1, entry.farEnd1);
       printf(
          "currPos2: %f, entry.middle2: %f, entry.farEnd2: %f\n",
          currPos2, entry.middle2, entry.farEnd2);
       printf(
          "currPos3: %f, entry.middle3: %f, entry.farEnd3: %f\n",
          currPos3, entry.middle3, entry.farEnd3);
      
      if (*entry.node) {
        printf("Leaf node\n");
      } else {
        printf("\n");
      }
    }
    */
    /*
    *distp++ = currDist;
     *distp++ = currPos0;
    *distp++ = invdir0;
    *distp++ = entry.middle0;
    *distp++ = entry.farEnd0;
     *distp++ = currPos1;
    *distp++ = invdir1;
    *distp++ = entry.middle1;
    *distp++ = entry.farEnd1;
     *distp++ = currPos2;
    *distp++ = invdir2;
    *distp++ = entry.middle2;
    *distp++ = entry.farEnd2;
     *distp++ = currPos3;
    *distp++ = invdir3;
    *distp++ = entry.middle3;
    *distp++ = entry.farEnd3;
    
    *distp++ = *entry.node;
    *distp++ = stackInd;
    */
    if (*entry.node) {
      // this is a leaf node
      placeBehind(*(entry.node + 1), img);
      if (*(img + 3) == 0xFF) {
        *dist = currDist;
        return true;
      }
      float minStep = FLT_MAX;
       minStep =
          fmin(minStep, (entry.farEnd0 - currPos0) * invdir0);
       minStep =
          fmin(minStep, (entry.farEnd1 - currPos1) * invdir1);
       minStep =
          fmin(minStep, (entry.farEnd2 - currPos2) * invdir2);
       minStep =
          fmin(minStep, (entry.farEnd3 - currPos3) * invdir3);
      
      if (minStep <= -1e-3f) {
        continue;
      }
      currDist += minStep + 1e-3f;
       currPos0 = pos0 + currDist * dir0;
       currPos1 = pos1 + currDist * dir1;
       currPos2 = pos2 + currDist * dir2;
       currPos3 = pos3 + currDist * dir3;
      
    } else {
      size_t newIndex = (
                            ((currPos0 > entry.middle0) ||
                             (currPos0 == entry.middle0 && (invdir0 > 0)))
                            << 0) |
                        (
                            ((currPos1 > entry.middle1) ||
                             (currPos1 == entry.middle1 && (invdir1 > 0)))
                            << 1) |
                        (
                            ((currPos2 > entry.middle2) ||
                             (currPos2 == entry.middle2 && (invdir2 > 0)))
                            << 2) |
                        (
                            ((currPos3 > entry.middle3) ||
                             (currPos3 == entry.middle3 && (invdir3 > 0)))
                            << 3) |
                        
      0;
      if (entry.indexed) {
        size_t diffIndex = newIndex ^ entry.index;
        if (diffIndex) {
          entry.index = newIndex;
          if (((entry.farEnd0 - currPos0) * invdir0 >
                                 0) &&
                  ((entry.farEnd1 - currPos1) * invdir1 >
                                 0) &&
                  ((entry.farEnd2 - currPos2) * invdir2 >
                                 0) &&
                  ((entry.farEnd3 - currPos3) * invdir3 >
                                 0) &&
                  
              true) {
            stack[stackInd++] = entry;
          }
          size_t tmpIndex = newIndex;
          StackEntry4 child;
           if ((1 << 0) & diffIndex) {
            makeChild4(&entry, tmpIndex, &child);
            stack[stackInd++] = child;
            tmpIndex ^= (1 << 0);
          }
           if ((1 << 1) & diffIndex) {
            makeChild4(&entry, tmpIndex, &child);
            stack[stackInd++] = child;
            tmpIndex ^= (1 << 1);
          }
           if ((1 << 2) & diffIndex) {
            makeChild4(&entry, tmpIndex, &child);
            stack[stackInd++] = child;
            tmpIndex ^= (1 << 2);
          }
           if ((1 << 3) & diffIndex) {
            makeChild4(&entry, tmpIndex, &child);
            stack[stackInd++] = child;
            tmpIndex ^= (1 << 3);
          }
          
          continue;
        }
      }
      entry.index = newIndex;
      entry.indexed = true;
      if (((entry.farEnd0 - currPos0) * invdir0 > 0) &&
              ((entry.farEnd1 - currPos1) * invdir1 > 0) &&
              ((entry.farEnd2 - currPos2) * invdir2 > 0) &&
              ((entry.farEnd3 - currPos3) * invdir3 > 0) &&
              
          true) {
        stack[stackInd++] = entry;
      }
      StackEntry4 child;
      makeChild4(&entry, entry.index, &child);
      stack[stackInd++] = child;
    }
  }
  return false;
}

bool rayBBox4( float pos0, float invdir0,
                    float min0, float max0,  float pos1, float invdir1,
                    float min1, float max1,  float pos2, float invdir2,
                    float min2, float max2,  float pos3, float invdir3,
                    float min3, float max3, 
                    int unused) {
  (void)unused;
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
  
  return tminMax < tmaxMin;
}

bool traverseBVH4(__global uint *bvh,  float pos0,
                        float dir0, float invdir0,  float pos1,
                        float dir1, float invdir1,  float pos2,
                        float dir2, float invdir2,  float pos3,
                        float dir3, float invdir3, 
                        __global uchar * img, __global float *dist) {
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
             pos0, invdir0, min0, max0,  pos1, invdir1, min1, max1,  pos2, invdir2, min2, max2,  pos3, invdir3, min3, max3,  0)) {
      continue;
    }
    currBvh = (__global uint *)floatBVH;
    if (*currBvh++) {
      if (traverseOctree4(currBvh + *currBvh,  pos0,
                                dir0, invdir0,  pos1,
                                dir1, invdir1,  pos2,
                                dir2, invdir2,  pos3,
                                dir3, invdir3, 
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

__kernel void renderStd4(__global uint *bvh, __global float *pos,
                               __global float *forward, __global float *right,
                               __global float *up, __global uchar *img,
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
                     img, dist);
}

#ifndef NON_REP_
#define NON_REP_

uchar mergeColorChannel(uint back, int bitshift, uchar front, uchar frontAlpha, uchar combinedAlpha) {
  return (front * frontAlpha + ((back >> bitshift) & 0xFF) * (255 - frontAlpha) * (back & 0xFF) / 255.0f) / combinedAlpha;
}

void placeBehind(uint color, __global uchar *img) {
  uchar frontAlpha = *(img + 3);
  uchar combinedAlpha = frontAlpha + (color & 0xFF) * (255 - frontAlpha) / 255.0f;
  *img = mergeColorChannel(color, 24, *img, frontAlpha, combinedAlpha);
  img++;
  *img = mergeColorChannel(color, 16, *img, frontAlpha, combinedAlpha);
  img++;
  *img = mergeColorChannel(color, 8, *img, frontAlpha, combinedAlpha);
  img++;
  *img = combinedAlpha;
}

#endif // NON_REP_

/*
BVH format:

float minmax[5][2];
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

float origin[5];
float sidelength;
This is the sidelength of the root node.

The root node is immediately after the header.

Node:

int isLeaf;
If isLeaf, then the following immediately follows:

int color;

Otherwise, the following immediately follows:

int children[2 ** 5];
The index of the node with minimum coordinates is 0, while changing the 2 **
IND bit would change only the coordinate on axis IND.

Each value in children in equal to childPtr - children, where childPtr is a
pointer to the corresponding node.

*/
typedef struct StackEntry5 {

  bool indexed;
  __global uint *node;
  size_t index;
   float middle0;
  float farEnd0;
   float middle1;
  float farEnd1;
   float middle2;
  float farEnd2;
   float middle3;
  float farEnd3;
   float middle4;
  float farEnd4;
  
} StackEntry5;

void makeChild5(const StackEntry5 *parent, size_t index,
                      StackEntry5 *child) {
  child->indexed = false;
  child->node = parent->node + 1 + parent->node[index + 1];
   child->middle0 =
      1.5f * parent->middle0 - 0.5f * parent->farEnd0 +
      (((index >> 0) & 1) ^ (parent->farEnd0 < parent->middle0)) *
          (parent->farEnd0 - parent->middle0);
  child->farEnd0 =
      child->middle0 + (parent->farEnd0 - parent->middle0) / 2;
   child->middle1 =
      1.5f * parent->middle1 - 0.5f * parent->farEnd1 +
      (((index >> 1) & 1) ^ (parent->farEnd1 < parent->middle1)) *
          (parent->farEnd1 - parent->middle1);
  child->farEnd1 =
      child->middle1 + (parent->farEnd1 - parent->middle1) / 2;
   child->middle2 =
      1.5f * parent->middle2 - 0.5f * parent->farEnd2 +
      (((index >> 2) & 1) ^ (parent->farEnd2 < parent->middle2)) *
          (parent->farEnd2 - parent->middle2);
  child->farEnd2 =
      child->middle2 + (parent->farEnd2 - parent->middle2) / 2;
   child->middle3 =
      1.5f * parent->middle3 - 0.5f * parent->farEnd3 +
      (((index >> 3) & 1) ^ (parent->farEnd3 < parent->middle3)) *
          (parent->farEnd3 - parent->middle3);
  child->farEnd3 =
      child->middle3 + (parent->farEnd3 - parent->middle3) / 2;
   child->middle4 =
      1.5f * parent->middle4 - 0.5f * parent->farEnd4 +
      (((index >> 4) & 1) ^ (parent->farEnd4 < parent->middle4)) *
          (parent->farEnd4 - parent->middle4);
  child->farEnd4 =
      child->middle4 + (parent->farEnd4 - parent->middle4) / 2;
  
}

bool traverseOctree5(__global uint *octree,
                            float pos0, float dir0,
                           float invdir0,  float pos1, float dir1,
                           float invdir1,  float pos2, float dir2,
                           float invdir2,  float pos3, float dir3,
                           float invdir3,  float pos4, float dir4,
                           float invdir4, 
                           __global uchar * img, __global float *dist) {
  __global float *header = (__global float *)octree;
   float origin0 = *header++;
   float origin1 = *header++;
   float origin2 = *header++;
   float origin3 = *header++;
   float origin4 = *header++;
  
  float sidelength = *header++;
  octree = (__global uint *)header;

  float tminMax = 1e-8f;
  float tmaxMin = FLT_MAX;
   float t00 = (origin0 - pos0) * invdir0;
  float t10 = (origin0 + sidelength - pos0) * invdir0;
  tminMax = fmax(tminMax, fmin(t00, t10));
  tmaxMin = fmin(tmaxMin, fmax(t00, t10));
   float t01 = (origin1 - pos1) * invdir1;
  float t11 = (origin1 + sidelength - pos1) * invdir1;
  tminMax = fmax(tminMax, fmin(t01, t11));
  tmaxMin = fmin(tmaxMin, fmax(t01, t11));
   float t02 = (origin2 - pos2) * invdir2;
  float t12 = (origin2 + sidelength - pos2) * invdir2;
  tminMax = fmax(tminMax, fmin(t02, t12));
  tmaxMin = fmin(tmaxMin, fmax(t02, t12));
   float t03 = (origin3 - pos3) * invdir3;
  float t13 = (origin3 + sidelength - pos3) * invdir3;
  tminMax = fmax(tminMax, fmin(t03, t13));
  tmaxMin = fmin(tmaxMin, fmax(t03, t13));
   float t04 = (origin4 - pos4) * invdir4;
  float t14 = (origin4 + sidelength - pos4) * invdir4;
  tminMax = fmax(tminMax, fmin(t04, t14));
  tmaxMin = fmin(tmaxMin, fmax(t04, t14));
  
  if (tminMax >= tmaxMin) {
    return false;
  }

  float currDist = tminMax + 1e-3f;
   float currPos0 = pos0 + currDist * dir0;
   float currPos1 = pos1 + currDist * dir1;
   float currPos2 = pos2 + currDist * dir2;
   float currPos3 = pos3 + currDist * dir3;
   float currPos4 = pos4 + currDist * dir4;
  

  StackEntry5 stack[12];
  size_t stackInd = 0;
  StackEntry5 baseEntry;
  baseEntry.indexed = false;
  baseEntry.node = octree;
   baseEntry.middle0 = origin0 + sidelength / 2;
  baseEntry.farEnd0 = origin0 + (invdir0 > 0) * sidelength;
   baseEntry.middle1 = origin1 + sidelength / 2;
  baseEntry.farEnd1 = origin1 + (invdir1 > 0) * sidelength;
   baseEntry.middle2 = origin2 + sidelength / 2;
  baseEntry.farEnd2 = origin2 + (invdir2 > 0) * sidelength;
   baseEntry.middle3 = origin3 + sidelength / 2;
  baseEntry.farEnd3 = origin3 + (invdir3 > 0) * sidelength;
   baseEntry.middle4 = origin4 + sidelength / 2;
  baseEntry.farEnd4 = origin4 + (invdir4 > 0) * sidelength;
  
  stack[stackInd++] = baseEntry;
  bool debug = true;
  __global float *distp = dist;
  size_t i = 0;
  while (stackInd && i < 100) {
    i++;
    StackEntry5 entry = stack[--stackInd];
    /*
    if (debug) {
      printf("currDist: %f, entry.indexed: %i, entry.index: %lu\n", currDist,
             (int)entry.indexed, entry.index);
       printf(
          "currPos0: %f, entry.middle0: %f, entry.farEnd0: %f\n",
          currPos0, entry.middle0, entry.farEnd0);
       printf(
          "currPos1: %f, entry.middle1: %f, entry.farEnd1: %f\n",
          currPos1, entry.middle1, entry.farEnd1);
       printf(
          "currPos2: %f, entry.middle2: %f, entry.farEnd2: %f\n",
          currPos2, entry.middle2, entry.farEnd2);
       printf(
          "currPos3: %f, entry.middle3: %f, entry.farEnd3: %f\n",
          currPos3, entry.middle3, entry.farEnd3);
       printf(
          "currPos4: %f, entry.middle4: %f, entry.farEnd4: %f\n",
          currPos4, entry.middle4, entry.farEnd4);
      
      if (*entry.node) {
        printf("Leaf node\n");
      } else {
        printf("\n");
      }
    }
    */
    /*
    *distp++ = currDist;
     *distp++ = currPos0;
    *distp++ = invdir0;
    *distp++ = entry.middle0;
    *distp++ = entry.farEnd0;
     *distp++ = currPos1;
    *distp++ = invdir1;
    *distp++ = entry.middle1;
    *distp++ = entry.farEnd1;
     *distp++ = currPos2;
    *distp++ = invdir2;
    *distp++ = entry.middle2;
    *distp++ = entry.farEnd2;
     *distp++ = currPos3;
    *distp++ = invdir3;
    *distp++ = entry.middle3;
    *distp++ = entry.farEnd3;
     *distp++ = currPos4;
    *distp++ = invdir4;
    *distp++ = entry.middle4;
    *distp++ = entry.farEnd4;
    
    *distp++ = *entry.node;
    *distp++ = stackInd;
    */
    if (*entry.node) {
      // this is a leaf node
      placeBehind(*(entry.node + 1), img);
      if (*(img + 3) == 0xFF) {
        *dist = currDist;
        return true;
      }
      float minStep = FLT_MAX;
       minStep =
          fmin(minStep, (entry.farEnd0 - currPos0) * invdir0);
       minStep =
          fmin(minStep, (entry.farEnd1 - currPos1) * invdir1);
       minStep =
          fmin(minStep, (entry.farEnd2 - currPos2) * invdir2);
       minStep =
          fmin(minStep, (entry.farEnd3 - currPos3) * invdir3);
       minStep =
          fmin(minStep, (entry.farEnd4 - currPos4) * invdir4);
      
      if (minStep <= -1e-3f) {
        continue;
      }
      currDist += minStep + 1e-3f;
       currPos0 = pos0 + currDist * dir0;
       currPos1 = pos1 + currDist * dir1;
       currPos2 = pos2 + currDist * dir2;
       currPos3 = pos3 + currDist * dir3;
       currPos4 = pos4 + currDist * dir4;
      
    } else {
      size_t newIndex = (
                            ((currPos0 > entry.middle0) ||
                             (currPos0 == entry.middle0 && (invdir0 > 0)))
                            << 0) |
                        (
                            ((currPos1 > entry.middle1) ||
                             (currPos1 == entry.middle1 && (invdir1 > 0)))
                            << 1) |
                        (
                            ((currPos2 > entry.middle2) ||
                             (currPos2 == entry.middle2 && (invdir2 > 0)))
                            << 2) |
                        (
                            ((currPos3 > entry.middle3) ||
                             (currPos3 == entry.middle3 && (invdir3 > 0)))
                            << 3) |
                        (
                            ((currPos4 > entry.middle4) ||
                             (currPos4 == entry.middle4 && (invdir4 > 0)))
                            << 4) |
                        
      0;
      if (entry.indexed) {
        size_t diffIndex = newIndex ^ entry.index;
        if (diffIndex) {
          entry.index = newIndex;
          if (((entry.farEnd0 - currPos0) * invdir0 >
                                 0) &&
                  ((entry.farEnd1 - currPos1) * invdir1 >
                                 0) &&
                  ((entry.farEnd2 - currPos2) * invdir2 >
                                 0) &&
                  ((entry.farEnd3 - currPos3) * invdir3 >
                                 0) &&
                  ((entry.farEnd4 - currPos4) * invdir4 >
                                 0) &&
                  
              true) {
            stack[stackInd++] = entry;
          }
          size_t tmpIndex = newIndex;
          StackEntry5 child;
           if ((1 << 0) & diffIndex) {
            makeChild5(&entry, tmpIndex, &child);
            stack[stackInd++] = child;
            tmpIndex ^= (1 << 0);
          }
           if ((1 << 1) & diffIndex) {
            makeChild5(&entry, tmpIndex, &child);
            stack[stackInd++] = child;
            tmpIndex ^= (1 << 1);
          }
           if ((1 << 2) & diffIndex) {
            makeChild5(&entry, tmpIndex, &child);
            stack[stackInd++] = child;
            tmpIndex ^= (1 << 2);
          }
           if ((1 << 3) & diffIndex) {
            makeChild5(&entry, tmpIndex, &child);
            stack[stackInd++] = child;
            tmpIndex ^= (1 << 3);
          }
           if ((1 << 4) & diffIndex) {
            makeChild5(&entry, tmpIndex, &child);
            stack[stackInd++] = child;
            tmpIndex ^= (1 << 4);
          }
          
          continue;
        }
      }
      entry.index = newIndex;
      entry.indexed = true;
      if (((entry.farEnd0 - currPos0) * invdir0 > 0) &&
              ((entry.farEnd1 - currPos1) * invdir1 > 0) &&
              ((entry.farEnd2 - currPos2) * invdir2 > 0) &&
              ((entry.farEnd3 - currPos3) * invdir3 > 0) &&
              ((entry.farEnd4 - currPos4) * invdir4 > 0) &&
              
          true) {
        stack[stackInd++] = entry;
      }
      StackEntry5 child;
      makeChild5(&entry, entry.index, &child);
      stack[stackInd++] = child;
    }
  }
  return false;
}

bool rayBBox5( float pos0, float invdir0,
                    float min0, float max0,  float pos1, float invdir1,
                    float min1, float max1,  float pos2, float invdir2,
                    float min2, float max2,  float pos3, float invdir3,
                    float min3, float max3,  float pos4, float invdir4,
                    float min4, float max4, 
                    int unused) {
  (void)unused;
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
  
  return tminMax < tmaxMin;
}

bool traverseBVH5(__global uint *bvh,  float pos0,
                        float dir0, float invdir0,  float pos1,
                        float dir1, float invdir1,  float pos2,
                        float dir2, float invdir2,  float pos3,
                        float dir3, float invdir3,  float pos4,
                        float dir4, float invdir4, 
                        __global uchar * img, __global float *dist) {
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
             pos0, invdir0, min0, max0,  pos1, invdir1, min1, max1,  pos2, invdir2, min2, max2,  pos3, invdir3, min3, max3,  pos4, invdir4, min4, max4,  0)) {
      continue;
    }
    currBvh = (__global uint *)floatBVH;
    if (*currBvh++) {
      if (traverseOctree5(currBvh + *currBvh,  pos0,
                                dir0, invdir0,  pos1,
                                dir1, invdir1,  pos2,
                                dir2, invdir2,  pos3,
                                dir3, invdir3,  pos4,
                                dir4, invdir4, 
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

__kernel void renderStd5(__global uint *bvh, __global float *pos,
                               __global float *forward, __global float *right,
                               __global float *up, __global uchar *img,
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
                     img, dist);
}

#ifndef NON_REP_
#define NON_REP_

uchar mergeColorChannel(uint back, int bitshift, uchar front, uchar frontAlpha, uchar combinedAlpha) {
  return (front * frontAlpha + ((back >> bitshift) & 0xFF) * (255 - frontAlpha) * (back & 0xFF) / 255.0f) / combinedAlpha;
}

void placeBehind(uint color, __global uchar *img) {
  uchar frontAlpha = *(img + 3);
  uchar combinedAlpha = frontAlpha + (color & 0xFF) * (255 - frontAlpha) / 255.0f;
  *img = mergeColorChannel(color, 24, *img, frontAlpha, combinedAlpha);
  img++;
  *img = mergeColorChannel(color, 16, *img, frontAlpha, combinedAlpha);
  img++;
  *img = mergeColorChannel(color, 8, *img, frontAlpha, combinedAlpha);
  img++;
  *img = combinedAlpha;
}

#endif // NON_REP_

/*
BVH format:

float minmax[6][2];
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

float origin[6];
float sidelength;
This is the sidelength of the root node.

The root node is immediately after the header.

Node:

int isLeaf;
If isLeaf, then the following immediately follows:

int color;

Otherwise, the following immediately follows:

int children[2 ** 6];
The index of the node with minimum coordinates is 0, while changing the 2 **
IND bit would change only the coordinate on axis IND.

Each value in children in equal to childPtr - children, where childPtr is a
pointer to the corresponding node.

*/
typedef struct StackEntry6 {

  bool indexed;
  __global uint *node;
  size_t index;
   float middle0;
  float farEnd0;
   float middle1;
  float farEnd1;
   float middle2;
  float farEnd2;
   float middle3;
  float farEnd3;
   float middle4;
  float farEnd4;
   float middle5;
  float farEnd5;
  
} StackEntry6;

void makeChild6(const StackEntry6 *parent, size_t index,
                      StackEntry6 *child) {
  child->indexed = false;
  child->node = parent->node + 1 + parent->node[index + 1];
   child->middle0 =
      1.5f * parent->middle0 - 0.5f * parent->farEnd0 +
      (((index >> 0) & 1) ^ (parent->farEnd0 < parent->middle0)) *
          (parent->farEnd0 - parent->middle0);
  child->farEnd0 =
      child->middle0 + (parent->farEnd0 - parent->middle0) / 2;
   child->middle1 =
      1.5f * parent->middle1 - 0.5f * parent->farEnd1 +
      (((index >> 1) & 1) ^ (parent->farEnd1 < parent->middle1)) *
          (parent->farEnd1 - parent->middle1);
  child->farEnd1 =
      child->middle1 + (parent->farEnd1 - parent->middle1) / 2;
   child->middle2 =
      1.5f * parent->middle2 - 0.5f * parent->farEnd2 +
      (((index >> 2) & 1) ^ (parent->farEnd2 < parent->middle2)) *
          (parent->farEnd2 - parent->middle2);
  child->farEnd2 =
      child->middle2 + (parent->farEnd2 - parent->middle2) / 2;
   child->middle3 =
      1.5f * parent->middle3 - 0.5f * parent->farEnd3 +
      (((index >> 3) & 1) ^ (parent->farEnd3 < parent->middle3)) *
          (parent->farEnd3 - parent->middle3);
  child->farEnd3 =
      child->middle3 + (parent->farEnd3 - parent->middle3) / 2;
   child->middle4 =
      1.5f * parent->middle4 - 0.5f * parent->farEnd4 +
      (((index >> 4) & 1) ^ (parent->farEnd4 < parent->middle4)) *
          (parent->farEnd4 - parent->middle4);
  child->farEnd4 =
      child->middle4 + (parent->farEnd4 - parent->middle4) / 2;
   child->middle5 =
      1.5f * parent->middle5 - 0.5f * parent->farEnd5 +
      (((index >> 5) & 1) ^ (parent->farEnd5 < parent->middle5)) *
          (parent->farEnd5 - parent->middle5);
  child->farEnd5 =
      child->middle5 + (parent->farEnd5 - parent->middle5) / 2;
  
}

bool traverseOctree6(__global uint *octree,
                            float pos0, float dir0,
                           float invdir0,  float pos1, float dir1,
                           float invdir1,  float pos2, float dir2,
                           float invdir2,  float pos3, float dir3,
                           float invdir3,  float pos4, float dir4,
                           float invdir4,  float pos5, float dir5,
                           float invdir5, 
                           __global uchar * img, __global float *dist) {
  __global float *header = (__global float *)octree;
   float origin0 = *header++;
   float origin1 = *header++;
   float origin2 = *header++;
   float origin3 = *header++;
   float origin4 = *header++;
   float origin5 = *header++;
  
  float sidelength = *header++;
  octree = (__global uint *)header;

  float tminMax = 1e-8f;
  float tmaxMin = FLT_MAX;
   float t00 = (origin0 - pos0) * invdir0;
  float t10 = (origin0 + sidelength - pos0) * invdir0;
  tminMax = fmax(tminMax, fmin(t00, t10));
  tmaxMin = fmin(tmaxMin, fmax(t00, t10));
   float t01 = (origin1 - pos1) * invdir1;
  float t11 = (origin1 + sidelength - pos1) * invdir1;
  tminMax = fmax(tminMax, fmin(t01, t11));
  tmaxMin = fmin(tmaxMin, fmax(t01, t11));
   float t02 = (origin2 - pos2) * invdir2;
  float t12 = (origin2 + sidelength - pos2) * invdir2;
  tminMax = fmax(tminMax, fmin(t02, t12));
  tmaxMin = fmin(tmaxMin, fmax(t02, t12));
   float t03 = (origin3 - pos3) * invdir3;
  float t13 = (origin3 + sidelength - pos3) * invdir3;
  tminMax = fmax(tminMax, fmin(t03, t13));
  tmaxMin = fmin(tmaxMin, fmax(t03, t13));
   float t04 = (origin4 - pos4) * invdir4;
  float t14 = (origin4 + sidelength - pos4) * invdir4;
  tminMax = fmax(tminMax, fmin(t04, t14));
  tmaxMin = fmin(tmaxMin, fmax(t04, t14));
   float t05 = (origin5 - pos5) * invdir5;
  float t15 = (origin5 + sidelength - pos5) * invdir5;
  tminMax = fmax(tminMax, fmin(t05, t15));
  tmaxMin = fmin(tmaxMin, fmax(t05, t15));
  
  if (tminMax >= tmaxMin) {
    return false;
  }

  float currDist = tminMax + 1e-3f;
   float currPos0 = pos0 + currDist * dir0;
   float currPos1 = pos1 + currDist * dir1;
   float currPos2 = pos2 + currDist * dir2;
   float currPos3 = pos3 + currDist * dir3;
   float currPos4 = pos4 + currDist * dir4;
   float currPos5 = pos5 + currDist * dir5;
  

  StackEntry6 stack[12];
  size_t stackInd = 0;
  StackEntry6 baseEntry;
  baseEntry.indexed = false;
  baseEntry.node = octree;
   baseEntry.middle0 = origin0 + sidelength / 2;
  baseEntry.farEnd0 = origin0 + (invdir0 > 0) * sidelength;
   baseEntry.middle1 = origin1 + sidelength / 2;
  baseEntry.farEnd1 = origin1 + (invdir1 > 0) * sidelength;
   baseEntry.middle2 = origin2 + sidelength / 2;
  baseEntry.farEnd2 = origin2 + (invdir2 > 0) * sidelength;
   baseEntry.middle3 = origin3 + sidelength / 2;
  baseEntry.farEnd3 = origin3 + (invdir3 > 0) * sidelength;
   baseEntry.middle4 = origin4 + sidelength / 2;
  baseEntry.farEnd4 = origin4 + (invdir4 > 0) * sidelength;
   baseEntry.middle5 = origin5 + sidelength / 2;
  baseEntry.farEnd5 = origin5 + (invdir5 > 0) * sidelength;
  
  stack[stackInd++] = baseEntry;
  bool debug = true;
  __global float *distp = dist;
  size_t i = 0;
  while (stackInd && i < 100) {
    i++;
    StackEntry6 entry = stack[--stackInd];
    /*
    if (debug) {
      printf("currDist: %f, entry.indexed: %i, entry.index: %lu\n", currDist,
             (int)entry.indexed, entry.index);
       printf(
          "currPos0: %f, entry.middle0: %f, entry.farEnd0: %f\n",
          currPos0, entry.middle0, entry.farEnd0);
       printf(
          "currPos1: %f, entry.middle1: %f, entry.farEnd1: %f\n",
          currPos1, entry.middle1, entry.farEnd1);
       printf(
          "currPos2: %f, entry.middle2: %f, entry.farEnd2: %f\n",
          currPos2, entry.middle2, entry.farEnd2);
       printf(
          "currPos3: %f, entry.middle3: %f, entry.farEnd3: %f\n",
          currPos3, entry.middle3, entry.farEnd3);
       printf(
          "currPos4: %f, entry.middle4: %f, entry.farEnd4: %f\n",
          currPos4, entry.middle4, entry.farEnd4);
       printf(
          "currPos5: %f, entry.middle5: %f, entry.farEnd5: %f\n",
          currPos5, entry.middle5, entry.farEnd5);
      
      if (*entry.node) {
        printf("Leaf node\n");
      } else {
        printf("\n");
      }
    }
    */
    /*
    *distp++ = currDist;
     *distp++ = currPos0;
    *distp++ = invdir0;
    *distp++ = entry.middle0;
    *distp++ = entry.farEnd0;
     *distp++ = currPos1;
    *distp++ = invdir1;
    *distp++ = entry.middle1;
    *distp++ = entry.farEnd1;
     *distp++ = currPos2;
    *distp++ = invdir2;
    *distp++ = entry.middle2;
    *distp++ = entry.farEnd2;
     *distp++ = currPos3;
    *distp++ = invdir3;
    *distp++ = entry.middle3;
    *distp++ = entry.farEnd3;
     *distp++ = currPos4;
    *distp++ = invdir4;
    *distp++ = entry.middle4;
    *distp++ = entry.farEnd4;
     *distp++ = currPos5;
    *distp++ = invdir5;
    *distp++ = entry.middle5;
    *distp++ = entry.farEnd5;
    
    *distp++ = *entry.node;
    *distp++ = stackInd;
    */
    if (*entry.node) {
      // this is a leaf node
      placeBehind(*(entry.node + 1), img);
      if (*(img + 3) == 0xFF) {
        *dist = currDist;
        return true;
      }
      float minStep = FLT_MAX;
       minStep =
          fmin(minStep, (entry.farEnd0 - currPos0) * invdir0);
       minStep =
          fmin(minStep, (entry.farEnd1 - currPos1) * invdir1);
       minStep =
          fmin(minStep, (entry.farEnd2 - currPos2) * invdir2);
       minStep =
          fmin(minStep, (entry.farEnd3 - currPos3) * invdir3);
       minStep =
          fmin(minStep, (entry.farEnd4 - currPos4) * invdir4);
       minStep =
          fmin(minStep, (entry.farEnd5 - currPos5) * invdir5);
      
      if (minStep <= -1e-3f) {
        continue;
      }
      currDist += minStep + 1e-3f;
       currPos0 = pos0 + currDist * dir0;
       currPos1 = pos1 + currDist * dir1;
       currPos2 = pos2 + currDist * dir2;
       currPos3 = pos3 + currDist * dir3;
       currPos4 = pos4 + currDist * dir4;
       currPos5 = pos5 + currDist * dir5;
      
    } else {
      size_t newIndex = (
                            ((currPos0 > entry.middle0) ||
                             (currPos0 == entry.middle0 && (invdir0 > 0)))
                            << 0) |
                        (
                            ((currPos1 > entry.middle1) ||
                             (currPos1 == entry.middle1 && (invdir1 > 0)))
                            << 1) |
                        (
                            ((currPos2 > entry.middle2) ||
                             (currPos2 == entry.middle2 && (invdir2 > 0)))
                            << 2) |
                        (
                            ((currPos3 > entry.middle3) ||
                             (currPos3 == entry.middle3 && (invdir3 > 0)))
                            << 3) |
                        (
                            ((currPos4 > entry.middle4) ||
                             (currPos4 == entry.middle4 && (invdir4 > 0)))
                            << 4) |
                        (
                            ((currPos5 > entry.middle5) ||
                             (currPos5 == entry.middle5 && (invdir5 > 0)))
                            << 5) |
                        
      0;
      if (entry.indexed) {
        size_t diffIndex = newIndex ^ entry.index;
        if (diffIndex) {
          entry.index = newIndex;
          if (((entry.farEnd0 - currPos0) * invdir0 >
                                 0) &&
                  ((entry.farEnd1 - currPos1) * invdir1 >
                                 0) &&
                  ((entry.farEnd2 - currPos2) * invdir2 >
                                 0) &&
                  ((entry.farEnd3 - currPos3) * invdir3 >
                                 0) &&
                  ((entry.farEnd4 - currPos4) * invdir4 >
                                 0) &&
                  ((entry.farEnd5 - currPos5) * invdir5 >
                                 0) &&
                  
              true) {
            stack[stackInd++] = entry;
          }
          size_t tmpIndex = newIndex;
          StackEntry6 child;
           if ((1 << 0) & diffIndex) {
            makeChild6(&entry, tmpIndex, &child);
            stack[stackInd++] = child;
            tmpIndex ^= (1 << 0);
          }
           if ((1 << 1) & diffIndex) {
            makeChild6(&entry, tmpIndex, &child);
            stack[stackInd++] = child;
            tmpIndex ^= (1 << 1);
          }
           if ((1 << 2) & diffIndex) {
            makeChild6(&entry, tmpIndex, &child);
            stack[stackInd++] = child;
            tmpIndex ^= (1 << 2);
          }
           if ((1 << 3) & diffIndex) {
            makeChild6(&entry, tmpIndex, &child);
            stack[stackInd++] = child;
            tmpIndex ^= (1 << 3);
          }
           if ((1 << 4) & diffIndex) {
            makeChild6(&entry, tmpIndex, &child);
            stack[stackInd++] = child;
            tmpIndex ^= (1 << 4);
          }
           if ((1 << 5) & diffIndex) {
            makeChild6(&entry, tmpIndex, &child);
            stack[stackInd++] = child;
            tmpIndex ^= (1 << 5);
          }
          
          continue;
        }
      }
      entry.index = newIndex;
      entry.indexed = true;
      if (((entry.farEnd0 - currPos0) * invdir0 > 0) &&
              ((entry.farEnd1 - currPos1) * invdir1 > 0) &&
              ((entry.farEnd2 - currPos2) * invdir2 > 0) &&
              ((entry.farEnd3 - currPos3) * invdir3 > 0) &&
              ((entry.farEnd4 - currPos4) * invdir4 > 0) &&
              ((entry.farEnd5 - currPos5) * invdir5 > 0) &&
              
          true) {
        stack[stackInd++] = entry;
      }
      StackEntry6 child;
      makeChild6(&entry, entry.index, &child);
      stack[stackInd++] = child;
    }
  }
  return false;
}

bool rayBBox6( float pos0, float invdir0,
                    float min0, float max0,  float pos1, float invdir1,
                    float min1, float max1,  float pos2, float invdir2,
                    float min2, float max2,  float pos3, float invdir3,
                    float min3, float max3,  float pos4, float invdir4,
                    float min4, float max4,  float pos5, float invdir5,
                    float min5, float max5, 
                    int unused) {
  (void)unused;
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
  
  return tminMax < tmaxMin;
}

bool traverseBVH6(__global uint *bvh,  float pos0,
                        float dir0, float invdir0,  float pos1,
                        float dir1, float invdir1,  float pos2,
                        float dir2, float invdir2,  float pos3,
                        float dir3, float invdir3,  float pos4,
                        float dir4, float invdir4,  float pos5,
                        float dir5, float invdir5, 
                        __global uchar * img, __global float *dist) {
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
             pos0, invdir0, min0, max0,  pos1, invdir1, min1, max1,  pos2, invdir2, min2, max2,  pos3, invdir3, min3, max3,  pos4, invdir4, min4, max4,  pos5, invdir5, min5, max5,  0)) {
      continue;
    }
    currBvh = (__global uint *)floatBVH;
    if (*currBvh++) {
      if (traverseOctree6(currBvh + *currBvh,  pos0,
                                dir0, invdir0,  pos1,
                                dir1, invdir1,  pos2,
                                dir2, invdir2,  pos3,
                                dir3, invdir3,  pos4,
                                dir4, invdir4,  pos5,
                                dir5, invdir5, 
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

__kernel void renderStd6(__global uint *bvh, __global float *pos,
                               __global float *forward, __global float *right,
                               __global float *up, __global uchar *img,
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
                     img, dist);
}

#ifndef NON_REP_
#define NON_REP_

uchar mergeColorChannel(uint back, int bitshift, uchar front, uchar frontAlpha, uchar combinedAlpha) {
  return (front * frontAlpha + ((back >> bitshift) & 0xFF) * (255 - frontAlpha) * (back & 0xFF) / 255.0f) / combinedAlpha;
}

void placeBehind(uint color, __global uchar *img) {
  uchar frontAlpha = *(img + 3);
  uchar combinedAlpha = frontAlpha + (color & 0xFF) * (255 - frontAlpha) / 255.0f;
  *img = mergeColorChannel(color, 24, *img, frontAlpha, combinedAlpha);
  img++;
  *img = mergeColorChannel(color, 16, *img, frontAlpha, combinedAlpha);
  img++;
  *img = mergeColorChannel(color, 8, *img, frontAlpha, combinedAlpha);
  img++;
  *img = combinedAlpha;
}

#endif // NON_REP_

/*
BVH format:

float minmax[7][2];
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

float origin[7];
float sidelength;
This is the sidelength of the root node.

The root node is immediately after the header.

Node:

int isLeaf;
If isLeaf, then the following immediately follows:

int color;

Otherwise, the following immediately follows:

int children[2 ** 7];
The index of the node with minimum coordinates is 0, while changing the 2 **
IND bit would change only the coordinate on axis IND.

Each value in children in equal to childPtr - children, where childPtr is a
pointer to the corresponding node.

*/
typedef struct StackEntry7 {

  bool indexed;
  __global uint *node;
  size_t index;
   float middle0;
  float farEnd0;
   float middle1;
  float farEnd1;
   float middle2;
  float farEnd2;
   float middle3;
  float farEnd3;
   float middle4;
  float farEnd4;
   float middle5;
  float farEnd5;
   float middle6;
  float farEnd6;
  
} StackEntry7;

void makeChild7(const StackEntry7 *parent, size_t index,
                      StackEntry7 *child) {
  child->indexed = false;
  child->node = parent->node + 1 + parent->node[index + 1];
   child->middle0 =
      1.5f * parent->middle0 - 0.5f * parent->farEnd0 +
      (((index >> 0) & 1) ^ (parent->farEnd0 < parent->middle0)) *
          (parent->farEnd0 - parent->middle0);
  child->farEnd0 =
      child->middle0 + (parent->farEnd0 - parent->middle0) / 2;
   child->middle1 =
      1.5f * parent->middle1 - 0.5f * parent->farEnd1 +
      (((index >> 1) & 1) ^ (parent->farEnd1 < parent->middle1)) *
          (parent->farEnd1 - parent->middle1);
  child->farEnd1 =
      child->middle1 + (parent->farEnd1 - parent->middle1) / 2;
   child->middle2 =
      1.5f * parent->middle2 - 0.5f * parent->farEnd2 +
      (((index >> 2) & 1) ^ (parent->farEnd2 < parent->middle2)) *
          (parent->farEnd2 - parent->middle2);
  child->farEnd2 =
      child->middle2 + (parent->farEnd2 - parent->middle2) / 2;
   child->middle3 =
      1.5f * parent->middle3 - 0.5f * parent->farEnd3 +
      (((index >> 3) & 1) ^ (parent->farEnd3 < parent->middle3)) *
          (parent->farEnd3 - parent->middle3);
  child->farEnd3 =
      child->middle3 + (parent->farEnd3 - parent->middle3) / 2;
   child->middle4 =
      1.5f * parent->middle4 - 0.5f * parent->farEnd4 +
      (((index >> 4) & 1) ^ (parent->farEnd4 < parent->middle4)) *
          (parent->farEnd4 - parent->middle4);
  child->farEnd4 =
      child->middle4 + (parent->farEnd4 - parent->middle4) / 2;
   child->middle5 =
      1.5f * parent->middle5 - 0.5f * parent->farEnd5 +
      (((index >> 5) & 1) ^ (parent->farEnd5 < parent->middle5)) *
          (parent->farEnd5 - parent->middle5);
  child->farEnd5 =
      child->middle5 + (parent->farEnd5 - parent->middle5) / 2;
   child->middle6 =
      1.5f * parent->middle6 - 0.5f * parent->farEnd6 +
      (((index >> 6) & 1) ^ (parent->farEnd6 < parent->middle6)) *
          (parent->farEnd6 - parent->middle6);
  child->farEnd6 =
      child->middle6 + (parent->farEnd6 - parent->middle6) / 2;
  
}

bool traverseOctree7(__global uint *octree,
                            float pos0, float dir0,
                           float invdir0,  float pos1, float dir1,
                           float invdir1,  float pos2, float dir2,
                           float invdir2,  float pos3, float dir3,
                           float invdir3,  float pos4, float dir4,
                           float invdir4,  float pos5, float dir5,
                           float invdir5,  float pos6, float dir6,
                           float invdir6, 
                           __global uchar * img, __global float *dist) {
  __global float *header = (__global float *)octree;
   float origin0 = *header++;
   float origin1 = *header++;
   float origin2 = *header++;
   float origin3 = *header++;
   float origin4 = *header++;
   float origin5 = *header++;
   float origin6 = *header++;
  
  float sidelength = *header++;
  octree = (__global uint *)header;

  float tminMax = 1e-8f;
  float tmaxMin = FLT_MAX;
   float t00 = (origin0 - pos0) * invdir0;
  float t10 = (origin0 + sidelength - pos0) * invdir0;
  tminMax = fmax(tminMax, fmin(t00, t10));
  tmaxMin = fmin(tmaxMin, fmax(t00, t10));
   float t01 = (origin1 - pos1) * invdir1;
  float t11 = (origin1 + sidelength - pos1) * invdir1;
  tminMax = fmax(tminMax, fmin(t01, t11));
  tmaxMin = fmin(tmaxMin, fmax(t01, t11));
   float t02 = (origin2 - pos2) * invdir2;
  float t12 = (origin2 + sidelength - pos2) * invdir2;
  tminMax = fmax(tminMax, fmin(t02, t12));
  tmaxMin = fmin(tmaxMin, fmax(t02, t12));
   float t03 = (origin3 - pos3) * invdir3;
  float t13 = (origin3 + sidelength - pos3) * invdir3;
  tminMax = fmax(tminMax, fmin(t03, t13));
  tmaxMin = fmin(tmaxMin, fmax(t03, t13));
   float t04 = (origin4 - pos4) * invdir4;
  float t14 = (origin4 + sidelength - pos4) * invdir4;
  tminMax = fmax(tminMax, fmin(t04, t14));
  tmaxMin = fmin(tmaxMin, fmax(t04, t14));
   float t05 = (origin5 - pos5) * invdir5;
  float t15 = (origin5 + sidelength - pos5) * invdir5;
  tminMax = fmax(tminMax, fmin(t05, t15));
  tmaxMin = fmin(tmaxMin, fmax(t05, t15));
   float t06 = (origin6 - pos6) * invdir6;
  float t16 = (origin6 + sidelength - pos6) * invdir6;
  tminMax = fmax(tminMax, fmin(t06, t16));
  tmaxMin = fmin(tmaxMin, fmax(t06, t16));
  
  if (tminMax >= tmaxMin) {
    return false;
  }

  float currDist = tminMax + 1e-3f;
   float currPos0 = pos0 + currDist * dir0;
   float currPos1 = pos1 + currDist * dir1;
   float currPos2 = pos2 + currDist * dir2;
   float currPos3 = pos3 + currDist * dir3;
   float currPos4 = pos4 + currDist * dir4;
   float currPos5 = pos5 + currDist * dir5;
   float currPos6 = pos6 + currDist * dir6;
  

  StackEntry7 stack[12];
  size_t stackInd = 0;
  StackEntry7 baseEntry;
  baseEntry.indexed = false;
  baseEntry.node = octree;
   baseEntry.middle0 = origin0 + sidelength / 2;
  baseEntry.farEnd0 = origin0 + (invdir0 > 0) * sidelength;
   baseEntry.middle1 = origin1 + sidelength / 2;
  baseEntry.farEnd1 = origin1 + (invdir1 > 0) * sidelength;
   baseEntry.middle2 = origin2 + sidelength / 2;
  baseEntry.farEnd2 = origin2 + (invdir2 > 0) * sidelength;
   baseEntry.middle3 = origin3 + sidelength / 2;
  baseEntry.farEnd3 = origin3 + (invdir3 > 0) * sidelength;
   baseEntry.middle4 = origin4 + sidelength / 2;
  baseEntry.farEnd4 = origin4 + (invdir4 > 0) * sidelength;
   baseEntry.middle5 = origin5 + sidelength / 2;
  baseEntry.farEnd5 = origin5 + (invdir5 > 0) * sidelength;
   baseEntry.middle6 = origin6 + sidelength / 2;
  baseEntry.farEnd6 = origin6 + (invdir6 > 0) * sidelength;
  
  stack[stackInd++] = baseEntry;
  bool debug = true;
  __global float *distp = dist;
  size_t i = 0;
  while (stackInd && i < 100) {
    i++;
    StackEntry7 entry = stack[--stackInd];
    /*
    if (debug) {
      printf("currDist: %f, entry.indexed: %i, entry.index: %lu\n", currDist,
             (int)entry.indexed, entry.index);
       printf(
          "currPos0: %f, entry.middle0: %f, entry.farEnd0: %f\n",
          currPos0, entry.middle0, entry.farEnd0);
       printf(
          "currPos1: %f, entry.middle1: %f, entry.farEnd1: %f\n",
          currPos1, entry.middle1, entry.farEnd1);
       printf(
          "currPos2: %f, entry.middle2: %f, entry.farEnd2: %f\n",
          currPos2, entry.middle2, entry.farEnd2);
       printf(
          "currPos3: %f, entry.middle3: %f, entry.farEnd3: %f\n",
          currPos3, entry.middle3, entry.farEnd3);
       printf(
          "currPos4: %f, entry.middle4: %f, entry.farEnd4: %f\n",
          currPos4, entry.middle4, entry.farEnd4);
       printf(
          "currPos5: %f, entry.middle5: %f, entry.farEnd5: %f\n",
          currPos5, entry.middle5, entry.farEnd5);
       printf(
          "currPos6: %f, entry.middle6: %f, entry.farEnd6: %f\n",
          currPos6, entry.middle6, entry.farEnd6);
      
      if (*entry.node) {
        printf("Leaf node\n");
      } else {
        printf("\n");
      }
    }
    */
    /*
    *distp++ = currDist;
     *distp++ = currPos0;
    *distp++ = invdir0;
    *distp++ = entry.middle0;
    *distp++ = entry.farEnd0;
     *distp++ = currPos1;
    *distp++ = invdir1;
    *distp++ = entry.middle1;
    *distp++ = entry.farEnd1;
     *distp++ = currPos2;
    *distp++ = invdir2;
    *distp++ = entry.middle2;
    *distp++ = entry.farEnd2;
     *distp++ = currPos3;
    *distp++ = invdir3;
    *distp++ = entry.middle3;
    *distp++ = entry.farEnd3;
     *distp++ = currPos4;
    *distp++ = invdir4;
    *distp++ = entry.middle4;
    *distp++ = entry.farEnd4;
     *distp++ = currPos5;
    *distp++ = invdir5;
    *distp++ = entry.middle5;
    *distp++ = entry.farEnd5;
     *distp++ = currPos6;
    *distp++ = invdir6;
    *distp++ = entry.middle6;
    *distp++ = entry.farEnd6;
    
    *distp++ = *entry.node;
    *distp++ = stackInd;
    */
    if (*entry.node) {
      // this is a leaf node
      placeBehind(*(entry.node + 1), img);
      if (*(img + 3) == 0xFF) {
        *dist = currDist;
        return true;
      }
      float minStep = FLT_MAX;
       minStep =
          fmin(minStep, (entry.farEnd0 - currPos0) * invdir0);
       minStep =
          fmin(minStep, (entry.farEnd1 - currPos1) * invdir1);
       minStep =
          fmin(minStep, (entry.farEnd2 - currPos2) * invdir2);
       minStep =
          fmin(minStep, (entry.farEnd3 - currPos3) * invdir3);
       minStep =
          fmin(minStep, (entry.farEnd4 - currPos4) * invdir4);
       minStep =
          fmin(minStep, (entry.farEnd5 - currPos5) * invdir5);
       minStep =
          fmin(minStep, (entry.farEnd6 - currPos6) * invdir6);
      
      if (minStep <= -1e-3f) {
        continue;
      }
      currDist += minStep + 1e-3f;
       currPos0 = pos0 + currDist * dir0;
       currPos1 = pos1 + currDist * dir1;
       currPos2 = pos2 + currDist * dir2;
       currPos3 = pos3 + currDist * dir3;
       currPos4 = pos4 + currDist * dir4;
       currPos5 = pos5 + currDist * dir5;
       currPos6 = pos6 + currDist * dir6;
      
    } else {
      size_t newIndex = (
                            ((currPos0 > entry.middle0) ||
                             (currPos0 == entry.middle0 && (invdir0 > 0)))
                            << 0) |
                        (
                            ((currPos1 > entry.middle1) ||
                             (currPos1 == entry.middle1 && (invdir1 > 0)))
                            << 1) |
                        (
                            ((currPos2 > entry.middle2) ||
                             (currPos2 == entry.middle2 && (invdir2 > 0)))
                            << 2) |
                        (
                            ((currPos3 > entry.middle3) ||
                             (currPos3 == entry.middle3 && (invdir3 > 0)))
                            << 3) |
                        (
                            ((currPos4 > entry.middle4) ||
                             (currPos4 == entry.middle4 && (invdir4 > 0)))
                            << 4) |
                        (
                            ((currPos5 > entry.middle5) ||
                             (currPos5 == entry.middle5 && (invdir5 > 0)))
                            << 5) |
                        (
                            ((currPos6 > entry.middle6) ||
                             (currPos6 == entry.middle6 && (invdir6 > 0)))
                            << 6) |
                        
      0;
      if (entry.indexed) {
        size_t diffIndex = newIndex ^ entry.index;
        if (diffIndex) {
          entry.index = newIndex;
          if (((entry.farEnd0 - currPos0) * invdir0 >
                                 0) &&
                  ((entry.farEnd1 - currPos1) * invdir1 >
                                 0) &&
                  ((entry.farEnd2 - currPos2) * invdir2 >
                                 0) &&
                  ((entry.farEnd3 - currPos3) * invdir3 >
                                 0) &&
                  ((entry.farEnd4 - currPos4) * invdir4 >
                                 0) &&
                  ((entry.farEnd5 - currPos5) * invdir5 >
                                 0) &&
                  ((entry.farEnd6 - currPos6) * invdir6 >
                                 0) &&
                  
              true) {
            stack[stackInd++] = entry;
          }
          size_t tmpIndex = newIndex;
          StackEntry7 child;
           if ((1 << 0) & diffIndex) {
            makeChild7(&entry, tmpIndex, &child);
            stack[stackInd++] = child;
            tmpIndex ^= (1 << 0);
          }
           if ((1 << 1) & diffIndex) {
            makeChild7(&entry, tmpIndex, &child);
            stack[stackInd++] = child;
            tmpIndex ^= (1 << 1);
          }
           if ((1 << 2) & diffIndex) {
            makeChild7(&entry, tmpIndex, &child);
            stack[stackInd++] = child;
            tmpIndex ^= (1 << 2);
          }
           if ((1 << 3) & diffIndex) {
            makeChild7(&entry, tmpIndex, &child);
            stack[stackInd++] = child;
            tmpIndex ^= (1 << 3);
          }
           if ((1 << 4) & diffIndex) {
            makeChild7(&entry, tmpIndex, &child);
            stack[stackInd++] = child;
            tmpIndex ^= (1 << 4);
          }
           if ((1 << 5) & diffIndex) {
            makeChild7(&entry, tmpIndex, &child);
            stack[stackInd++] = child;
            tmpIndex ^= (1 << 5);
          }
           if ((1 << 6) & diffIndex) {
            makeChild7(&entry, tmpIndex, &child);
            stack[stackInd++] = child;
            tmpIndex ^= (1 << 6);
          }
          
          continue;
        }
      }
      entry.index = newIndex;
      entry.indexed = true;
      if (((entry.farEnd0 - currPos0) * invdir0 > 0) &&
              ((entry.farEnd1 - currPos1) * invdir1 > 0) &&
              ((entry.farEnd2 - currPos2) * invdir2 > 0) &&
              ((entry.farEnd3 - currPos3) * invdir3 > 0) &&
              ((entry.farEnd4 - currPos4) * invdir4 > 0) &&
              ((entry.farEnd5 - currPos5) * invdir5 > 0) &&
              ((entry.farEnd6 - currPos6) * invdir6 > 0) &&
              
          true) {
        stack[stackInd++] = entry;
      }
      StackEntry7 child;
      makeChild7(&entry, entry.index, &child);
      stack[stackInd++] = child;
    }
  }
  return false;
}

bool rayBBox7( float pos0, float invdir0,
                    float min0, float max0,  float pos1, float invdir1,
                    float min1, float max1,  float pos2, float invdir2,
                    float min2, float max2,  float pos3, float invdir3,
                    float min3, float max3,  float pos4, float invdir4,
                    float min4, float max4,  float pos5, float invdir5,
                    float min5, float max5,  float pos6, float invdir6,
                    float min6, float max6, 
                    int unused) {
  (void)unused;
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
  
  return tminMax < tmaxMin;
}

bool traverseBVH7(__global uint *bvh,  float pos0,
                        float dir0, float invdir0,  float pos1,
                        float dir1, float invdir1,  float pos2,
                        float dir2, float invdir2,  float pos3,
                        float dir3, float invdir3,  float pos4,
                        float dir4, float invdir4,  float pos5,
                        float dir5, float invdir5,  float pos6,
                        float dir6, float invdir6, 
                        __global uchar * img, __global float *dist) {
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
             pos0, invdir0, min0, max0,  pos1, invdir1, min1, max1,  pos2, invdir2, min2, max2,  pos3, invdir3, min3, max3,  pos4, invdir4, min4, max4,  pos5, invdir5, min5, max5,  pos6, invdir6, min6, max6,  0)) {
      continue;
    }
    currBvh = (__global uint *)floatBVH;
    if (*currBvh++) {
      if (traverseOctree7(currBvh + *currBvh,  pos0,
                                dir0, invdir0,  pos1,
                                dir1, invdir1,  pos2,
                                dir2, invdir2,  pos3,
                                dir3, invdir3,  pos4,
                                dir4, invdir4,  pos5,
                                dir5, invdir5,  pos6,
                                dir6, invdir6, 
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

__kernel void renderStd7(__global uint *bvh, __global float *pos,
                               __global float *forward, __global float *right,
                               __global float *up, __global uchar *img,
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
                     img, dist);
}

#ifndef NON_REP_
#define NON_REP_

uchar mergeColorChannel(uint back, int bitshift, uchar front, uchar frontAlpha, uchar combinedAlpha) {
  return (front * frontAlpha + ((back >> bitshift) & 0xFF) * (255 - frontAlpha) * (back & 0xFF) / 255.0f) / combinedAlpha;
}

void placeBehind(uint color, __global uchar *img) {
  uchar frontAlpha = *(img + 3);
  uchar combinedAlpha = frontAlpha + (color & 0xFF) * (255 - frontAlpha) / 255.0f;
  *img = mergeColorChannel(color, 24, *img, frontAlpha, combinedAlpha);
  img++;
  *img = mergeColorChannel(color, 16, *img, frontAlpha, combinedAlpha);
  img++;
  *img = mergeColorChannel(color, 8, *img, frontAlpha, combinedAlpha);
  img++;
  *img = combinedAlpha;
}

#endif // NON_REP_

/*
BVH format:

float minmax[8][2];
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

float origin[8];
float sidelength;
This is the sidelength of the root node.

The root node is immediately after the header.

Node:

int isLeaf;
If isLeaf, then the following immediately follows:

int color;

Otherwise, the following immediately follows:

int children[2 ** 8];
The index of the node with minimum coordinates is 0, while changing the 2 **
IND bit would change only the coordinate on axis IND.

Each value in children in equal to childPtr - children, where childPtr is a
pointer to the corresponding node.

*/
typedef struct StackEntry8 {

  bool indexed;
  __global uint *node;
  size_t index;
   float middle0;
  float farEnd0;
   float middle1;
  float farEnd1;
   float middle2;
  float farEnd2;
   float middle3;
  float farEnd3;
   float middle4;
  float farEnd4;
   float middle5;
  float farEnd5;
   float middle6;
  float farEnd6;
   float middle7;
  float farEnd7;
  
} StackEntry8;

void makeChild8(const StackEntry8 *parent, size_t index,
                      StackEntry8 *child) {
  child->indexed = false;
  child->node = parent->node + 1 + parent->node[index + 1];
   child->middle0 =
      1.5f * parent->middle0 - 0.5f * parent->farEnd0 +
      (((index >> 0) & 1) ^ (parent->farEnd0 < parent->middle0)) *
          (parent->farEnd0 - parent->middle0);
  child->farEnd0 =
      child->middle0 + (parent->farEnd0 - parent->middle0) / 2;
   child->middle1 =
      1.5f * parent->middle1 - 0.5f * parent->farEnd1 +
      (((index >> 1) & 1) ^ (parent->farEnd1 < parent->middle1)) *
          (parent->farEnd1 - parent->middle1);
  child->farEnd1 =
      child->middle1 + (parent->farEnd1 - parent->middle1) / 2;
   child->middle2 =
      1.5f * parent->middle2 - 0.5f * parent->farEnd2 +
      (((index >> 2) & 1) ^ (parent->farEnd2 < parent->middle2)) *
          (parent->farEnd2 - parent->middle2);
  child->farEnd2 =
      child->middle2 + (parent->farEnd2 - parent->middle2) / 2;
   child->middle3 =
      1.5f * parent->middle3 - 0.5f * parent->farEnd3 +
      (((index >> 3) & 1) ^ (parent->farEnd3 < parent->middle3)) *
          (parent->farEnd3 - parent->middle3);
  child->farEnd3 =
      child->middle3 + (parent->farEnd3 - parent->middle3) / 2;
   child->middle4 =
      1.5f * parent->middle4 - 0.5f * parent->farEnd4 +
      (((index >> 4) & 1) ^ (parent->farEnd4 < parent->middle4)) *
          (parent->farEnd4 - parent->middle4);
  child->farEnd4 =
      child->middle4 + (parent->farEnd4 - parent->middle4) / 2;
   child->middle5 =
      1.5f * parent->middle5 - 0.5f * parent->farEnd5 +
      (((index >> 5) & 1) ^ (parent->farEnd5 < parent->middle5)) *
          (parent->farEnd5 - parent->middle5);
  child->farEnd5 =
      child->middle5 + (parent->farEnd5 - parent->middle5) / 2;
   child->middle6 =
      1.5f * parent->middle6 - 0.5f * parent->farEnd6 +
      (((index >> 6) & 1) ^ (parent->farEnd6 < parent->middle6)) *
          (parent->farEnd6 - parent->middle6);
  child->farEnd6 =
      child->middle6 + (parent->farEnd6 - parent->middle6) / 2;
   child->middle7 =
      1.5f * parent->middle7 - 0.5f * parent->farEnd7 +
      (((index >> 7) & 1) ^ (parent->farEnd7 < parent->middle7)) *
          (parent->farEnd7 - parent->middle7);
  child->farEnd7 =
      child->middle7 + (parent->farEnd7 - parent->middle7) / 2;
  
}

bool traverseOctree8(__global uint *octree,
                            float pos0, float dir0,
                           float invdir0,  float pos1, float dir1,
                           float invdir1,  float pos2, float dir2,
                           float invdir2,  float pos3, float dir3,
                           float invdir3,  float pos4, float dir4,
                           float invdir4,  float pos5, float dir5,
                           float invdir5,  float pos6, float dir6,
                           float invdir6,  float pos7, float dir7,
                           float invdir7, 
                           __global uchar * img, __global float *dist) {
  __global float *header = (__global float *)octree;
   float origin0 = *header++;
   float origin1 = *header++;
   float origin2 = *header++;
   float origin3 = *header++;
   float origin4 = *header++;
   float origin5 = *header++;
   float origin6 = *header++;
   float origin7 = *header++;
  
  float sidelength = *header++;
  octree = (__global uint *)header;

  float tminMax = 1e-8f;
  float tmaxMin = FLT_MAX;
   float t00 = (origin0 - pos0) * invdir0;
  float t10 = (origin0 + sidelength - pos0) * invdir0;
  tminMax = fmax(tminMax, fmin(t00, t10));
  tmaxMin = fmin(tmaxMin, fmax(t00, t10));
   float t01 = (origin1 - pos1) * invdir1;
  float t11 = (origin1 + sidelength - pos1) * invdir1;
  tminMax = fmax(tminMax, fmin(t01, t11));
  tmaxMin = fmin(tmaxMin, fmax(t01, t11));
   float t02 = (origin2 - pos2) * invdir2;
  float t12 = (origin2 + sidelength - pos2) * invdir2;
  tminMax = fmax(tminMax, fmin(t02, t12));
  tmaxMin = fmin(tmaxMin, fmax(t02, t12));
   float t03 = (origin3 - pos3) * invdir3;
  float t13 = (origin3 + sidelength - pos3) * invdir3;
  tminMax = fmax(tminMax, fmin(t03, t13));
  tmaxMin = fmin(tmaxMin, fmax(t03, t13));
   float t04 = (origin4 - pos4) * invdir4;
  float t14 = (origin4 + sidelength - pos4) * invdir4;
  tminMax = fmax(tminMax, fmin(t04, t14));
  tmaxMin = fmin(tmaxMin, fmax(t04, t14));
   float t05 = (origin5 - pos5) * invdir5;
  float t15 = (origin5 + sidelength - pos5) * invdir5;
  tminMax = fmax(tminMax, fmin(t05, t15));
  tmaxMin = fmin(tmaxMin, fmax(t05, t15));
   float t06 = (origin6 - pos6) * invdir6;
  float t16 = (origin6 + sidelength - pos6) * invdir6;
  tminMax = fmax(tminMax, fmin(t06, t16));
  tmaxMin = fmin(tmaxMin, fmax(t06, t16));
   float t07 = (origin7 - pos7) * invdir7;
  float t17 = (origin7 + sidelength - pos7) * invdir7;
  tminMax = fmax(tminMax, fmin(t07, t17));
  tmaxMin = fmin(tmaxMin, fmax(t07, t17));
  
  if (tminMax >= tmaxMin) {
    return false;
  }

  float currDist = tminMax + 1e-3f;
   float currPos0 = pos0 + currDist * dir0;
   float currPos1 = pos1 + currDist * dir1;
   float currPos2 = pos2 + currDist * dir2;
   float currPos3 = pos3 + currDist * dir3;
   float currPos4 = pos4 + currDist * dir4;
   float currPos5 = pos5 + currDist * dir5;
   float currPos6 = pos6 + currDist * dir6;
   float currPos7 = pos7 + currDist * dir7;
  

  StackEntry8 stack[12];
  size_t stackInd = 0;
  StackEntry8 baseEntry;
  baseEntry.indexed = false;
  baseEntry.node = octree;
   baseEntry.middle0 = origin0 + sidelength / 2;
  baseEntry.farEnd0 = origin0 + (invdir0 > 0) * sidelength;
   baseEntry.middle1 = origin1 + sidelength / 2;
  baseEntry.farEnd1 = origin1 + (invdir1 > 0) * sidelength;
   baseEntry.middle2 = origin2 + sidelength / 2;
  baseEntry.farEnd2 = origin2 + (invdir2 > 0) * sidelength;
   baseEntry.middle3 = origin3 + sidelength / 2;
  baseEntry.farEnd3 = origin3 + (invdir3 > 0) * sidelength;
   baseEntry.middle4 = origin4 + sidelength / 2;
  baseEntry.farEnd4 = origin4 + (invdir4 > 0) * sidelength;
   baseEntry.middle5 = origin5 + sidelength / 2;
  baseEntry.farEnd5 = origin5 + (invdir5 > 0) * sidelength;
   baseEntry.middle6 = origin6 + sidelength / 2;
  baseEntry.farEnd6 = origin6 + (invdir6 > 0) * sidelength;
   baseEntry.middle7 = origin7 + sidelength / 2;
  baseEntry.farEnd7 = origin7 + (invdir7 > 0) * sidelength;
  
  stack[stackInd++] = baseEntry;
  bool debug = true;
  __global float *distp = dist;
  size_t i = 0;
  while (stackInd && i < 100) {
    i++;
    StackEntry8 entry = stack[--stackInd];
    /*
    if (debug) {
      printf("currDist: %f, entry.indexed: %i, entry.index: %lu\n", currDist,
             (int)entry.indexed, entry.index);
       printf(
          "currPos0: %f, entry.middle0: %f, entry.farEnd0: %f\n",
          currPos0, entry.middle0, entry.farEnd0);
       printf(
          "currPos1: %f, entry.middle1: %f, entry.farEnd1: %f\n",
          currPos1, entry.middle1, entry.farEnd1);
       printf(
          "currPos2: %f, entry.middle2: %f, entry.farEnd2: %f\n",
          currPos2, entry.middle2, entry.farEnd2);
       printf(
          "currPos3: %f, entry.middle3: %f, entry.farEnd3: %f\n",
          currPos3, entry.middle3, entry.farEnd3);
       printf(
          "currPos4: %f, entry.middle4: %f, entry.farEnd4: %f\n",
          currPos4, entry.middle4, entry.farEnd4);
       printf(
          "currPos5: %f, entry.middle5: %f, entry.farEnd5: %f\n",
          currPos5, entry.middle5, entry.farEnd5);
       printf(
          "currPos6: %f, entry.middle6: %f, entry.farEnd6: %f\n",
          currPos6, entry.middle6, entry.farEnd6);
       printf(
          "currPos7: %f, entry.middle7: %f, entry.farEnd7: %f\n",
          currPos7, entry.middle7, entry.farEnd7);
      
      if (*entry.node) {
        printf("Leaf node\n");
      } else {
        printf("\n");
      }
    }
    */
    /*
    *distp++ = currDist;
     *distp++ = currPos0;
    *distp++ = invdir0;
    *distp++ = entry.middle0;
    *distp++ = entry.farEnd0;
     *distp++ = currPos1;
    *distp++ = invdir1;
    *distp++ = entry.middle1;
    *distp++ = entry.farEnd1;
     *distp++ = currPos2;
    *distp++ = invdir2;
    *distp++ = entry.middle2;
    *distp++ = entry.farEnd2;
     *distp++ = currPos3;
    *distp++ = invdir3;
    *distp++ = entry.middle3;
    *distp++ = entry.farEnd3;
     *distp++ = currPos4;
    *distp++ = invdir4;
    *distp++ = entry.middle4;
    *distp++ = entry.farEnd4;
     *distp++ = currPos5;
    *distp++ = invdir5;
    *distp++ = entry.middle5;
    *distp++ = entry.farEnd5;
     *distp++ = currPos6;
    *distp++ = invdir6;
    *distp++ = entry.middle6;
    *distp++ = entry.farEnd6;
     *distp++ = currPos7;
    *distp++ = invdir7;
    *distp++ = entry.middle7;
    *distp++ = entry.farEnd7;
    
    *distp++ = *entry.node;
    *distp++ = stackInd;
    */
    if (*entry.node) {
      // this is a leaf node
      placeBehind(*(entry.node + 1), img);
      if (*(img + 3) == 0xFF) {
        *dist = currDist;
        return true;
      }
      float minStep = FLT_MAX;
       minStep =
          fmin(minStep, (entry.farEnd0 - currPos0) * invdir0);
       minStep =
          fmin(minStep, (entry.farEnd1 - currPos1) * invdir1);
       minStep =
          fmin(minStep, (entry.farEnd2 - currPos2) * invdir2);
       minStep =
          fmin(minStep, (entry.farEnd3 - currPos3) * invdir3);
       minStep =
          fmin(minStep, (entry.farEnd4 - currPos4) * invdir4);
       minStep =
          fmin(minStep, (entry.farEnd5 - currPos5) * invdir5);
       minStep =
          fmin(minStep, (entry.farEnd6 - currPos6) * invdir6);
       minStep =
          fmin(minStep, (entry.farEnd7 - currPos7) * invdir7);
      
      if (minStep <= -1e-3f) {
        continue;
      }
      currDist += minStep + 1e-3f;
       currPos0 = pos0 + currDist * dir0;
       currPos1 = pos1 + currDist * dir1;
       currPos2 = pos2 + currDist * dir2;
       currPos3 = pos3 + currDist * dir3;
       currPos4 = pos4 + currDist * dir4;
       currPos5 = pos5 + currDist * dir5;
       currPos6 = pos6 + currDist * dir6;
       currPos7 = pos7 + currDist * dir7;
      
    } else {
      size_t newIndex = (
                            ((currPos0 > entry.middle0) ||
                             (currPos0 == entry.middle0 && (invdir0 > 0)))
                            << 0) |
                        (
                            ((currPos1 > entry.middle1) ||
                             (currPos1 == entry.middle1 && (invdir1 > 0)))
                            << 1) |
                        (
                            ((currPos2 > entry.middle2) ||
                             (currPos2 == entry.middle2 && (invdir2 > 0)))
                            << 2) |
                        (
                            ((currPos3 > entry.middle3) ||
                             (currPos3 == entry.middle3 && (invdir3 > 0)))
                            << 3) |
                        (
                            ((currPos4 > entry.middle4) ||
                             (currPos4 == entry.middle4 && (invdir4 > 0)))
                            << 4) |
                        (
                            ((currPos5 > entry.middle5) ||
                             (currPos5 == entry.middle5 && (invdir5 > 0)))
                            << 5) |
                        (
                            ((currPos6 > entry.middle6) ||
                             (currPos6 == entry.middle6 && (invdir6 > 0)))
                            << 6) |
                        (
                            ((currPos7 > entry.middle7) ||
                             (currPos7 == entry.middle7 && (invdir7 > 0)))
                            << 7) |
                        
      0;
      if (entry.indexed) {
        size_t diffIndex = newIndex ^ entry.index;
        if (diffIndex) {
          entry.index = newIndex;
          if (((entry.farEnd0 - currPos0) * invdir0 >
                                 0) &&
                  ((entry.farEnd1 - currPos1) * invdir1 >
                                 0) &&
                  ((entry.farEnd2 - currPos2) * invdir2 >
                                 0) &&
                  ((entry.farEnd3 - currPos3) * invdir3 >
                                 0) &&
                  ((entry.farEnd4 - currPos4) * invdir4 >
                                 0) &&
                  ((entry.farEnd5 - currPos5) * invdir5 >
                                 0) &&
                  ((entry.farEnd6 - currPos6) * invdir6 >
                                 0) &&
                  ((entry.farEnd7 - currPos7) * invdir7 >
                                 0) &&
                  
              true) {
            stack[stackInd++] = entry;
          }
          size_t tmpIndex = newIndex;
          StackEntry8 child;
           if ((1 << 0) & diffIndex) {
            makeChild8(&entry, tmpIndex, &child);
            stack[stackInd++] = child;
            tmpIndex ^= (1 << 0);
          }
           if ((1 << 1) & diffIndex) {
            makeChild8(&entry, tmpIndex, &child);
            stack[stackInd++] = child;
            tmpIndex ^= (1 << 1);
          }
           if ((1 << 2) & diffIndex) {
            makeChild8(&entry, tmpIndex, &child);
            stack[stackInd++] = child;
            tmpIndex ^= (1 << 2);
          }
           if ((1 << 3) & diffIndex) {
            makeChild8(&entry, tmpIndex, &child);
            stack[stackInd++] = child;
            tmpIndex ^= (1 << 3);
          }
           if ((1 << 4) & diffIndex) {
            makeChild8(&entry, tmpIndex, &child);
            stack[stackInd++] = child;
            tmpIndex ^= (1 << 4);
          }
           if ((1 << 5) & diffIndex) {
            makeChild8(&entry, tmpIndex, &child);
            stack[stackInd++] = child;
            tmpIndex ^= (1 << 5);
          }
           if ((1 << 6) & diffIndex) {
            makeChild8(&entry, tmpIndex, &child);
            stack[stackInd++] = child;
            tmpIndex ^= (1 << 6);
          }
           if ((1 << 7) & diffIndex) {
            makeChild8(&entry, tmpIndex, &child);
            stack[stackInd++] = child;
            tmpIndex ^= (1 << 7);
          }
          
          continue;
        }
      }
      entry.index = newIndex;
      entry.indexed = true;
      if (((entry.farEnd0 - currPos0) * invdir0 > 0) &&
              ((entry.farEnd1 - currPos1) * invdir1 > 0) &&
              ((entry.farEnd2 - currPos2) * invdir2 > 0) &&
              ((entry.farEnd3 - currPos3) * invdir3 > 0) &&
              ((entry.farEnd4 - currPos4) * invdir4 > 0) &&
              ((entry.farEnd5 - currPos5) * invdir5 > 0) &&
              ((entry.farEnd6 - currPos6) * invdir6 > 0) &&
              ((entry.farEnd7 - currPos7) * invdir7 > 0) &&
              
          true) {
        stack[stackInd++] = entry;
      }
      StackEntry8 child;
      makeChild8(&entry, entry.index, &child);
      stack[stackInd++] = child;
    }
  }
  return false;
}

bool rayBBox8( float pos0, float invdir0,
                    float min0, float max0,  float pos1, float invdir1,
                    float min1, float max1,  float pos2, float invdir2,
                    float min2, float max2,  float pos3, float invdir3,
                    float min3, float max3,  float pos4, float invdir4,
                    float min4, float max4,  float pos5, float invdir5,
                    float min5, float max5,  float pos6, float invdir6,
                    float min6, float max6,  float pos7, float invdir7,
                    float min7, float max7, 
                    int unused) {
  (void)unused;
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
  
  return tminMax < tmaxMin;
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
                        __global uchar * img, __global float *dist) {
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
             pos0, invdir0, min0, max0,  pos1, invdir1, min1, max1,  pos2, invdir2, min2, max2,  pos3, invdir3, min3, max3,  pos4, invdir4, min4, max4,  pos5, invdir5, min5, max5,  pos6, invdir6, min6, max6,  pos7, invdir7, min7, max7,  0)) {
      continue;
    }
    currBvh = (__global uint *)floatBVH;
    if (*currBvh++) {
      if (traverseOctree8(currBvh + *currBvh,  pos0,
                                dir0, invdir0,  pos1,
                                dir1, invdir1,  pos2,
                                dir2, invdir2,  pos3,
                                dir3, invdir3,  pos4,
                                dir4, invdir4,  pos5,
                                dir5, invdir5,  pos6,
                                dir6, invdir6,  pos7,
                                dir7, invdir7, 
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

__kernel void renderStd8(__global uint *bvh, __global float *pos,
                               __global float *forward, __global float *right,
                               __global float *up, __global uchar *img,
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
                     img, dist);
}

)OPENCLPP"
