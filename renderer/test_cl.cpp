#include <iostream>
#include <load_cl.hpp>
#include <lodepng/lodepng.h>
#include <memory>
#include <random>
#include <time.h>
#include <signal.h>
#include <unistd.h>

std::unique_ptr<cl_float[]> getGradVecs(unsigned numGradVecs,
                                        std::size_t numDims, unsigned seed) {
  std::unique_ptr<cl_float[]> gradVecs(new cl_float[numGradVecs * numDims]);
  std::mt19937 mtrand(seed);
  double pi2 = 2 * 3.141592653589792653589793238462643383;
  // assuming numGradVecs is even (which it is)
  for (std::size_t i = 0; i < numGradVecs * numDims; i += 2) {
    double u1 = (1 - mtrand() / 4294967296.0);
    double u2 = (1 - mtrand() / 4294967296.0);
    double r = std::sqrt(-2 * std::log(u1));
    gradVecs[i] = r * std::cos(pi2 * u2);
    gradVecs[i + 1] = r * std::sin(pi2 * u2);
  }
  for (std::size_t i = 0; i < numGradVecs; i++) {
    double norm = 0;
    for (std::size_t j = 0; j < numDims; j++) {
      norm += gradVecs[numDims * i + j] * gradVecs[numDims * i + j];
    }
    // the likelihood of norm being less than 1e-12 in 3 dimensions
    // (chi-squared random variable) is insanely small
    norm = std::sqrt(norm);
    for (std::size_t j = 0; j < numDims; j++) {
      gradVecs[numDims * i + j] /= norm;
    }
  }
  return gradVecs;
}

int main() {
  ClStuff clStuff = getClStuff();
  const std::size_t numDims = 3, width = 1020, height = 1020;
  cl_uint bvh[1048576];
  cl_float *currBvhF = reinterpret_cast<cl_float *>(bvh);
  for (std::size_t i = 0; i < numDims; i++) {
    *currBvhF++ = 0;  // min
    *currBvhF++ = 64; // max
  }
  cl_uint *currBvh = reinterpret_cast<cl_uint *>(currBvhF);
  *currBvh++ = 2; // objType
  *currBvh++ = 1; // obj
  cl_float *objectF = reinterpret_cast<cl_float *>(currBvh);
  for (std::size_t i = 0; i < numDims; i++) {
    *objectF++ = 0.0f; // origin
  }
  cl_uint *object = reinterpret_cast<cl_uint *>(objectF);
  for (std::size_t i = 0; i < numDims; i++) {
    *object++ = 64; // objDims
  }
  *object++ = 1; // hashMapSize
  for (std::size_t i = 0; i <= numDims; i++) {
    *object++ = 0; // hashMap
  }
  RenderKernel kernel = compileRenderKernel(clStuff, numDims, 1048576, width,
                                            height, 0.0875, 4096, 3, 0.5);
  kernel.writeBvh(clStuff, bvh, 1048576);
  cl_float pos[] = {32, 32, -40};
  cl_float forward[] = {0, 0, 1};
  cl_float right[] = {1, 0, 0};
  cl_float up[] = {0, 1, 0};
  cl_float scale[] = {256, 256, 256};
  kernel.writePos(clStuff, pos);
  kernel.writeForward(clStuff, forward);
  kernel.writeRight(clStuff, right);
  kernel.writeUp(clStuff, up);
  kernel.writeScale(clStuff, scale);
  kernel.writeGradVecs(clStuff, getGradVecs(4096, 3, 2).get(), 4096);
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
    cl::Event ev = kernel.run(clStuff, width, height);
    bool success = false;
    for (std::size_t i = 30; i--;) {
      sleep(1);
      if (ev.getInfo<CL_EVENT_COMMAND_EXECUTION_STATUS>() == CL_COMPLETE) {
        std::cout << "Success!" << std::endl;
        success = true;
        break;
      }
    }
    if (!success) {
      std::cout << "Time's up!" << std::endl;
      exit(SIGTERM);
    }
    //throw std::runtime_error("Owned");
  } catch (const cl::Error &err) {
    throw std::runtime_error(getClErrorString(err));
  }
  std::vector<unsigned char> imgVec(width * height * 4);
  std::cout << "Reading image" << std::endl;
  kernel.readImg(clStuff, &imgVec[0], nullptr);
  std::cout << "Writing image" << std::endl;
  unsigned error = lodepng::encode("out.png", imgVec, width, height);
  if (error) {
    throw std::runtime_error(lodepng_error_text(error));
  }
}

