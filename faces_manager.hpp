#ifndef HYPERVOXEL_FACES_MANAGER_HPP_
#define HYPERVOXEL_FACES_MANAGER_HPP_

#include <unordered_map>
#include <utility>

#include "pool_linked_list.hpp"
#include "vector.hpp"

namespace hypervoxel {

template <std::size_t N> class FacesManager {

  struct Face {
    IVec<N> c;
    std::size_t dim;
  };

  struct FaceHash {

    std::size_t operator()(const Face &f) const noexcept {
      std::size_t basic = CoordHash<N>()(f.c);
      return (((basic << 32 - f.dim) | (basic >> f.dim)) ^ f.dim) + f.dim;
    }
  };

  struct Entry;

  typedef std::unordered_map<Face, Entry, FaceHash> umap;
  typedef PoolLinkedList<umap::iterator> plist;

  struct Entry {
    std::uint32_t color = 0;
    plist::Node *listEntry = nullptr;
  };

  umap map;
  plist list;
  std::size_t maxSize;
  DVec<N> cam;

  template <std::size_t D> void addFaces(const IVec<N> &coord, std::uint32_t val) {
    Face f1 {coord, D}, f2 {coord, D};
    if (cam[D] > coord[D]) {
      f1.c[D]++;
    } else {
      f2.c[D]++;
    }

    auto pp = map.emplace(f1, {});
    auto next = pp.first;
    if (pp.second) {
      next->second.color = val;
      next->second.listEntry = list.addToBeg(next);
    }

    if (!(~val & 0xFF)) {
      // this means opaque
      pp = map.emplace(f2, {});
      next = pp.first;
      if (!pp.second) {
        if (next->second.listEntry) {
          list.remove(next->second.listEntry);
          next->second.listEntry = nullptr;
        }
      }
    }

    addFaces<D + 1>(coord, val);
  }

  template<> void addFaces<N>(const IVec<N> &, std::uint32_t) { return; }

public:
  FacesManager(std::size_t maxSize, const DVec<N> &cam)
      : map(maxSize * 2), list(maxSize), maxSize(maxSize), cam(cam) {}

  bool addCube(const IVec<N> &coord, std::uint32_t val) {
    if (!val) {
      return true;
    }
    if (list.invSize() < N) {
      return false;
    }
    addFaces<0>(coord, val);
    return true;
  }
};

} // namespace hypervoxel

#endif // HYPERVOXEL_FACES_MANAGER_HPP_

