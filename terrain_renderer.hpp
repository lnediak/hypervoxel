#ifndef HYPERVOXEL_TERRAIN_RENDERER_HPP_
#define HYPERVOXEL_TERRAIN_RENDERER_HPP_
#define CL_HPP_MINIMUM_OPENCL_VERSION 110
#define CL_HPP_TARGET_OPENCL_VERSION 110
#define CL_HPP_ENABLE_EXCEPTIONS
#include <CL/cl2.hpp>
#include <exception>
#include <memory>
#include <sstream>

namespace hypervoxel {

class TerrainRenderer {

  double rm, um, fm;

  cl::Platform plat;
  cl::Device devi;
  cl::Context cont;
  cl::CommandQueue queu;

  cl::Program prog;
  cl::Kernel kernel; // TODO: ADD ACTUAL KERNELS

  cl::Buffer buf; // TODO: ADD ACTUAL BUFFERS

  static std::string getSource() const {
#include "cl/cl_sources_all.inc"
    return std::string((const char *)cl_cl_sources_all_cl,
                       cl_cl_sources_all_cl_len);
  }

public:
  TerrainRenderer(/* TODO: ADD ACTUAL ARGUMENTS */) {
    try {
      bool found = false;
      cl::vector<cl::Platform> platforms;
      cl::Platform::get(&platforms);
      for (const cl::Platform &p : platforms) {
        cl::vector<cl::Device> ds;
        p.getDevices(CL_DEVICE_TYPE_GPU, &ds);
        for (const cl::Device &d : ds) {
          /*
          std::string exts = d.getInfo<CL_DEVICE_EXTENSIONS>();
          std::istringstream iss(exts);
          do {
            std::string ext;
            iss >> ext;
            if (ext == "cl_khr_gl_sharing") {
              plat = p;
              devi = d;
              found = true;
              break;
            }
          } while (iss);
          */
          plat = p;
          devi = d;
          found = true;
          if (found) {
            break;
          }
        }
        if (found) {
          break;
        }
      }
      if (!found) {
        throw std::runtime_error("No OpenCL device found. I am sad.");
      }
      cl_context_properties props[] = {CL_CONTEXT_PLATFORM,
                                       (cl_context_properties)plat(), 0};
      cont = cl::Context(devi, props);
      queu = cl::CommandQueue(cont, devi);

      prog = cl::Program(cont, getSource());
      std::vector<cl::Device> dv{devi};
      try {
        prog.build(dv);
      } catch (cl::Error err) {
        if (err.err() != CL_BUILD_PROGRAM_FAILURE) {
          throw err;
        }
        throw std::runtime_error("Build error:\n" +
                                 prog.getBuildInfo<CL_PROGRAM_BUILD_LOG>(devi));
      }

      // TODO: INITIALIZE KERNSL AND BUFFERS
    } catch (cl::Error e) {
      std::stringstream ss;
      ss << "Well, well. " << err.err();
      throw std::runtime_error(ss.str());
    }
  }

  void render(const SliceDirs<5> &sd, int w, int h, unsigned char *img) {
    // TODO: USE KERNEL THAT GENERATES TERRAIN
    // TODO: USE KERNEL THAT RENDERS BASED ON THAT
  }
};

} // namespace hypervoxel

#endif // HYPERVOXEL_TERRAIN_RENDERER_HPP_

