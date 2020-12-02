#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <iostream>

#define GLAD_GL_IMPLEMENTATION
#include "gl_program.hpp"

#include "terrain_generator_tester.hpp"
#include "terrain_renderer.hpp"

namespace {

void errCallback(int, const char *err) { std::cerr << err << std::endl; }

} // namespace

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

  double pdists[] = {25, 23.91, 22.714, 21.375, 19.843, 18.028, 15.749, 12.5};
  double sq12 = std::sqrt(.5);
  hypervoxel::SliceDirs<4> sd = {{0, 0, 0, 0},
                                 {sq12, -sq12, 0, 0},
                                 {0, 0, sq12, -sq12},
                                 {.5, .5, -.5, -.5},
                                 1,
                                 1};
  hypervoxel::TerrainRenderer<4, hypervoxel::TerrainGeneratorTester> renderer(
      hypervoxel::TerrainGeneratorTester<4>{2, 1}, 8, pdists, sd);
  const std::size_t lenTriangles = 21 * 65536;
  std::unique_ptr<float[]> triangles(new float[lenTriangles]);
  float *triangles_end = triangles.get() + lenTriangles;
  GLProgram prog;
  prog.compileProgram(lenTriangles);
  prog.setProjMat(sd.width2, sd.height2, pdists[0]);

  std::size_t fpsCount = 0;
  auto beg = std::chrono::high_resolution_clock::now();
  while (!glfwWindowShouldClose(window)) {
    glfwSwapBuffers(window);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glfwPollEvents();

    float *tmpend = renderer.writeTriangles(sd, triangles.get(), triangles_end);
    prog.renderTriangles(triangles.get(), tmpend);

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
