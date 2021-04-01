#ifndef HYPERVOXEL_UTIL_RENDER_INT_BUFFER_HPP_
#define HYPERVOXEL_UTIL_RENDER_INT_BUFFER_HPP_

#include "glad/gl.h"

#include <memory>
#include <new>
#include <vector>

typedef GLint int32;

template <std::size_t M> struct BufStructure {
  static constexpr std::size_t numBins = 7;
  static constexpr int32 sizes[numBins] = {1024,  2048,  4096, 8192,
                                           16384, 32768, 65536};
  static constexpr std::size_t counts[numBins] = {4096, 2048, 1024, 512,
                                                  256,  128,  64};
  static constexpr std::size_t total = 29360128;

  static std::size_t getBini(int32 sz) {
    if (sz > sizes[3]) {
      if (sz > sizes[5]) {
        if (sz > sizes[6]) {
          return 7;
        }
        return 6;
      }
      if (sz > sizes[4]) {
        return 5;
      }
      return 4;
    }
    if (sz > sizes[1]) {
      if (sz > sizes[2]) {
        return 3;
      }
      return 2;
    }
    if (sz > sizes[0]) {
      return 1;
    }
    return 0;
  }
};

template <std::size_t M> struct RenderBufferState {

  struct IntStack {
    std::size_t sz;
    std::unique_ptr<int32[]> p;

    void push(int32 i) { p[sz++] = i; }

    bool pop(int32 *out) {
      if (sz) {
        *out = p[--sz];
        return true;
      }
      return false;
    }
  };

  struct BufDetail {
    GLuint bufn;
    int32 size; /// in bytes
    IntStack binstacks[BufStructure<M>::numBins];

    explicit BufDetail(int32 sz) {
      if (sz) {
        size = sz;
      } else {
        size = BufStructure<M>::total;
        int32 ci = 0;
        for (std::size_t i = 0; i < BufStructure<M>::numBins; i++) {
          const std::size_t coui = BufStructure<M>::counts[i];
          binstacks[i].sz = coui;
          int32 *sp;
          binstacks[i].p.reset(sp = new int32[coui]);
          for (std::size_t j = coui; j--;) {
            sp[j] = ci;
            ci += BufStructure<M>::sizes[i];
          }
        }
      }

      glGenBuffers(1, &bufn);
      glBindBuffer(GL_ARRAY_BUFFER, bufn);
      glBufferData(GL_ARRAY_BUFFER, size, NULL, GL_DYNAMIC_DRAW);
      GLenum err;
      while ((err = glGetError()) != GL_NO_ERROR) {
        if (err == GL_OUT_OF_MEMORY) {
          throw std::bad_alloc();
        }
      }
    }

    void swapWith(BufDetail &a) {
      if (this == &a) {
        return;
      }
      GLuint tmp = bufn;
      bufn = a.bufn;
      a.bufn = tmp;
      int32 tmp0 = size;
      size = a.size;
      a.size = tmp0;
      std::swap(binstacks, a.binstacks);
    }

    BufDetail(BufDetail &&) = delete;
    BufDetail &operator=(BufDetail &&) = delete;

    ~BufDetail() {
      if (bufn) {
        glDeleteBuffers(1, &bufn);
        bufn = 0;
      }
    }
  };

  /// bufs[1] is for buffers that are too big
  std::vector<BufDetail> bufs[2];

  static RenderBufferState &getState() {
    static RenderBufferState singleton;
    return singleton;
  }
};

template <std::size_t M> class RenderIntBuffer {

  struct ActiveFlag {
    bool a = true;

    ActiveFlag(const ActiveFlag &) = delete;
    ActiveFlag &operator=(const ActiveFlag &) = delete;
    ActiveFlag(ActiveFlag &&b) : a(b.a) { b.a = false; }
    ActiveFlag &operator=(ActiveFlag &&b) {
      a = b.a;
      b.a = false;
      return *this;
    }
    ~ActiveFlag() { a = false; }
  } flag;

  bool big;
  std::size_t bufsi, bini;
  GLuint bufn;
  int32 starti;
  std::size_t numVertices;
  int32 sz, trusz;

public:
  RenderIntBuffer(const int32 *data, std::size_t numVertices)
      : numVertices(numVertices), sz(numVertices * M * sizeof(int32)) {
    bini = BufStructure<M>::getBini(sz);
    big = bini == BufStructure<M>::numBins;
    if (big) {
      auto &bufs = RenderBufferState<M>::getState().bufs[1];
      bufsi = bufs.size();
      starti = 0;
      trusz = sz;
      bufs.emplace_back(sz);
      bufn = bufs[bufsi].bufn;
    } else {
      trusz = BufStructure<M>::sizes[bini];
      auto &bufs = RenderBufferState<M>::getState().bufs[0];
      std::size_t bsz = bufs.size();
      for (std::size_t bi = 0; bi < bsz; bi++) {
        if (bufs[bi].binstacks[bini].pop(&starti)) {
          bufsi = bi;
          bufn = bufs[bi].bufn;
          return;
        }
      }
      bufs.emplace_back(0);
      bufsi = bsz;
      bufs[bufsi].binstacks[bini].pop(&starti);
      bufn = bufs[bufsi].bufn;
    }
  }

  ~RenderIntBuffer() {
    if (!flag.a) {
      return;
    }
    if (big) {
      auto &bufs = RenderBufferState<M>::getState().bufs[1];
      bufs[bufsi].swapWith(bufs.back());
      bufs.pop_back();
    } else {
      RenderBufferState<M>::getState().bufs[0][bufsi].binstacks[bini].push_back(
          starti);
    }
  }

  void draw() const {
    // TODO: Kek....
  }
};

#endif // HYPERVOXEL_UTIL_RENDER_INT_BUFFER_HPP_

