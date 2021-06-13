#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <iostream>
#include <random>

#define GLAD_GL_IMPLEMENTATION
#include "gl_program.hpp"

#include "terrain_renderer.hpp"

namespace {

void errCallback(int, const char *err) { std::cerr << err << std::endl; }

} // namespace

/// numGradVecs needs to be a power of 2
std::unique_ptr<double[]> getGradVecs(std::size_t numGradVecs,
                                      std::size_t numDims, unsigned seed) {
  std::unique_ptr<double[]> gradVecs(new double[numGradVecs * numDims]);
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

std::unique_ptr<cl_float8[]> convertGradVecs(double *g,
                                             std::size_t numGradVecs) {
  std::unique_ptr<cl_float8[]> toret(new cl_float8[numGradVecs]);
  for (std::size_t i = 0; i < numGradVecs; i++) {
    toret[i].s[0] = g[i * 5 + 0];
    toret[i].s[1] = g[i * 5 + 1];
    toret[i].s[2] = g[i * 5 + 2];
    toret[i].s[3] = g[i * 5 + 3];
    toret[i].s[4] = g[i * 5 + 4];
  }
  return toret;
}

int main() {
  glfwSetErrorCallback(&errCallback);
  if (!glfwInit()) {
    std::cout << "GLFW Initialization Failed." << std::endl;
    return 1;
  }
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
  const std::size_t width = 1000, height = 1000;
  GLFWwindow *window =
      glfwCreateWindow(width, height, "test lel", nullptr, nullptr);
  if (!window) {
    std::cout << "GLFW window creation failed." << std::endl;
    return 1;
  }
  glfwMakeContextCurrent(window);
  if (!gladLoadGL((GLADloadfunc)glfwGetProcAddress)) {
    std::cout << "GLAD loading failed." << std::endl;
  }
  glClearColor(0.f, 0.f, 0.f, 1.f);
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  // double pdists[] = {25, 23.91, 22.714, 21.375, 19.843, 18.028,
  // 15.749, 12.5}; double pdists[] = {25, 22.714, 19.843, 15.749}; double
  // pdists[] = {50, 47.82, 45.428, 42.75, 39.686, 36.056, 31.498, 25}; double
  // pdists[] = {50, 45.428, 39.686, 31.498};
  double pdists[] = {5};
  double sq12 = std::sqrt(.5);
  /*
  hypervoxel::SliceDirs<5> sd = {{0.1, 0.1, 0.1, 0.1, 0.1},
                                 {1, 0, 0, 0, 0},
                                 {0, sq12, 0, .5, -.5},
                                 {0, 0, sq12, .5, .5},
                                 1,
                                 1};
  */
  hypervoxel::SliceDirs<5> sd = {{0.1, 0.1, 0.1, 0.1, 0.1},
                                 {0, 0, sq12, -sq12, 0},
                                 {.5, .5, -.5, -.5, 0},
                                 {sq12, -sq12, 0, 0, 0},
                                 1,
                                 1};
  /*
  hypervoxel::SliceDirs<4> sd = {{0.1, 0.1, 0.1, 0.1},
                                 {1, 0, 0, 0},
                                 {0, 1, 0, 0},
                                 {0, 0, 1, 0},
                                 1,
                                 1};
  */
  std::size_t numGradVecs = 4096;
  unsigned seed = 2;
  hypervoxel::TerrainRenderer renderer(12900000, 1, 1, pdists[0]);
  renderer.setPerlinOptions(
      {512, 512, 512, 512, 512}, 4, 0.5,
      convertGradVecs(getGradVecs(numGradVecs, 5, seed).get(), numGradVecs)
          .get());
  const std::size_t lenTriangles = 18 * 1048576;
  std::unique_ptr<float[]> triangles(new float[lenTriangles]);
  GLProgram prog;
  prog.compileProgram(lenTriangles);
  prog.setProjMat(sd.width2, sd.height2, pdists[0] - 2);

  std::size_t fpsCount = 0;
  auto beg = std::chrono::high_resolution_clock::now();
  while (!glfwWindowShouldClose(window)) {
    glfwSwapBuffers(window);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glfwPollEvents();

    // std::cout << "NEW FRAME" << std::endl << std::endl << std::endl <<
    // std::endl << std::endl;

    float *tmpend =
        renderer.generateTriangles(sd, 0, pdists[0], triangles.get());
    prog.renderTriangles(triangles.get(), tmpend);
    sd.cam += 0.01;
    // sd.cam[3] -= 0.01;

    auto dur = std::chrono::high_resolution_clock::now() - beg;
    double secs =
        std::chrono::duration_cast<std::chrono::duration<double>>(dur).count();
    fpsCount++;
    if (secs >= 1) {
      std::cout << fpsCount << "fps" << std::endl;
      fpsCount = 0;
      beg = std::chrono::high_resolution_clock::now();
    }
  }
}

