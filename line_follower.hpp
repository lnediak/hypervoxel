#ifndef HYPERVOXEL_LINE_FOLLOWER_HPP_
#define HYPERVOXEL_LINE_FOLLOWER_HPP_

#include <cmath>
#include <condition_variable>
#include <mutex>

#include "faces_manager.hpp"
#include "primitives.hpp"

namespace hypervoxel {

template <std::size_t N, class TerGen> class LineFollower {

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
  TerGen terGen;
  FacesManager<N> &out;

  Controller &controller;

  // helpers

  template <std::size_t M, class T = v::DVec<M>,
            class = typename T::thisisavvec,
            class = typename std::enable_if<
                std::is_same<typename T::value_type, double>::value &&
                T::size == M>::type>
  struct DVecSign1 {

    typedef void thisisavvec;
    typedef double value_type;
    static const std::size_t size = M;

    const T &vec;

    value_type operator[](std::size_t i) const {
      return vec[i] >= 0 ? 1 + 1e-14 : -1e-14;
    }
  };

public:
  LineFollower(double dist1, double dist2, TerGen &&terGenr, FacesManager<N> &out,
               Controller &controller)
      : dist1(dist1), dist2(dist2), dist1s(dist1 * dist1),
        dist2s(dist2 * dist2), terGen(std::move(terGenr)), out(out),
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
        if (lines->a3[2] > dist2 || lines->b3[2] < dist1) {
          continue;
        }
        v::DVec<N> a = lines->a;
        v::DVec<N> b = lines->b;
        v::DVec<3> a3 = lines->a3;
        v::DVec<3> b3 = lines->b3;

        v::DVec<N> df = b - a;
        v::DVec<3> df3 = b3 - a3;
        if (dist1 - a3[2] >= 1e-8) {
          double offset = (dist1 - a3[2]) / df3[2];
          a += df * offset;
          a3 += df3 * offset;
        }
        if (b3[2] - dist2 >= 1e-8) {
          double offset = (b3[2] - dist2) / df3[2];
          b -= df * offset;
          b3 -= df3 * offset;
        }

        v::IVec<N> coord = v::DVecFloor<N>{a};
        v::DVec<N> invdf = 1. / df;
        v::DVec<N> sigdf1 = DVecSign1<N>{invdf};

        if (v::min(invdf) >= 1e8) {
          continue;
        }
        double dist = v::min((v::toDVec(coord) - a + sigdf1) * invdf);
        v::DVec<N> pos = a + df * dist;
        v::DVec<3> ppos3 = a3 + df3 * dist;
        while (true) {
          double ndist = v::min((toDVec(coord) - pos + sigdf1) * invdf);
          dist += ndist + 1e-8;
          if (dist >= 1) {
            v::DVec<3> tmp = a3 + df3 * dist;
            out.addEdge(coord, lines->dim1, lines->dim2, ppos3, tmp, terGen);
            break;
          }
          if (ndist >= 1e-8) {
            v::DVec<3> tmp = a3 + df3 * dist;
            out.addEdge(coord, lines->dim1, lines->dim2, ppos3, tmp, terGen);
            ppos3 = tmp;
          }
          pos = a + df * dist;
          coord = v::DVecFloor<N>{pos};
        }
      }
      controller.queued_op = false;
    }
  }
};

} // namespace hypervoxel

#endif // HYPERVOXEL_LINE_FOLLOWER_HPP_

