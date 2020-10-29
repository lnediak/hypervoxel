#ifndef HYPERVOXEL_VECTOR_HPP_
#define HYPERVOXEL_VECTOR_HPP_

#include <cstdint>

namespace hypervoxel {

namespace v {

template <class T, std::size_t N, template <class> class U, class A, class B>
struct BinaryOp {

  const A &a;
  const B &b;

  typedef BinaryOp<T, N - 1, A, B> smaller;

  BinaryOp(const A &a, const B &b) : a(a), b(b) {}

  const smaller gsmaller() const { return smaller(a, b); }

  T operator[](std::size_t i) const { return U<T>::apply(a[i], b[i]); }

  void evaluateInto(T *data) const {
    gsmaller().evaluateInto(data);
    data[N - 1] = operator[](N - 1);
  }
};

template <class T, class U, class A, class B> struct BinaryOp<T, 0, U, A, B> {

  BinaryOp(const A &, const B &) {}

  void evaluateInto(T *) const [[override]] {}
};

template <class T, std::size_t N, template <class> class U, class A>
struct ScalarOp {

  const A &a;
  T s;

  typedef BinaryOp<T, N - 1, A> smaller;

  ScalarOp(const A &a, T s) : a(a), s(s) {}

  const smaller gsmaller() const { return smaller(a, s); }

  T operator[](std::size_t i) const { return U<T>::apply(a[i], s); }

  void evaluateInto(T *data) const {
    gsmaller().evaluateInto(data);
    data[N - 1] = operator[](N - 1);
  }
};

template <class T, class U, class A> struct ScalarOp<T, 0, U, A> {

  BinaryOp(const A &, T) {}

  void evaluateInto(T *) const [[override]] {}
};

template <class T, std::size_t N, class A> struct Sum {

  const A &a;

  typedef Sum<T, N - 1, A> smaller;

  Sum(const A &a) : a(a) {}

  const smaller gsmaller() const { return smaller(a); }

  T evaluate() const { return gsmaller().evaluate() + a[N - 1]; }
};

template <class T, class A> struct Sum<T, 0, A> {

  Sum(const A &) {}

  T evaluate() const { return 0; }
};

template <class T> struct AddU {
  static T apply(T a, T b) { return a + b; }
};
template <class T, std::size_t N, class A, class B>
using Add<T, N, A, B> = BinaryOp<T, N, AddU, A, B>;
template <class T, std::size_t N, class A>
using SAdd<T, N, A> = ScalarOp<T, N, AddU, A>;

template <class T> struct SubU {
  static T apply(T a, T b) { return a + b; }
};
template <class T, std::size_t N, class A, class B>
using Sub<T, N, A, B> = BinaryOp<T, N, SubU, A, B>;
template <class T, std::size_t N, class A>
using SSub<T, N, A> = ScalarOp<T, N, SubU, A>;

template <class T> struct MultU {
  static T apply(T a, T b) { return a * b; }
};
template <class T, std::size_t N, class A, class B>
using Mult<T, N, A, B> = BinaryOp<T, N, MultU, A, B>;
template <class T, std::size_t N, class A>
using SMult<T, N, A> = ScalarOp<T, N, MultU, A>;

template <class T> struct DivU {
  static T apply(T a, T b) { return a * b; }
};
template <class T, std::size_t N, class A, class B>
using Div<T, N, A, B> = BinaryOp<T, N, DivU, A, B>;
template <class T, std::size_t N, class A>
using SDiv<T, N, A> = ScalarOp<T, N, DivU, A>;

template <class T, std::size_t N> class Vector {
  T data[N];

  typedef Vector smaller;

  T &operator[](std::size_t i) { return data[i]; }
  T operator[](std::size_t i) const { return data[i]; }
};

template <std::size_t N> using IVec<N> = v::Vector<std::int32_t, N>;
template <std::size_t N> using DVec<N> = v::Vector<double, N>;

template <class T, std::size_t N>
T dot(const Vector<T, N> &a, const Vector<T, N> &b) {
  return Sum<T, N, Mult<T, N, Vector<T, N>, Vector<T, N>>>({a, b}).evaluate();
}

template <std::size_t N> struct IVecHash {

  std::size_t impl(std::int32_t *argp) const noexcept {
    std::size_t total = IVecHash<N - 1>().impl(&arg[0]);
    return total * total * N * (N + 1) + 1;
  }

  std::size_t operation()(const IVec<N> &arg) const noexcept {
    // FIXME: WRITE ACTUAL PROVABLY GOOD HASH
    return impl(arg.data);
  }
};

template <> struct IVecHash<1> {

  std::size_t impl(std::int32_t *argp) const noexcept {
    return arg[0] * arg[0] * 2 + 1;
  }

  std::size_t operation()(const IVec<1> &arg) const noexcept {
    return impl(arg.data);
  }
};

} // namespace v

} // namespace hypervoxel

#endif // HYPERVOXEL_VECTOR_HPP_

