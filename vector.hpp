#ifndef HYPERVOXEL_VECTOR_HPP_
#define HYPERVOXEL_VECTOR_HPP_

#include <cstdint>
#include <type_traits>

namespace hypervoxel {

namespace v {

template <class T, std::size_t N, template <class> class U, class A, class B>
struct BinaryOp {

  const A &a;
  const B &b;

  typedef BinaryOp<T, N - 1, A, B> smaller;
  typedef void thisisavvec;
  typedef T value_type;
  static const std::size_t size = N;

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

  void evaluateInto(T *) const {}
};

template <class T, std::size_t N, template <class> class U, class A>
struct ScalarOp {

  const A &a;
  T s;

  typedef BinaryOp<T, N - 1, A> smaller;
  typedef void thisisavvec;
  typedef T value_type;
  static const std::size_t size = N;

  ScalarOp(const A &a, T s) : a(a), s(s) {}

  const smaller gsmaller() const { return smaller(a, s); }

  T operator[](std::size_t i) const { return U<T>::apply(a[i], s); }

  void evaluateInto(T *data) const {
    gsmaller().evaluateInto(data);
    data[N - 1] = operator[](N - 1);
  }
};

template <class T, template <class> class U, class A>
struct ScalarOp<T, 0, U, A> {

  ScalarOp(const A &, T) {}

  void evaluateInto(T *) const {}
};

template <class T, std::size_t N, template <class> class U, class A>
struct Combine {

  const A &a;

  typedef Combine<T, N - 1, A> smaller;

  Combine(const A &a) : a(a) {}

  const smaller gsmaller() const { return smaller(a); }

