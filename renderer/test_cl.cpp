#include <iostream>
#include <load_cl.hpp>
#include <lodepng/lodepng.h>
#include <random>

cl_uint *generateNode(cl_uint *octree, std::size_t numDims, std::mt19937 &mtrand,
                     cl_float sidelength) {
  cl_uint isLeaf;
  std::uniform_real_distribution<> distroF(0, 1);
  if (sidelength <= 1) {
    isLeaf = 1;
  } else {
    isLeaf = 0; // distroF(mtrand) < 0.08;
  }
  std::uniform_int_distribution<cl_uint> distroI(0, 0xFFFFFF);
  cl_uint color = (distroI(mtrand) << 8) | 0xF0;
  *octree++ = isLeaf;
  if (isLeaf) {
    *octree++ = color;
    return octree;
  }
  std::size_t numChildren = 1 << numDims;
  cl_uint *children = octree;
  cl_uint *currPtr = octree + numChildren;
  cl_float newSlen = sidelength / 2;
  for (std::size_t child = 0; child < numChildren; child++) {
    *octree++ = currPtr - children;
    currPtr = generateNode(currPtr, numDims, mtrand, newSlen);
  }
  return currPtr;
}

int main() {
  ClStuff clStuff = getClStuff();
  const std::size_t numDims = 3, width = 256, height = 256;
  cl_uint bvh[1048576];
  cl_float *currBvhF = reinterpret_cast<cl_float *>(bvh);
  for (std::size_t i = 0; i < numDims; i++) {
    *currBvhF++ = 0;  // min
    *currBvhF++ = 16; // max
  }
  cl_uint *currBvh = reinterpret_cast<cl_uint *>(currBvhF);
  *currBvh++ = 1; // isOctree
  *currBvh++ = 1; // octree
  cl_float *octreeF = reinterpret_cast<cl_float *>(currBvh);
  for (std::size_t i = 0; i < numDims; i++) {
    *octreeF++ = 0; // origin
  }
  *octreeF++ = 16;
  cl_uint *octree = reinterpret_cast<cl_uint *>(octreeF);
  std::mt19937 mtrand(1);
  generateNode(octree, numDims, mtrand, 16);
  RenderKernel kernel =
      compileRenderKernel(clStuff, numDims, 1048576, width, height);
  kernel.writeBvh(clStuff, bvh, 1048576);
  cl_float pos[] = {7.5, 7.5, -4};
  cl_float forward[] = {0, 0, 1};
  cl_float right[] = {1, 0, 0};
  cl_float up[] = {0, 1, 0};
  kernel.writePos(clStuff, pos);
  kernel.writeForward(clStuff, forward);
  kernel.writeRight(clStuff, right);
  kernel.writeUp(clStuff, up);
  try {
    kernel.run(clStuff).wait();
  } catch (const cl::Error &err) {
    throw std::runtime_error(getClErrorString(err));
  }
  cl_uint img[width * height];
  kernel.readImg(clStuff, img, nullptr);
  std::vector<unsigned char> imgVec(width * height * 4);
  for (std::size_t i = 0; i < width * height; i++) {
    imgVec[i * 4] = img[i] >> 24;
    imgVec[i * 4 + 1] = (img[i] >> 16) & 0xFF;
    imgVec[i * 4 + 2] = (img[i] >> 8) & 0xFF;
    imgVec[i * 4 + 3] = img[i] & 0xFF;
  }
  unsigned error = lodepng::encode("out.png", imgVec, width, height);
  if (error) {
    throw std::runtime_error(lodepng_error_text(error));
  }
}

