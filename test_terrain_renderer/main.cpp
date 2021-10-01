#include <iostream>

#include <lodepng/lodepng.h>

#include "terrain_renderer.hpp"

int main() {
  const std::size_t numGradVecs = 4096;
  const std::size_t N = 5;
  hypervoxel::GeneratorPerlin<N> terGen;
  hypervoxel::SliceDirs<5> sd{{0.1f, 0.1f, 0.1f, 0.1f, 0.1f},
                              {-0.2626f, 0.3939f, 0.5252, 0.2626, 0.6565},
                              {0.1426f, -0.02, 0.1025, -0.9182, 0.3544},
                              {0.8233, -0.3495, 0.1244, 0.2764, 0.329},
                              200.f,
                              1.5f,
                              1.5f};
  const std::size_t width = 1024;
  const std::size_t height = 1024;
  hypervoxel::TerrainRenderer terRen(
      sd.rm, sd.um, sd.fm, width, height,
      {{{32, 32, 32, 32, 32},
        hypervoxel::getGradVecs(numGradVecs, N, 2),
        numGradVecs - 1,
        3,
        0.5}});
  std::vector<unsigned char> img(width * height * 4);

  auto now = std::chrono::high_resolution_clock::now();
  terRen.render(sd, (cl_uchar *)&img[0]);
  std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(
                   std::chrono::high_resolution_clock::now() - now)
                   .count()
            << "ms" << std::endl;

  unsigned error = lodepng::encode("result.png", img, width, height);
  if (error) {
    std::cout << "encoder error " << error << ": " << lodepng_error_text(error)
              << std::endl;
  }
}

