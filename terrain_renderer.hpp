#ifndef HYPERVOXEL_TERRAIN_RENDERER_HPP_
#define HYPERVOXEL_TERRAIN_RENDERER_HPP_

#define CL_HPP_MINIMUM_OPENCL_VERSION 110
#define CL_HPP_TARGET_OPENCL_VERSION 110
#define CL_HPP_ENABLE_EXCEPTIONS
#include <CL/cl2.hpp>
#include <exception>
#include <memory>
#include <sstream>

#include "generator_perlin.hpp"
#include "terrain_indexer.hpp"

namespace hypervoxel {

class TerrainRenderer {

  double rm, um, fm;
  std::size_t width, height;

  cl::Platform plat;
  cl::Device devi;
  cl::Context cont;
  cl::CommandQueue queu;

  cl::Program prog;
  cl::Kernel lerpTerrain, renderTerrain;

  std::unique_ptr<cl_float[]> tdsd;
  std::unique_ptr<cl_float16[]> dsd;
  cl::Buffer ftis, fti1, ds, d1;
  cl::Buffer fsd, out;

  GeneratorPerlin<5> terGen;

  static std::string getSource() {
#include "cl/cl_sources_all.inc"
    return std::string((const char *)___cl_cl_sources_all_cl,
                       ___cl_cl_sources_all_cl_len);
  }

public:
  TerrainRenderer(double rm, double um, double fm, std::size_t width,
                  std::size_t height, GeneratorPerlin<5> &&terGen)
      : rm(rm), um(um), fm(fm), width(width), height(height),
        terGen(std::move(terGen)) {
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

      lerpTerrain = cl::Kernel(prog, "lerpTerrain");
      renderTerrain = cl::Kernel(prog, "renderTerrain");

      ftis = cl::Buffer(cont, CL_MEM_READ_ONLY, TerrainIndexer::SERIAL_LEN);
      fti1 = cl::Buffer(cont, CL_MEM_READ_ONLY, TerrainIndexer::SERIAL_LEN);
      std::size_t dssz = 20000000; // TODO: PROPER SZ
      tdsd.reset(new cl_float[dssz * 16]);
      dsd.reset(new cl_float16[dssz]);
      ds = cl::Buffer(cont, CL_MEM_READ_ONLY, dssz * sizeof(cl_float16));
      d1 = cl::Buffer(cont, CL_MEM_READ_WRITE, /* ^^^ */ 410000000);

      fsd = cl::Buffer(cont, CL_MEM_READ_ONLY, SliceDirs<5>::SERIAL_LEN);
      out = cl::Buffer(cont, CL_MEM_WRITE_ONLY, width * height * 4);
    } catch (cl::Error e) {
      std::stringstream ss;
      ss << "Well, well. " << e.err();
      throw std::runtime_error(ss.str());
    }
  }

#define TERGEN_CACHE_SCALE 16

  cl_float16 generateClFloat16(const TerrainIndexer &ttis, const v::IVec<5> &ds,
                               v::IVec<5> &c) {
    cl_float16 toreturn;
    toreturn.s[0] = tdsd[ttis.getIndex(c)];
    c[ds[0]]++;
    toreturn.s[1] = tdsd[ttis.getIndex(c)];
    c[ds[1]]++;
    toreturn.s[3] = tdsd[ttis.getIndex(c)];
    c[ds[0]]--;
    toreturn.s[2] = tdsd[ttis.getIndex(c)];

    c[ds[2]]++;
    toreturn.s[6] = tdsd[ttis.getIndex(c)];
    c[ds[0]]++;
    toreturn.s[7] = tdsd[ttis.getIndex(c)];
    c[ds[1]]--;
    toreturn.s[5] = tdsd[ttis.getIndex(c)];
    c[ds[0]]--;
    toreturn.s[4] = tdsd[ttis.getIndex(c)];

    c[ds[3]]++;
    toreturn.s[12] = tdsd[ttis.getIndex(c)];
    c[ds[0]]++;
    toreturn.s[13] = tdsd[ttis.getIndex(c)];
    c[ds[1]]++;
    toreturn.s[15] = tdsd[ttis.getIndex(c)];
    c[ds[0]]--;
    toreturn.s[14] = tdsd[ttis.getIndex(c)];

    c[ds[2]]--;
    toreturn.s[10] = tdsd[ttis.getIndex(c)];
    c[ds[0]]++;
    toreturn.s[11] = tdsd[ttis.getIndex(c)];
    c[ds[1]]--;
    toreturn.s[9] = tdsd[ttis.getIndex(c)];
    c[ds[0]]--;
    toreturn.s[8] = tdsd[ttis.getIndex(c)];
    c[ds[3]]--;

    return toreturn;
  }

