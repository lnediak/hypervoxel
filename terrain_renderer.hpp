#define CL_HPP_MINIMUM_OPENCL_VERSION 110
#define CL_HPP_TARGET_OPENCL_VERSION 110
#define CL_HPP_ENABLE_EXCEPTIONS
#include <CL/cl2.hpp>
#include <exception>
#include <iostream>
#include <iterator>
#include <memory>
#include <sstream>

#include "terrain_slicer.hpp"

namespace hypervoxel {

class TerrainRenderer {

  double rm, um, fm;

  cl::Platform plat;
  cl::Device devi;
  cl::Context cont;
  cl::CommandQueue queu;

  cl::Program prog;
  cl::Kernel getOffs, clearBuf, initGlobalsForMain, mainKernel;

  cl::Buffer fs, us2s, len, data, ui32s, floats2, o;
  cl::Buffer f, g;
  cl_uint sz;
  unsigned us2ssz, szsz, ui32ssz, floats2sz;

  unsigned dEntrySz, dEdgeSz, dEdgeOf, dEdg2Of;
  unsigned dFSz, dGSz;

  cl::Buffer lines;
  std::unique_ptr<cl_float[]> lineshp;
  unsigned linessz;

  std::string getSource() const {
#include "cl/cl_sources_all.inc"
    return std::string((const char *)cl_cl_sources_all_cl,
                       cl_cl_sources_all_cl_len);
  }

public:
  TerrainRenderer(cl_uint sz, double rm, double um, double fm)
      : rm(rm), um(um), fm(fm), prog(""), sz(sz) {
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

      getOffs = cl::Kernel(prog, "getOffs");
      clearBuf = cl::Kernel(prog, "clearBuf");
      initGlobalsForMain = cl::Kernel(prog, "initGlobalsForMain");
      mainKernel = cl::Kernel(prog, "mainKernel");

      fs = cl::Buffer(cont, CL_MEM_READ_ONLY, 23 * sizeof(cl_float));
      const unsigned fit1 = 100663296;
      const unsigned fit1sm = 65536;
      const unsigned fit2sm = 524288;
      us2s = cl::Buffer(cont, CL_MEM_READ_WRITE,
                        us2ssz = fit1 * sizeof(cl_ushort2));
      len = cl::Buffer(cont, CL_MEM_READ_WRITE, sizeof(cl_uint));
      data = cl::Buffer(cont, CL_MEM_WRITE_ONLY | CL_MEM_ALLOC_HOST_PTR, sz);
      ui32s = cl::Buffer(cont, CL_MEM_READ_WRITE,
                         ui32ssz = fit1sm * 32 * sizeof(cl_uint));
      floats2 = cl::Buffer(cont, CL_MEM_READ_WRITE,
                           floats2sz = fit2sm * sizeof(cl_float));
      o = cl::Buffer(cont, CL_MEM_READ_ONLY, 4097 * sizeof(cl_float8));
      us2ssz /= 128;
      szsz = sz / 128;
      ui32ssz /= 128;
      floats2sz /= 128;

      cl::Buffer offs(cont, CL_MEM_WRITE_ONLY, 6 * sizeof(cl_uint));
      getOffs.setArg(0, offs);
      queu.enqueueNDRangeKernel(getOffs, cl::NullRange, cl::NDRange(1));
      cl_uint offsres[6];
      queu.enqueueReadBuffer(offs, CL_TRUE, 0, 6 * sizeof(cl_uint), offsres);
      dEntrySz = offsres[0];
      dEdgeSz = offsres[1];
      dEdgeOf = offsres[2];
      dEdg2Of = offsres[3];
      dFSz = offsres[4];
      dGSz = offsres[5];

      f = cl::Buffer(cont, CL_MEM_READ_WRITE, dFSz);
      g = cl::Buffer(cont, CL_MEM_READ_WRITE, dGSz);

      lines = cl::Buffer(
          cont, CL_MEM_READ_ONLY,
          linessz =
              17 * sizeof(cl_float) * 10 *
              unsigned((fm + 5) * (fm * 5) * (1 + rm * rm + um * um) + 3 * fm));
      lineshp.reset(new cl_float[linessz / sizeof(cl_float)]);
    } catch (cl::Error err) {
      std::stringstream ss;
      ss << "Well, well. " << err.err();
      throw std::runtime_error(ss.str());
    }
  }

  void setPerlinOptions(v::DVec<5> s, unsigned nOct, float pers,
                        const cl_float8 *g) {
    cl_float ob[4097 * 8];
    for (int i = 0; i < 5; i++) {
      ob[i] = s[i];
    }
    cl_uint ngvm = 4095, no = nOct;
    ob[5] = *(cl_float *)&ngvm;
    ob[6] = *(cl_float *)&no;
    ob[7] = pers;
    std::memcpy(&ob[8], g, 4096 * sizeof(cl_float8));
    queu.enqueueWriteBuffer(o, CL_TRUE, 0, 4097 * sizeof(cl_float8), ob);
  }

