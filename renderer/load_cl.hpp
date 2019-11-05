#define __CL_ENABLE_EXCEPTIONS
#include <CL/cl.hpp>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

inline std::string getClErrorString(cl_int err) {
  switch (err) {
  case 0:
    return "CL_SUCCESS";
  case -1:
    return "CL_DEVICE_NOT_FOUND";
  case -2:
    return "CL_DEVICE_NOT_AVAILABLE";
  case -3:
    return "CL_COMPILER_NOT_AVAILABLE";
  case -4:
    return "CL_MEM_OBJECT_ALLOCATION_FAILURE";
  case -5:
    return "CL_OUT_OF_RESOURCES";
  case -6:
    return "CL_OUT_OF_HOST_MEMORY";
  case -7:
    return "CL_PROFILING_INFO_NOT_AVAILABLE";
  case -8:
    return "CL_MEM_COPY_OVERLAP";
  case -9:
    return "CL_IMAGE_FORMAT_MISMATCH";
  case -10:
    return "CL_IMAGE_FORMAT_NOT_SUPPORTED";
  case -11:
    return "CL_BUILD_PROGRAM_FAILURE";
  case -12:
    return "CL_MAP_FAILURE";
  case -13:
    return "CL_MISALIGNED_SUB_BUFFER_OFFSET";
  case -14:
    return "CL_EXEC_STATUS_ERROR_FOR_EVENTS_IN_WAIT_LIST";
  case -15:
    return "CL_COMPILE_PROGRAM_FAILURE";
  case -16:
    return "CL_LINKER_NOT_AVAILABLE";
  case -17:
    return "CL_LINK_PROGRAM_FAILURE";
  case -18:
    return "CL_DEVICE_PARTITION_FAILED";
  case -19:
    return "CL_KERNEL_ARG_INFO_NOT_AVAILABLE";

  case -30:
    return "CL_INVALID_VALUE";
  case -31:
    return "CL_INVALID_DEVICE_TYPE";
  case -32:
    return "CL_INVALID_PLATFORM";
  case -33:
    return "CL_INVALID_DEVICE";
  case -34:
    return "CL_INVALID_CONTEXT";
  case -35:
    return "CL_INVALID_QUEUE_PROPERTIES";
  case -36:
    return "CL_INVALID_COMMAND_QUEUE";
  case -37:
    return "CL_INVALID_HOST_PTR";
  case -38:
    return "CL_INVALID_MEM_OBJECT";
  case -39:
    return "CL_INVALID_IMAGE_FORMAT_DESCRIPTOR";
  case -40:
    return "CL_INVALID_IMAGE_SIZE";
  case -41:
    return "CL_INVALID_SAMPLER";
  case -42:
    return "CL_INVALID_BINARY";
  case -43:
    return "CL_INVALID_BUILD_OPTIONS";
  case -44:
    return "CL_INVALID_PROGRAM";
  case -45:
    return "CL_INVALID_PROGRAM_EXECUTABLE";
  case -46:
    return "CL_INVALID_KERNEL_NAME";
  case -47:
    return "CL_INVALID_KERNEL_DEFINITION";
  case -48:
    return "CL_INVALID_KERNEL";
  case -49:
    return "CL_INVALID_ARG_INDEX";
  case -50:
    return "CL_INVALID_ARG_VALUE";
  case -51:
    return "CL_INVALID_ARG_SIZE";
  case -52:
    return "CL_INVALID_KERNEL_ARGS";
  case -53:
    return "CL_INVALID_WORK_DIMENSION";
  case -54:
    return "CL_INVALID_WORK_GROUP_SIZE";
  case -55:
    return "CL_INVALID_WORK_ITEM_SIZE";
  case -56:
    return "CL_INVALID_GLOBAL_OFFSET";
  case -57:
    return "CL_INVALID_EVENT_WAIT_LIST";
  case -58:
    return "CL_INVALID_EVENT";
  case -59:
    return "CL_INVALID_OPERATION";
  case -60:
    return "CL_INVALID_GL_OBJECT";
  case -61:
    return "CL_INVALID_BUFFER_SIZE";
  case -62:
    return "CL_INVALID_MIP_LEVEL";
  case -63:
    return "CL_INVALID_GLOBAL_WORK_SIZE";
  case -64:
    return "CL_INVALID_PROPERTY";
  case -65:
    return "CL_INVALID_IMAGE_DESCRIPTOR";
  case -66:
    return "CL_INVALID_COMPILER_OPTIONS";
  case -67:
    return "CL_INVALID_LINKER_OPTIONS";
  case -68:
    return "CL_INVALID_DEVICE_PARTITION_COUNT";

  case -1000:
    return "CL_INVALID_GL_SHAREGROUP_REFERENCE_KHR";
  case -1001:
    return "CL_PLATFORM_NOT_FOUND_KHR";
  case -1002:
    return "CL_INVALID_D3D10_DEVICE_KHR";
  case -1003:
    return "CL_INVALID_D3D10_RESOURCE_KHR";
  case -1004:
    return "CL_D3D10_RESOURCE_ALREADY_ACQUIRED_KHR";
  case -1005:
    return "CL_D3D10_RESOURCE_NOT_ACQUIRED_KHR";
  default:
    return "Unknown OpenCL error";
  }
}

