#include <iostream>
#include <load_cl.hpp>
#include <lodepng/lodepng.h>
#include <random>
#include <time.h>

cl_uint *generateNode(cl_uint *octree, std::size_t numDims,
                      std::mt19937 &mtrand, cl_float sidelength) {
  cl_uint isLeaf;
  std::uniform_real_distribution<> distroF(0, 1);
  if (sidelength <= 1) {
    isLeaf = 1;
  } else {
    isLeaf = distroF(mtrand) < 0.08;
  }
  std::uniform_int_distribution<cl_uint> distroI(0, 0xFFFFFFFF);
  cl_uint color = distroF(mtrand) < 0.6 ? 0 : (distroI(mtrand) & ~0xFF) | 0xA0;
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
  const std::size_t numDims = 3, width = 1020, height = 1020;
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
  cl_float pos[] = {8, 8, -4};
  cl_float forward[] = {0, 0, 1};
  cl_float right[] = {1, 0, 0};
  cl_float up[] = {0, 1, 0};
  kernel.writePos(clStuff, pos);
  kernel.writeForward(clStuff, forward);
  kernel.writeRight(clStuff, right);
  kernel.writeUp(clStuff, up);
  try {
    /*
    for (std::size_t col = 0; col < width; col++) {
      for (std::size_t row = 0; row < height; row++) {
        //size_t row = 25;
        //size_t col = 0;
        std::cout << row << " " << col << std::endl;
        cl_uint arr[] = {cl_uint(row), cl_uint(col), cl_uint(height),
                         cl_uint(width)};
        clStuff.queue.enqueueWriteBuffer(kernel.img, true, 0, sizeof(arr), arr);
        cl::Event event = kernel.run(clStuff, 1, 1);
        *
        struct timespec nano = {0, 10000000};
        nanosleep(&nano, nullptr);
        if (event.getInfo<CL_EVENT_COMMAND_EXECUTION_STATUS>() != CL_COMPLETE) {
          std::cout << "quack" << std::endl;
          return 1;
        }
        *
        event.wait();
      }
    }
    */
    /*float dist[width * height];
    float *distp = dist;
    kernel.readImg(clStuff, nullptr, distp);
    for (std::size_t i = 0; i < 99; i++) {
      std::cout << "currDist: " << *distp++ << std::endl;
      std::cout << "currPos"
                << ": " << *distp << ", " << *(distp + 4) << ", "
                << *(distp + 8) << std::endl;
      distp++;
      std::cout << "invdir"
                << ": " << *distp << ", " << *(distp + 4) << ", "
                << *(distp + 8) << std::endl;
      distp++;
      std::cout << "middle"
                << ": " << *distp << ", " << *(distp + 4) << ", "
                << *(distp + 8) << std::endl;
      distp++;
      std::cout << "farEnd"
                << ": " << *distp << ", " << *(distp + 4) << ", "
                << *(distp + 8) << std::endl;
      distp += 9;
      std::cout << "IsLeaf: " << *distp++ << std::endl;
      std::cout << "StackInd: " << *distp++ << std::endl;
      std::cout << std::endl << std::endl;
    } */
    kernel.run(clStuff, width, height).wait();
  } catch (const cl::Error &err) {
    throw std::runtime_error(getClErrorString(err));
  }
  std::vector<unsigned char> imgVec(width * height * 4);
  kernel.readImg(clStuff, &imgVec[0], nullptr);
  unsigned error = lodepng::encode("out.png", imgVec, width, height);
  if (error) {
    throw std::runtime_error(lodepng_error_text(error));
  }
}