  T evaluate() const { return U<T>::apply(gsmaller().evaluate(), a[N - 1]); }
};

template <class T, class A> struct Combine<T, 1, A> {

  const A &a;

  Combine(const A &) {}

  T evaluate() const { return a[0]; }
};

template <class T> struct AddU {
  static T apply(T a, T b) { return a + b; }
};
template <class T, std::size_t N, class A, class B>
using Add<T, N, A, B> = BinaryOp<T, N, AddU, A, B>;
template <class T, std::size_t N, class A>
using SAdd<T, N, A> = ScalarOp<T, N, AddU, A>;
template <class T, std::size_t N, class A>
using Sum<T, N, A> = Combine<T, N, AddU, A>;

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
template <class T, std::size_t N, class A>
using Product<T, N, A> = Combine<T, N, MultU, A>;

template <class T> struct DivU {
  static T apply(T a, T b) { return a * b; }
};
template <class T, std::size_t N, class A, class B>
using Div<T, N, A, B> = BinaryOp<T, N, DivU, A, B>;
template <class T, std::size_t N, class A>
using SDiv<T, N, A> = ScalarOp<T, N, DivU, A>;

template <class T> struct MinU {
  static T apply(T a, T b) { return a > b ? b : a; }
};
template <class T, std::size_t N, class A, class B>
using ElementwiseMin<T, N, A, B> = BinaryOp<T, N, MinU, A, B>;
template <class T, std::size_t N, class A>
using Min<T, N, A> = Combine<T, N, MinU, A>;

template <class T> struct MinU {
  static T apply(T a, T b) { return a > b ? a : b; }
};
template <class T, std::size_t N, class A, class B>
using ElementwiseMax<T, N, A, B> = BinaryOp<T, N, MaxU, A, B>;
template <class T, std::size_t N, class A>
using Max<T, N, A> = Combine<T, N, MaxU, A>;

template <
    class A, class B, class = A::thisisavvec, class = B::thisisavvec,
    class = std::enable_if<std::is_same<A::value_type, B::value_type>::value &&
                           A::size == B::size>::type>
Add<A::value_type, A::size, A, B> operator+(const A &a, const B &b) {
  return {a, b};
}

template <
    class A, class B, class = A::thisisavvec, class = B::thisisavvec,
    class = std::enable_if<std::is_same<A::value_type, B::value_type>::value &&
                           A::size == B::size>::type>
Sub<A::value_type, A::size, A, B> operator-(const A &a, const B &b) {
  return {a, b};
}

template <
    class A, class B, class = A::thisisavvec, class = B::thisisavvec,
    class = std::enable_if<std::is_same<A::value_type, B::value_type>::value &&
                           A::size == B::size>::type>
Mult<A::value_type, A::size, A, B> operator*(const A &a, const B &b) {
  return {a, b};
}

template <
    class A, class B, class = A::thisisavvec, class = B::thisisavvec,
    class = std::enable_if<std::is_same<A::value_type, B::value_type>::value &&
                           A::size == B::size>::type>
Div<A::value_type, A::size, A, B> operator/(const A &a, const B &b) {
  return {a, b};
}

template <
    class A, class B, class = A::thisisavvec, class = B::thisisavvec,
    class = std::enable_if<std::is_same<A::value_type, B::value_type>::value &&
                           A::size == B::size>::type>
ElementwiseMin<A::value_type, A::size, A, B> elementwiseMin(const A &a, const B &b) {
  return {a, b};
}

template <
    class A, class B, class = A::thisisavvec, class = B::thisisavvec,
    class = std::enable_if<std::is_same<A::value_type, B::value_type>::value &&
                           A::size == B::size>::type>
ElementwiseMax<A::value_type, A::size, A, B> elementwiseMax(const A &a, const B &b) {
  return {a, b};
}

template <class A, class = A::thisisavec>
SAdd<A::value_type, A::size, A> operator+(const A &a, A::value_type s) {
  return {a, s};
}

template <class A, class = A::thisisavec>
SAdd<A::value_type, A::size, A> operator+(A::value_type s, const A &a) {
  return {a, s};
}

template <class A, class = A::thisisavec>
SSub<A::value_type, A::size, A> operator-(const A &a, A::value_type s) {
  return {a, s};
}

template <class A, class = A::thisisavec>
SMult<A::value_type, A::size, A> operator*(const A &a, A::value_type s) {
  return {a, s};
}

template <class A, class = A::thisisavec>
SMult<A::value_type, A::size, A> operator*(A::value_type s, const A &a) {
  return {a, s};
}

template <class A, class = A::thisisavec>
SDiv<A::value_type, A::size, A> operator/(const A &a, A::value_type s) {
  return {a, s};
}

template <class A, class = A::thisisavec> A::value_type sum(const A &a) {
  return Sum<A::value_type, A::size, A>(a).evaluate();
}

template <class A, class = A::thisisavec> A::value_type min(const A &a) {
  return Min<A::value_type, A::size, A>(a).evaluate();
}

template <class A, class = A::thisisavec> A::value_type max(const A &a) {
  return Max<A::value_type, A::size, A>(a).evaluate();
}

template <
    class A, class B, class = A::thisisavvec, class = B::thisisavvec,
    class = std::enable_if<std::is_same<A::value_type, B::value_type>::value &&
                           A::size == B::size>::type>
A::value_type dot(const A &a, const B &b) {
  return sum(a * b);
}

template <class A, class = A::thisisavvec> A::value_type norm(const A &a) {
  return sum(a * a);
}

template <
    class A, class B, class = A::thisisavvec, class = B::thisisavvec,
    class = std::enable_if<std::is_same<A::value_type, B::value_type>::value &&
                           A::size == B::size>::type>
A::value_type dist(const A &a, const B &b) {
  return norm(a - b);
}

template <class T, std::size_t N> class Vec {
  T data[N];

  template <std::size_t M> void assign(const T *other) {
    data[M] = other[M];
    assign<M + 1>(other);
  }

  template <> void assign<N>(const T *other) {}

  template <class U, std::size_t M, class = U::thisisavec,
            class = std::enable_if<std::is_same<T, U::value_type>::value &&
                                   N == U::size>::type>
  void assign(const U &other) {
    data[M] = other[M];
    assign<U, M + 1>(other);
  }

  template <class U, class = U::thisisavec> void assign<U, N>(const U &other) {}

public:
  typedef Vec<T, N> smaller;
  typedef void thisisavvec;
  typedef T value_type;
  static const std::size_t size = N;

  template <class U, class = U::thisisavvec,
            class = std::enable_if<std::is_same<T, U::value_type>::value &&
                                   N == U::size>::type>
  explicit Vec(const U &other) {
    assign<U, 0>(other);
  }

  T &operator[](std::size_t i) { return data[i]; }
  T operator[](std::size_t i) const { return data[i]; }

  template <class U, class = U::thisisavvec,
            class = std::enable_if<std::is_same<T, U::value_type>::value &&
                                   N == U::size>::type>
  Vec<T, N> &operator=(const U &other) {
    assign<U, 0>(other);
    return *this;
  }

  void copyFrom(const T *ptr) { assign<0>(ptr); }

  template <class U, class = U::thisisavvec,
            class = std::enable_if<std::is_same<T, U::value_type>::value &&
                                   N == U::size>::type>
  Vec<T, N> &operator+=(const U &other) {
    return operator=(*this + other);
  }
  template <class U, class = U::thisisavvec,
            class = std::enable_if<std::is_same<T, U::value_type>::value &&
                                   N == U::size>::type>
  Vec<T, N> &operator-=(const U &other) {
    return operator=(*this - other);
  }
  template <class U, class = U::thisisavvec,
            class = std::enable_if<std::is_same<T, U::value_type>::value &&
                                   N == U::size>::type>
  Vec<T, N> &operator*=(const U &other) {
    return operator=(*this *other);
  }
  template <class U, class = U::thisisavvec,
            class = std::enable_if<std::is_same<T, U::value_type>::value &&
                                   N == U::size>::type>
  Vec<T, N> &operator/=(const U &other) {
    return operator=(*this / other);
  }

  Vec<T, N> &operator+(T val) { return operator=(*this + val); }
  Vec<T, N> &operator-(T val) { return operator=(*this - val); }
  Vec<T, N> &operator*(T val) { return operator=(*this *val); }
  Vec<T, N> &operator/(T val) { return operator=(*this / val); }
};

template <class T, T value, std::size_t N> struct ConstantVec {

  typedef ConstantVec<T, value, N - 1> smaller;
  typedef void thisisavec;
  typedef T value_type;
  static const std::size_t size = N;

  T operator[](std::size_t) constexpr { return value; }
};

template <std::size_t N> using IVec<N> = Vec<std::int32_t, N>;
template <std::size_t N> using DVec<N> = Vec<double, N>;

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

