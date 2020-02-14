#include <CL/cl_types.h>
#include <cmath>
#include <vector>

class TerrainGenerator {

  unsigned seed;
  std::size_t numDims;
  std::vector<cl_float> gradVecs;
  static const int numGradVecs = 1024;

  static std::size_t hash(unsigned seed, const std::vector<int> &location, int sidelength) {
    // TODO: do this
  }

public:
  TerrainGenerator(unsigned seed, std::size_t numDims)
      : seed(seed), numDims(numDims) {
    gradVecs.resize(numGradVecs * numDims);
    std::mt19937 mtrand(seed);
    double pi2 = 2 * 3.141592653589792653589793238462643383;
    // assuming numGradVecs is even (which it is)
    for (std::size_t i = 0; i < numGradVecs * numDims; i += 2) {
      double u1 = (1 - mtrand() / 2147483648.0);
      double u2 = (1 - mtrand() / 2147483648.0);
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
  }

  void generateTerrain(cl_uint *octree, unsigned seed, std::vector<int> origin, int sidelength) const {
    
  }

  void generateTerrain(cl_uint *octree, unsigned seed, std::vector<int> origin, std::vector<int> location, int sidelength, std::vector<double> polynomial) const {
    
  }
};