inline std::string getClErrorString(const cl::Error err) {
  if (err.what()) {
    return getClErrorString(err.err()) + ": " + err.what();
  }
  return getClErrorString(err.err());
}

struct ClStuff {

  cl::Platform platform;
  cl::Device device;
  cl::Context context;
  cl::CommandQueue queue;
};

inline ClStuff getClStuff() {
  try {
    std::vector<cl::Platform> platforms;
    cl::Platform::get(&platforms);
    if (platforms.empty()) {
      throw std::runtime_error("No OpenCL platforms found. Check your drivers");
    }
    for (cl::Platform plat : platforms) {
      std::vector<cl::Device> devices;
      plat.getDevices(CL_DEVICE_TYPE_GPU, &devices);
      if (devices.empty()) {
        continue;
      }
      cl::Context context({devices[0]});
      return {plat, devices[0], context, {context, devices[0]}};
    }
    throw std::runtime_error("No OpenCL devices found");
  } catch (const cl::Error &err) {
    throw std::runtime_error(getClErrorString(err));
  }
}

struct RenderKernel {

  std::size_t numDims, bvhLen;
  std::size_t width, height;
  cl::Buffer bvh;
  cl::Buffer pos, forward, right, up;
  cl::Buffer img, dist;
  cl::Kernel kernel;

  cl::Event run(const ClStuff &clStuff, std::size_t tmpWidth = -1,
                std::size_t tmpHeight = -1) {
    std::size_t neg1 = -1;
    if ((tmpWidth == neg1) || (tmpHeight == neg1)) {
      tmpWidth = width;
      tmpHeight = height;
    }
    try {
      kernel.setArg(0, bvh);
      kernel.setArg(1, pos);
      kernel.setArg(2, forward);
      kernel.setArg(3, right);
      kernel.setArg(4, up);
      kernel.setArg(5, img);
      kernel.setArg(6, dist);
      cl::Event toreturn;
      clStuff.queue.enqueueNDRangeKernel(kernel, cl::NullRange,
                                         cl::NDRange(tmpWidth, tmpHeight),
                                         cl::NullRange, nullptr, &toreturn);
      return toreturn;
    } catch (const cl::Error &err) {
      throw std::runtime_error(getClErrorString(err));
    }
  }

  void writeBvh(const ClStuff &clStuff, cl_uint *bvhPtr,
                std::size_t newBvhLen) {
    try {
      if (newBvhLen > bvhLen) {
        bvh = cl::Buffer(clStuff.context, CL_MEM_READ_ONLY,
                         sizeof(cl_uint) * newBvhLen);
        bvhLen = newBvhLen;
      }
      clStuff.queue.enqueueWriteBuffer(bvh, true, 0, sizeof(cl_uint) * bvhLen,
                                       bvhPtr);
    } catch (const cl::Error &err) {
      throw std::runtime_error(getClErrorString(err));
    }
  }

