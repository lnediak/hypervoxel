#ifndef HYPERVOXEL_LINE_FOLLOWER_HPP_
#define HYPERVOXEL_LINE_FOLLOWER_HPP_

#include <mutex>

#include "faces_manager.hpp"
#include "terrain_slicer.hpp"

namespace hypervoxel {

template <std::size_t N, class G> class LineFollower {

  Line *lines = nullptr, *lines_end = nullptr;
  std::size_t dist1, dist2;
  G<N> terGen;
  FacesManager &out;

  std::mutex this_mutex;
  std::condition_variable cond_var;

  /*
  struct Operation {
    Line *nlines, *nlines_end;
    v::DVec<N> cam;
  };
  ConcurrentQueue<Operation> queue;
  */

  bool queued_op;
  Line *nlines, *nlines_end;
  const double *cam;

public:
  LineFollower(std::size_t dist1, std::size_t dist2, G<N> &&terGen,
               FacesManager &out)
      : dist1(dist1), dist2(dist2), terGen(terGen), out(out), queued_op(false) {
  }

  void queueOp(Line *lines, Line *lines_end, const double *cam) {
    std::unique_lock<std::mutex> lock(this_mutex);
    queued_op = true;
    nlines = lines;
    nlines_end = lines_end;
    ncam = cam;
    lock.unlock();
    cond_var.notify_one();
  }

  void operator()() {
    while (true) {
      std::unique_lock<std::mutex> lock(this_mutex);
      cond_var.wait(lock, [this]() -> bool {
        if (queued_op) {
          lines = nlines;
          lines_end = nlines_end;
          out.clear();
          out.setCam(cam);
          return true;
        }
        return false;
      });
      for (; lines != lines_end; ++lines) {
        if (lines->arr > dist2 || lines->brr < dist1) {
          continue;
        }
        v::DVec<N> a = lines->a;
        v::DVec<N> b = lines->b;
        v::DVec<N> df = b - a;
        if (lines->arr > dist1) {
          a += df * (lines->arr - dist1);
        }
        if (lines->brr < dist2) {
          b += df * (lines->brr - dist2);
        }
        template <std::size_t N> struct DVecFloor {

          typedef void thisisavec;
          typedef std::int32_t value_type;
          static const std::size_t size = N;

          const v::DVec<N> &a;

          value_type operator[](std::size_t i) {
            double val = a[i];
            value_type toreturn = val;
            toreturn -= (toreturn > val);
            return toreturn;
          }
        };
        template <std::size_t N> struct DVecSign {

          typedef void thisisavec;
          typedef std::int32_t value_type;
          static const std::size_t size = N;

          const v::DVec<N> &a;

          value_type operator[](std::size_t i) { return (a[i] > 0) * 2 - 1; }
        };
        template <std::size_t N>
        std::size_t minInd(const v::DVec<N> &a, double *out) {
          std::size_t ind = minInd<N - 1>(a, out);
          if (a[N - 1] < *out) {
            *out = a[N - 1];
            return N - 1;
          }
          return ind;
        }
        template <> std::size_t minInd<0>(const v::DVec<N> &, double *out) {
          out = std::numeric_limits<double>::max();
          return -1;
        }
        v::IVec<N> coord = DVecFloor<N>{a};
        v::DVec<N> pos = a;
        v::DVec<N> invdf = v::ConstantVec<int, 1, N>() / df;
        v::IVec<N> dfdir = DVecSign<N>{df};
        double dist = 0;
        while (dist < 1) {
          v::DVec<N> offs = (coord + 1 - pos) * invdf;
          double ndist;
          std::size_t ind = minInd<N>(offs, &ndist);
          dist += ndist + 1e-8;
          pos = a + df * dist;
          coord[ind] = DVecFloor<N>{pos};
          if (ndist >= 1e-8) {
            out.addCube(coord, terGen(coord));
            coord[lines->dim1]--;
            out.addCube(coord, terGen(coord));
            coord[lines->dim2]--;
            out.addCube(coord, terGen(coord));
            coord[lines->dim1]++;
            out.addCube(coord, terGen(coord));
            coord[lines->dim2]++;
          }
        }
      }
      queued_op = false;
    }
  }
};

} // namespace hypervoxel

#endif // HYPERVOXEL_LINE_FOLLOWER_HPP_

