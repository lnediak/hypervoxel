#include <stdio.h>

#include "generator_perlin.hpp"

int main() {
  /* hypervoxel::v::DVec<4> a{1, 2, 7, 4};
  hypervoxel::v::DVec<2> lerp{0.2, 0.3};
  std::cout << hypervoxel::vlerpAll(a, lerp) << std::endl;
  return 0; */
  const std::size_t numGradVecs = 4096;
  const std::size_t N = 5;
  hypervoxel::GeneratorPerlin<N> terGen{
      {{24, 32, 32, 32, 32},
       hypervoxel::getGradVecs(numGradVecs, N, 1),
       numGradVecs - 1,
       3,
       0.5}};
  for (int r = 0; r < 200; r += 1) {
    for (int c = 0; c < 200; c += 1) {
      // printf("%+.2d ", (int)(10 * terGen.get({r, c})));
      // printf("%+.2f ", (6 * terGen.get({r, c})));
      printf("%s", terGen.get({r, c, 0, 0, 0}) > 0 ? "0" : " ");
    }
    printf("\n");
  }
}

