#ifndef TERRAIN_RENDERER_HPP_
#define TERRAIN_RENDERER_HPP_

#include <memory>
#include <thread>

#include "faces_manager.hpp"
#include "line_follower.hpp"
#include "terrain_slicer.hpp"

namespace hypervoxel {

template <std::size_t N, template <std::size_t> class G> class TerrainRenderer {

  G<N> terGen;
  std::unique_ptr<Line<N>[]> lines;
  std::size_t numThreads;
  std::unique_ptr<double[]> dists;
  std::unique_ptr<FacesManager<N>[]> facesManagers;
  std::unique_ptr<typename LineFollower<N, G>::Controller[]> controllers;
  std::unique_ptr<std::thread[]> threads;

  SliceDirs<4> sd;

public:
  /// pdists decreasing
  TerrainRenderer(G<N> &&terGen, std::size_t numThreads, double *pdists,
                  const SliceDirs<N> &sd)
      : terGen(terGen), lines(new Line<N>[((N * (N - 1)) / 2) *
                                          static_cast<std::size_t>(
                                              pdists[0] * pdists[0] *
                                                  (1 + sd.width2 * sd.width2 +
                                                   sd.height2 * sd.height2) +
                                              3 * pdists[0])]),
        numThreads(numThreads), dists(new double[numThreads]),
        facesManagers(new FacesManager<N>[numThreads]),
        controllers(new typename LineFollower<N, G>::Controller[numThreads]()),
        threads(new std::thread[numThreads]), sd(sd) {
    std::copy(pdists, pdists + numThreads, dists.get());
    double currDist = 0;
    double currVolume = 0;
    double multi = (N * N + 5 * N + 6) * std::sqrt(3) / 2;
    double baseAreaD = 4 * multi * sd.width2 * sd.height2 / 6;
    for (std::size_t i = numThreads; i--;) {
      double newDist = pdists[i];
      double newVolume = newDist * newDist * newDist * baseAreaD;
      facesManagers[i].~FacesManager();
      new (&facesManagers[i]) FacesManager<N>(
          static_cast<std::size_t>(newVolume - currVolume), sd.cam);
      threads[i] = std::thread(
          LineFollower<N, G>(currDist, newDist,
                             /*TODO: TerrainCache here*/ G<N>{terGen},
                             facesManagers[i], controllers[i]));
      currDist = newDist;
      currVolume = newVolume;
    }
  }

  ~TerrainRenderer() {
    for (std::size_t i = numThreads; i--;) {
      controllers[i].queue_op({nullptr, nullptr, nullptr, true});
      threads[i].join();
    }
  }

  float *writeTriangles(const SliceDirs<N> &nsd, float *out, float *out_fend) {
    sd = nsd;
    for (std::size_t i = numThreads; i--;) {
      facesManagers[i].setCam(&sd.cam[0]);
    }

    Line<N> *lines_end = getLines(sd, dists[0], lines.get());
    for (std::size_t i = numThreads; i--;) {
      controllers[i].queue_op({lines.get(), lines_end, &sd.cam[0], false});
    }
    for (std::size_t i = numThreads; i--;) {
      std::unique_lock<std::mutex> lock(controllers[i].this_mutex);
    }
    for (std::size_t i = numThreads; i--;) {
      out = facesManagers[i].fillVertexAttribPointer(sd, out, out_fend);
    }
    return out;
  }
};

} // namespace hypervoxel

#endif // TERRAIN_RENDERER_HPP_

