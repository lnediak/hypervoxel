#include "ray_follower.hpp"

int main() {
  // We'll just do a very simple 2-dimensional test to see how this works...
  struct Op {
    bool operator()(const hypervoxel::v::IVec<2> &v) {
      std::cout << v << std::endl;
      return false;
    }
  } op;
  hypervoxel::SliceDirs<2> sd = {{0, 0}, {0, 1}, {1, 0}, {0, 0}, 10, 1, 1};
  hypervoxel::RayFollower<2, Op &> rf(sd, op, 1, 2);
  rf();
}

