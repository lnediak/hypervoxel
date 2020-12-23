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
  template <std::size_t M> struct DVecSign1 {

    typedef void thisisavvec;
    typedef double value_type;
    static const std::size_t size = M;

    const v::DVec<M> &a;

    value_type operator[](std::size_t i) const {
      return a[i] >= 0 ? 1 + 1e-14 : -1e-14;
    }
  };
  template <std::size_t M> struct DVecSign2 {

    typedef void thisisavvec;
    typedef double value_type;
    static const std::size_t size = M;

    const v::DVec<M> &a;

    value_type operator[](std::size_t i) const {
      return a[i] >= 0 ? 1 : -1;
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
        if (lines->arr > dist2s || lines->brr < dist1s) {
          continue;
        }
        v::DVec<N> a = lines->a;
        v::DVec<N> b = lines->b;
        v::DVec<3> a3 = lines->a3;
        v::DVec<3> b3 = lines->b3;

        v::DVec<N> df = b - a;
        v::DVec<3> df3 = b3 - a3;
        if (dist1s - lines->arr >= 1e-8) {
          a += df *
               (std::sqrt(lines->pdiscr + lines->dfdf * dist1s + 1e-12) - lines->adf) /
               lines->dfdf;
        }
        if (lines->brr - dist2s >= 1e-8) {
          b += df *
               (std::sqrt(lines->pdiscr + lines->dfdf * dist2s + 1e-12) - lines->bdf) /
               lines->dfdf;
        }

        v::IVec<N> coord = DVecFloor<N>{a};
        v::DVec<N> pos = a;
        v::DVec<N> invdf = 1. / df;
        v::DVec<N> sigdf1 = DVecSign1<N>{invdf};
        v::DVec<3> ppos3 = a3;
        double dist = 0;
        while (true) {
          v::DVec<N> offs = (toDVec(coord) - pos + sigdf1) * invdf;
          double ndist = v::min(offs);
          dist += ndist + 1e-8;
          if (dist >= 1) {
            out.addEdge(coord, lines->dim1, lines->dim2, ppos3, b3, terGen);
            break;
          }
          if (ndist >= 1e-8) {
            v::DVec<3> tmp = a3 + df3 * dist;
            out.addEdge(coord, lines->dim1, lines->dim2, ppos3, tmp, terGen);
            ppos3 = tmp;
          }
          pos = a + df * dist;
          coord = DVecFloor<N>{pos};
        }
      }
      controller.queued_op = false;
    }
  }
};

} // namespace hypervoxel

#endif // HYPERVOXEL_LINE_FOLLOWER_HPP_