  void writePos(const ClStuff &clStuff, cl_float *posPtr) {
    try {
      clStuff.queue.enqueueWriteBuffer(pos, true, 0, sizeof(cl_float) * numDims,
                                       posPtr);
    } catch (const cl::Error &err) {
      throw std::runtime_error(getClErrorString(err));
    }
  }

  void writeForward(const ClStuff &clStuff, cl_float *forwardPtr) {
    try {
      clStuff.queue.enqueueWriteBuffer(forward, true, 0,
                                       sizeof(cl_float) * numDims, forwardPtr);
    } catch (const cl::Error &err) {
      throw std::runtime_error(getClErrorString(err));
    }
  }

  void writeRight(const ClStuff &clStuff, cl_float *rightPtr) {
    try {
      clStuff.queue.enqueueWriteBuffer(right, true, 0,
                                       sizeof(cl_float) * numDims, rightPtr);
    } catch (const cl::Error &err) {
      throw std::runtime_error(getClErrorString(err));
    }
  }

  void writeUp(const ClStuff &clStuff, cl_float *upPtr) {
    try {
      clStuff.queue.enqueueWriteBuffer(up, true, 0, sizeof(cl_float) * numDims,
                                       upPtr);
    } catch (const cl::Error &err) {
      throw std::runtime_error(getClErrorString(err));
    }
  }

  void readImg(const ClStuff &clStuff, cl_uint *imgPtr, cl_uint *distPtr) {
    try {
      if (imgPtr) {
        clStuff.queue.enqueueReadBuffer(
            img, true, 0, sizeof(cl_uint) * width * height, imgPtr);
      }
      if (distPtr) {
        clStuff.queue.enqueueReadBuffer(
            dist, true, 0, sizeof(cl_float) * width * height, distPtr);
      }
    } catch (const cl::Error &err) {
      throw std::runtime_error(getClErrorString(err));
    }
  }
};

inline RenderKernel compileRenderKernel(const ClStuff &clStuff,
                                        std::size_t numDims, std::size_t bvhLen,
                                        std::size_t width, std::size_t height) {
  try {
    cl::Program::Sources src;
    std::string code =
#include "main.cl.inc"
        ;
    src.push_back({code.c_str(), code.size()});

    cl::Program prog(clStuff.context, src);
    try {
      prog.build({clStuff.device}, "-cl-std=CL1.1");
    } catch (const cl::Error &err) {
      throw std::runtime_error(
          getClErrorString(err) + "\nBuild log: " +
          prog.getBuildInfo<CL_PROGRAM_BUILD_LOG>(clStuff.device));
    }
    std::stringstream ss;
    ss << "renderStd" << numDims;
    std::size_t numPixels = width * height;
    return {numDims,
            bvhLen,
            width,
            height,
            {clStuff.context, CL_MEM_READ_ONLY, sizeof(cl_uint) * bvhLen},
            {clStuff.context, CL_MEM_READ_ONLY, sizeof(cl_float) * numDims},
            {clStuff.context, CL_MEM_READ_ONLY, sizeof(cl_float) * numDims},
            {clStuff.context, CL_MEM_READ_ONLY, sizeof(cl_float) * numDims},
            {clStuff.context, CL_MEM_READ_ONLY, sizeof(cl_float) * numDims},
            {clStuff.context, CL_MEM_READ_WRITE, sizeof(cl_uint) * numPixels},
            {clStuff.context, CL_MEM_WRITE_ONLY, sizeof(cl_float) * numPixels},
            {prog, ss.str().c_str()}};
  } catch (const cl::Error &err) {
    throw std::runtime_error(getClErrorString(err));
  }
}

