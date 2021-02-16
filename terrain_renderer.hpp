#ifndef TERRAIN_RENDERER_HPP_
#define TERRAIN_RENDERER_HPP_

#include <chrono>
#include <memory>
#include <thread>

#include "faces_manager.hpp"
#include "line_follower.hpp"
#include "terrain_cache.hpp"
#include "terrain_slicer.hpp"

namespace hypervoxel {

template <std::size_t N, class TerGen> class TerrainRenderer {

  TerGen terGen;
  std::unique_ptr<Line<N>[]> lines;
  std::size_t numThreads;
  std::unique_ptr<double[]> dists;

  FacesManager<N> facesManager;
  std::unique_ptr<
      typename LineFollower<N, TerrainCache<N, TerGen>>::Controller[]>
      controllers;
  std::unique_ptr<std::thread[]> threads;

  SliceDirs<N> sd;

public:
  /// pdists decreasing
  TerrainRenderer(TerGen &&tterGen, std::size_t terCacheVol,
                  std::size_t facesManagerSize, std::size_t numThreads,
                  double *pdists, const SliceDirs<N> &sd)
      : terGen(tterGen),
        lines(new Line<N>[((N * (N - 1)) / 2) *
                          static_cast<std::size_t>(
                              (pdists[0] + 5) * (pdists[0] + 5) *
                                  (1 + sd.width2 * sd.width2 +
                                   sd.height2 * sd.height2) +
                              3 * pdists[0])]),
        numThreads(numThreads), dists(new double[numThreads]),
        facesManager(facesManagerSize, facesManagerSize / numThreads, sd.cam),
        controllers(new typename LineFollower<
                    N, TerrainCache<N, TerGen>>::Controller[numThreads]()),
        threads(new std::thread[numThreads]), sd(sd) {
    std::copy(pdists, pdists + numThreads, dists.get());
    double currDist = 0;
    double newDist;
    for (std::size_t i = numThreads; i--;) {
      newDist = pdists[i] + 5;
      threads[i] = std::thread(LineFollower<N, TerrainCache<N, TerGen>>(
          currDist, newDist, TerrainCache<N, TerGen>{terGen, terCacheVol},
          facesManager, controllers[i], i));
      currDist = newDist;
    }
  }

  ~TerrainRenderer() {
    for (std::size_t i = numThreads; i--;) {
      controllers[i].queue_op({nullptr, nullptr, true});
      threads[i].join();
    }
  }

  float *writeTriangles(const SliceDirs<N> &nsd, float *out, float *out_fend) {
    Line<N> *lines_end = getLines(sd, dists[0], lines.get());

    sd = nsd;
    facesManager.setCam(&sd.cam[0]);
    facesManager.clear(); // I need the fence after getLines, yes?
    for (std::size_t i = numThreads; i--;) {
      controllers[i].queue_op({lines.get(), lines_end, false});
    }
    for (std::size_t i = numThreads; i--;) {
      while (true) {
        std::unique_lock<std::mutex>(controllers[i].this_mutex);
        if (!controllers[i].queued_op) {
          break;
        }
        std::this_thread::sleep_for(std::chrono::microseconds{1});
      }
    }
    return facesManager.fillVertexAttribPointer(out, out_fend);
  }
};

} // namespace hypervoxel

#endif // TERRAIN_RENDERER_HPP_

