changequote(`"', `"')
changequote("{", "}")
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
define({KERNELS},
{
  bool traverseOctreeNUMDIMS(
      __read_only int *octree,
      REPEAT(NUMDIMS)(float posIND, float dirIND, float invdirIND, );
      unsigned int *img, __write_only float *dist) {
    float *header = (float *)octree;
    REPEAT(NUMDIMS)(float originIND = *header++;);
    float sidelength = *header++;
    octree = (int *)header;

    float tminMax = 1e-8;
    float tmaxMin = FLT_MAX;
    REPEAT(NUMDIMS)
    (float t0IND = (originIND - posIND) * invdirIND;
     float t1IND = (originIND + sidelength - posIND) * invdirIND;
     tminMax = max(tminMax, min(t0IND, t1IND));
     tmaxMin = min(tmaxMin, max(t0IND, t1IND)););
    if (tminMax >= tmaxMin) {
      return false;
    }

    float currDist = tminMax + 1e-8;
    REPEAT(NUMDIMS)
    (float currPosIND = posIND + currDist * dirIND;
     float absinvdirIND = fabs(invdirIND); float dirsignIND = invdirIND > 0;
     float farCornerIND = originIND + dirsignIND * sidelength;);

    struct StackEntry {

      bool processed;
      int *node;
      size_t index;
      REPEAT(NUMDIMS)
      (float middleIND; float farEndIND;);
    } * stack[1024];
    size_t stackInd = 0;
    StackEntry baseEntry;
    baseEntry.processed = false;
    baseEntry.node = octree;
    REPEAT(NUMDIMS)
    (baseEntry.middleIND = originIND + sidelength / 2;
     baseEntry.farEndIND = origin + dirsignIND * sidelength);
    stack[stackInd++] = baseEntry;
    while (stackInd) {
      StackEntry entry = stack[--stackInd];
      if (entry.processed) {
        float minStep = FLT_MAX;
        REPEAT(NUMDIMS)
        (minStep = min(minStep, (farEndIND - currPosIND) * invdirIND););
        currDist += minStep + 1e-8;
        REPEAT(NUMDIMS)(currPosIND = posIND + currDist * dirIND;);
        if (REPEAT(NUMDIMS)((farEndIND - currPosIND) * invdirIND <
                                0 ||) false) {
          continue;
        }
        entry.processed = false;
        stack[stackInd++] = entry;
      } else {
        if (*entry.node++) {
          // this is a leaf node
          int color = *entry.node;
          unsigned char alpha = *img & 0xFF;
          float falpha = alpha / 255.0;
          alpha = alpha * falpha + (color & 0xFF) * (1 - falpha);
          unsigned char blue = ((*img & 0x00FF) >> 8) * falpha +
                               ((color & 0x00FF) >> 8) * (1 - falpha);
          unsigned char green = ((*img & 0x0000FF) >> 16) * falpha +
                                ((color & 0x0000FF) >> 16) * (1 - falpha);
          unsigned char red = ((*img & 0x000000FF) >> 24) * falpha +
                              ((color & 0x000000FF) >> 24) * (1 - falpha);
          *img = alpha | (blue << 8) | (green << 16) | (red << 24);
          if (color & 0xFF == 0xFF) {
            *dist = currDist;
            return true;
          }
        } else {
          float halfSidelength = sidelength / (2 << stackInd);
          entry.index = REPEAT(NUMDIMS)((((entry.middleIND <= currPosIND &&
                                           currPosIND <= entry.farEndIND) ||
                                          (entry.middleIND >= currPosIND &&
                                           currPosIND >= entry.farEndIND))
                                         << IND) |);
          0;
          entry.processed = true;
          stack[stackInd++] = entry;
          StackEntry child;
          child.processed = false;
          child.node = entry.node + entry.node[entry.index];
          REPEAT(NUMDIMS)
          (child.middleIND =
               1.5 * middleIND - 0.5 * farEndIND +
               ((entry.index & (1 << IND)) >> IND) * (farEndIND - middleIND);
           child.farEndIND = child.middleIND + (farEndIND - middleIND) / 2;);
          stack[stackInd++] = child;
        }
      }
    }
    return false;
  }

  bool rayBBoxNUMDIMS(REPEAT(NUMDIMS)(float posIND, float invdirIND,
                                      float minIND, float maxIND, );
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

  bool traverseBVHNUMDIMS(
      __read_only int *bvh,
      REPEAT(NUMDIMS)(float posIND, float dirIND, float invdirIND, );
      unsigned int *img, __write_only float *dist) {
    float invdirArr[NUMDIMS];
    REPEAT(NUMDIMS)(invdirArr[IND] = invdirIND;);

    int *stack[1024];
    size_t stackInd = 0;
    stack[stackInd++] = bvh;
    while (stackInd) {
      int *currBvh = stack[--stackInd];
      float *floatBVH = (float *)currBvh;
      REPEAT(NUMDIMS)
      (float minIND = *floatBVH++; float maxIND = *floatBVH++;);
      if (!rayBBoxNUMDIMS(REPEAT(NUMDIMS)(posIND, invdirIND, minIND, maxIND, );
                          0)) {
        continue;
      }
      currBvh = (int *)floatBVH;
      if (*currBvh++) {
        if (traverseOctreeNUMDIMS(currBvh + *currBvh,
                                  REPEAT(NUMDIMS)(posIND, dirIND, invdirIND, );
                                  img, dist)) {
          return true;
        }
        continue;
      }

      int *lbvh = currBvh + *currBvh;
      currBvh++;
      int *rbvh = currBvh + *currBvh;
      currBvh++;
      int splitInd = *currBvh++;
      if (invdirArr[splitInd] < 0) {
        int *tmp;
        tmp = lbvh;
        lbvh = rbvh;
        rbvh = tmp;
      }
      stack[stackInd++] = rbvh;
      stack[stackInd++] = lbvh;
    }
    return false;
  }

  /*
   The format of soaRays is:

   float posdir[NUMDIMS][2][height][width];
   */
  __kernel void intersectRaysNUMDIMS(__read_only float *soaRays,
                                     __read_only int *bvh, unsigned int *img,
                                     __write_only float *dist) {
    size_t row = get_global_id(0);
    size_t col = get_global_id(1);
    size_t height = get_global_size(0);
    size_t width = get_global_size(1);
    size_t posIndex = row * width + col;
    size_t dirIndex = (height + row) * width + col;
    size_t stride = 2 * width * height;
    REPEAT(NUMDIMS)
    (float posIND = soaRays[posIndex]; float dirIND = soaRays[dirIndex];
     float invdirIND = 1 / dirIND; soaRays += stride;);
    img += posIndex;
    dist += posIndex;
    *img = 0;
    *dist = -1;
    traverseBVHNUMDIMS(bvh, REPEAT(NUMDIMS)(posIND, dirIND, invdirIND, );
                       img, dist);
  }

  __kernel void generateRaysStdNUMDIMS(float *pos, float *forward, float *right,
                                       float *up, __write_only float *soaRays) {
    size_t row = get_global_id(0);
    size_t col = get_global_id(1);
    size_t height = get_global_size(0);
    size_t width = get_global_size(1);
    float xoff = (col + 0.0) / width - 0.5;
    float yoff = (row + 0.0) / height - 0.5;
    REPEAT(NUMDIMS)
    (float posIND = pos[IND];
     float dirIND = forward[IND] + xoff * right[IND] + yoff * up[IND];);
    float norm = 0;
    REPEAT(NUMDIMS)(norm += dirIND * dirIND;);
    norm = sqrt(norm);
    REPEAT(NUMDIMS)(dirIND /= norm;);
    size_t posIndex = row * width + col;
    size_t dirIndex = (height + row) * width + col;
    size_t stride = 2 * width * height;
    REPEAT(NUMDIMS)
    (soaRays[posIndex] = posIND; soaRays[dirIndex] = dirIND;
     soaRays += stride;);
  }})
