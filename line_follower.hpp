#ifndef HYPERVOXEL_LINE_FOLLOWER_HPP_
#define HYPERVOXEL_LINE_FOLLOWER_HPP_

#include <cmath>
#include <condition_variable>
#include <mutex>

#include "faces_manager.hpp"
#include "terrain_slicer.hpp"

namespace hypervoxel {

template <std::size_t N, template <std::size_t> class G> class LineFollower {

public:
  struct Operation {
    const Line<N> *nlines, *nlines_end;
    const double *ncam;
    bool term;
  };

  struct Controller {
    std::mutex this_mutex;
    std::condition_variable cond_var;

    bool queued_op;
    Operation op;

    Controller() : queued_op(false), op{nullptr, nullptr, nullptr, false} {}

    void queue_op(Operation op) {
      std::unique_lock<std::mutex> lock(this_mutex);
      queued_op = true;
      this->op = op;
      lock.unlock();
      cond_var.notify_one();
    }
  };

private:
  const Line<N> *lines = nullptr, *lines_end = nullptr;
  double dist1, dist2;
  double dist1s, dist2s;
  G<N> terGen;
  FacesManager<N> &out;

  Controller &controller;

  // helpers

  template <std::size_t M, class A, class = typename A::thisisavvec>
  struct DVecFrom {

    typedef void thisisavvec;
    typedef double value_type;
    static const std::size_t size = M;

    const A &a;

    value_type operator[](std::size_t i) const { return a[i]; }
  };

  template <class A> DVecFrom<A::size, A> toDVec(const A &a) { return {a}; }

  template <std::size_t M> struct DVecFloor {

    typedef void thisisavvec;
    typedef std::int32_t value_type;
    static const std::size_t size = M;

    const v::DVec<M> &a;

    value_type operator[](std::size_t i) const {
      double val = a[i];
      value_type toreturn = val;
      toreturn -= (toreturn > val);
      return toreturn;
    }
  };
  template <std::size_t M> struct DVecAbs {

    typedef void thisisavvec;
    typedef double value_type;
    static const std::size_t size = M;

    const v::DVec<M> &a;

    value_type operator[](std::size_t i) const {
      return a[i] >= 0 ? a[i] : -a[i];
    }
  };

  std::size_t minInd(const v::DVec<N> &a, double *out) {
    double val = std::numeric_limits<double>::infinity();
    std::size_t ind = -1;
    for (std::size_t i = N; ind--;) {
      if (a[i] < val) {
        val = a[i];
        ind = i;
      }
    }
    *out = val;
    return ind;
  }

public:
  LineFollower(double dist1, double dist2, G<N> &&terGen, FacesManager<N> &out,
               Controller &controller)
      : dist1(dist1), dist2(dist2), dist1s(dist1 * dist1),
        dist2s(dist2 * dist2), terGen(terGen), out(out),
        controller(controller) {}

  LineFollower(const LineFollower &) = delete;
  LineFollower(LineFollower &&) = default;
  LineFollower &operator=(const LineFollower &) = delete;
  LineFollower &operator=(LineFollower &&) = default;

  void operator()() {
    std::unique_lock<std::mutex> lock(controller.this_mutex);
    while (true) {
      bool shouldTerm = false;
      controller.cond_var.wait(lock, [this, &shouldTerm]() -> bool {
        if (controller.queued_op) {
          if (controller.op.term) {
            return shouldTerm = true;
          }
          lines = controller.op.nlines;
          lines_end = controller.op.nlines_end;
          out.clear();
          out.setCam(controller.op.ncam);
          return true;
        }
        return false;
      });
      if (shouldTerm) {
        return;
      }
      for (; lines != lines_end; ++lines) {
        if (lines->arr > dist2 || lines->brr < dist1) {
          continue;
        }
        v::DVec<N> a = lines->a;
        v::DVec<N> b = lines->b;
        v::DVec<N> df = b - a;
        if (lines->arr < dist1s) {
          a += df *
               (std::sqrt(lines->pdiscr - lines->dfdf * dist1s) - lines->adf) /
               lines->dfdf;
        }
        if (lines->brr > dist2s) {
          b += df *
               (std::sqrt(lines->pdiscr - lines->dfdf * dist2s) - lines->bdf) /
               lines->dfdf;
        }
        v::IVec<N> coord = DVecFloor<N>{a};
        v::DVec<N> pos = a;
        v::DVec<N> invdf = DVecAbs<N>{1. / df};
        double dist = 0;
        while (dist < 1) {
          v::DVec<N> offs = (toDVec(coord + 1) - pos) * invdf;
          double ndist = v::min(offs);
          dist += ndist + 1e-8;
          pos = a + df * dist;
          coord = DVecFloor<N>{pos};
          if (ndist >= 1e-8) {

            for (std::size_t i = 0; i < N; i++) std::cout << coord[i] << " ";
            std::cout << std::endl;

            out.addCube(coord, terGen(coord));
            coord[lines->dim1]--;
            out.addCube(coord, terGen(coord));
            coord[lines->dim2]--;
            out.addCube(coord, terGen(coord));
            coord[lines->dim1]++;
            out.addCube(coord, terGen(coord));
            // coord[lines->dim2]++;
          }
        }
      }
      controller.queued_op = false;
    }
  }
};

} // namespace hypervoxel

#endif // HYPERVOXEL_LINE_FOLLOWER_HPP_