  float *generateTriangles(const SliceDirs<5> &d, double d1, double d2,
                           float *out) {
    try {
      cl_float fsf[23] = {
          (cl_float)d.cam[0],     (cl_float)d.cam[1],
          (cl_float)d.cam[2],     (cl_float)d.cam[3],
          (cl_float)d.cam[4],     (cl_float)d.right[0],
          (cl_float)d.right[1],   (cl_float)d.right[2],
          (cl_float)d.right[3],   (cl_float)d.right[4],
          (cl_float)d.up[0],      (cl_float)d.up[1],
          (cl_float)d.up[2],      (cl_float)d.up[3],
          (cl_float)d.up[4],      (cl_float)d.forward[0],
          (cl_float)d.forward[1], (cl_float)d.forward[2],
          (cl_float)d.forward[3], (cl_float)d.forward[4],
          (cl_float)fm,           (cl_float)rm,
          (cl_float)um,
      };
      queu.enqueueWriteBuffer(fs, CL_FALSE, 0, 23 * sizeof(cl_float), fsf);
      struct LineWriter {

        cl_float *out;

        void operator()(const Line<5> &l) {
          *out++ = l.a[0];
          *out++ = l.a[1];
          *out++ = l.a[2];
          *out++ = l.a[3];
          *out++ = l.a[4];

          *out++ = l.b[0];
          *out++ = l.b[1];
          *out++ = l.b[2];
          *out++ = l.b[3];
          *out++ = l.b[4];

          *out++ = l.a3[0];
          *out++ = l.a3[1];
          *out++ = l.a3[2];

          *out++ = l.b3[0];
          *out++ = l.b3[1];
          *out++ = l.b3[2];

          cl_float weird;
          cl_ushort *weirdp = (cl_ushort *)&weird;
          weirdp[0] = l.dim1;
          weirdp[1] = l.dim2;
          *out++ = weird;
        }
      } lineWriter{lineshp.get()};
      getLines(d, fm, lineWriter);
      unsigned df = lineWriter.out - lineshp.get();
      queu.enqueueWriteBuffer(lines, CL_FALSE, 0, df * sizeof(cl_float),
                              lineshp.get());

      clearBuf.setArg(0, us2s);
      queu.enqueueNDRangeKernel(clearBuf, cl::NullRange, cl::NDRange(us2ssz));
      clearBuf.setArg(0, data);
      queu.enqueueNDRangeKernel(clearBuf, cl::NullRange, cl::NDRange(szsz));
      clearBuf.setArg(0, ui32s);
      queu.enqueueNDRangeKernel(clearBuf, cl::NullRange, cl::NDRange(ui32ssz));
      clearBuf.setArg(0, floats2);
      queu.enqueueNDRangeKernel(clearBuf, cl::NullRange,
                                cl::NDRange(floats2sz));

      initGlobalsForMain.setArg(0, f);
      initGlobalsForMain.setArg(1, g);
      initGlobalsForMain.setArg(2, fs);
      initGlobalsForMain.setArg(3, us2s);
      initGlobalsForMain.setArg(4, sz);
      initGlobalsForMain.setArg(5, len);
      initGlobalsForMain.setArg(6, data);
      initGlobalsForMain.setArg(7, ui32s);
      initGlobalsForMain.setArg(8, floats2);
      initGlobalsForMain.setArg(9, o);
      queu.enqueueNDRangeKernel(initGlobalsForMain, cl::NullRange,
                                cl::NDRange(1));

      mainKernel.setArg(0, f);
      mainKernel.setArg(1, g);
      mainKernel.setArg(2, lines);
      mainKernel.setArg(3, (cl_float)d1);
      mainKernel.setArg(4, (cl_float)d2);
      queu.enqueueNDRangeKernel(mainKernel, cl::NullRange,
                                cl::NDRange(df / 17));

      // now then, let's process data!

      char *dp =
          (char *)queu.enqueueMapBuffer(data, CL_TRUE, CL_MAP_READ, 0, sz);
      std::cout << "Memory mapped! " << std::endl;
      char *dpe = dp + sz - dEntrySz;
      while (dp < dpe) {
        for (int i = 0; i < 5; i++) {
          cl_ushort color = ((cl_ushort *)dp)[i];
          char *dpp = dp + dEdgeOf + i * 8 * dEdgeSz;
          int k = 0;
          bool found = false;
          cl_float3 base;
          for (int j = 0; j < 8; j++, k++) {
            if (k == i || k == i + 5) {
              k++;
            }
            cl_float3 *a = (cl_float3 *)dpp;
            dpp += dEdgeSz;
            if (std::isnan(a->s[0])) {
              continue;
            }
            if (!found) {
              base = *a;
              found = true;
              continue;
            }
            struct IsClose {
              bool operator()(cl_float a, cl_float b) const {
                cl_float c = a - b;
                return -1e-5 < c && c < 1e-5;
              }
            } isC;
            if (isC(a->s[0], base.s[0]) && isC(a->s[1], base.s[1]) &&
                isC(a->s[2], base.s[2])) {
              continue;
            }
            cl_float3 b = *(cl_float3 *)((char *)a + dEdg2Of);
            if (isC(b.s[0], base.s[0]) && isC(b.s[1], base.s[1]) &&
                isC(b.s[2], base.s[2])) {
              continue;
            }
            float cr = (color & 0x7C00) / 32768.;
            float cg = (color & 0x03E0) / 1024.;
            float cb = (color & 0x001F) / 32.;
            *out++ = base.s[0];
            *out++ = base.s[1];
            *out++ = base.s[2];
            *out++ = cr;
            *out++ = cg;
            *out++ = cb;

            *out++ = a->s[0];
            *out++ = a->s[1];
            *out++ = a->s[2];
            *out++ = cr;
            *out++ = cg;
            *out++ = cb;

            *out++ = b.s[0];
            *out++ = b.s[1];
            *out++ = b.s[2];
            *out++ = cr;
            *out++ = cg;
            *out++ = cb;
          }
        }
      }
      queu.enqueueUnmapMemObject(data, dp);
      return out;
    } catch (cl::Error err) {
      std::stringstream ss;
      ss << "Well, well. " << err.err();
      throw std::runtime_error(ss.str());
    }
    return nullptr;
  }
};

} // namespace hypervoxel