  void render(const SliceDirs<5> &sdi, cl_uchar *img) {
    try {
      SliceDirs<5> sd = sdi;
      sd.rm = rm;
      sd.um = um;
      sd.fm = fm;
      SliceDirs<5> sds = sd;
      sds.c /= TERGEN_CACHE_SCALE;
      sds.fm /= TERGEN_CACHE_SCALE;
      TerrainIndexer ttis(sds, 2, false);
      std::size_t index = 0;
      v::IVec<5> i5 = ttis.getI5(index);
      v::IVec<5> coord = ttis.getCoord5(i5);
      do {
        /*float f = */ tdsd[index++] = terGen.get(coord * TERGEN_CACHE_SCALE);
        // std::cout << "generated: " << f << std::endl;
      } while (ttis.incCoord5(coord, i5));

      TerrainIndexer tis(sds, 1, false);
      index = 0;
      i5 = tis.getI5(0);
      coord = tis.getCoord5(i5);
      do {
        v::IVec<5> tcoord = coord - 1;
        v::IVec<5> ttisds = ttis.getDs();
        dsd[index++] = generateClFloat16(ttis, ttisds, tcoord);
        // std::cout << "ds32[0] orig: " << v::FVec<16>(&why.s[0]) << std::endl;
        tcoord[ttisds[4]]++;
        dsd[index++] = generateClFloat16(ttis, ttisds, tcoord);
      } while (tis.incCoord5(coord, i5));
      queu.enqueueWriteBuffer(ds, CL_FALSE, 0, index * sizeof(cl_float16),
                              dsd.get());

      cl_float tiserial[TerrainIndexer::SERIAL_LEN / 4];
      tis.serialize(tiserial, (cl_int *)tiserial);
      queu.enqueueWriteBuffer(ftis, CL_FALSE, 0, sizeof(tiserial), tiserial);
      TerrainIndexer ti(sd, 1, false);
      ti.serialize(tiserial, (cl_int *)tiserial);
      queu.enqueueWriteBuffer(fti1, CL_FALSE, 0, sizeof(tiserial), tiserial);

      lerpTerrain.setArg(0, (int)ti.getSize());
      lerpTerrain.setArg(1, ftis);
      lerpTerrain.setArg(2, fti1);
      lerpTerrain.setArg(3, ds);
      lerpTerrain.setArg(4, d1);
      queu.enqueueNDRangeKernel(
          lerpTerrain, cl::NullRange,
          cl::NDRange((ti.getSize() + 31) & ~(std::size_t)0x1F), cl::NullRange);

      cl_float sdserial[SliceDirs<5>::SERIAL_LEN / 4];
      serializeSliceDirs(sd, ttis, sdserial);
      queu.enqueueWriteBuffer(fsd, CL_FALSE, 0, sizeof(sdserial), sdserial);

      renderTerrain.setArg(0, fsd);
      renderTerrain.setArg(1, fti1);
      renderTerrain.setArg(2, d1);
      renderTerrain.setArg(3, out);
      queu.enqueueNDRangeKernel(renderTerrain, cl::NullRange,
                                cl::NDRange(width, height), cl::NullRange);

      queu.enqueueReadBuffer(out, CL_TRUE, 0, width * height * 4, img);
    } catch (cl::Error e) {
      std::stringstream ss;
      ss << "Well, well. render here. " << e.what() << " " << e.err();
      throw std::runtime_error(ss.str());
    }
  }
};

} // namespace hypervoxel

#endif // HYPERVOXEL_TERRAIN_RENDERER_HPP_

