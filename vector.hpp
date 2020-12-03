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

  typedef BinaryOp<T, N - 1, U, A, B> smaller;
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

template <class T, template <class> class U, class A, class B>
struct BinaryOp<T, 0, U, A, B> {

  BinaryOp(const A &, const B &) {}

  void evaluateInto(T *) const {}
};

template <class T, std::size_t N, template <class> class U, class A>
struct ScalarOp {

  const A &a;
  T s;

  typedef ScalarOp<T, N - 1, U, A> smaller;
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

  typedef Combine<T, N - 1, U, A> smaller;

  Combine(const A &a) : a(a) {}

  const smaller gsmaller() const { return smaller(a); }

  T evaluate() const { return U<T>::apply(gsmaller().evaluate(), a[N - 1]); }
};

template <class T, template <class> class U, class A>
struct Combine<T, 1, U, A> {

  const A &a;

  Combine(const A &a) : a(a) {}

  T evaluate() const { return a[0]; }
};

template <class T> struct AddU {
  static T apply(T a, T b) { return a + b; }
};
template <class T, std::size_t N, class A, class B>
using Add = BinaryOp<T, N, AddU, A, B>;
template <class T, std::size_t N, class A> using SAdd = ScalarOp<T, N, AddU, A>;
template <class T, std::size_t N, class A> using Sum = Combine<T, N, AddU, A>;

template <class T> struct SubU {
  static T apply(T a, T b) { return a - b; }
};
template <class T, std::size_t N, class A, class B>
using Sub = BinaryOp<T, N, SubU, A, B>;
template <class T, std::size_t N, class A> using SSub = ScalarOp<T, N, SubU, A>;

template <class T> struct MultU {
  static T apply(T a, T b) { return a * b; }
};
template <class T, std::size_t N, class A, class B>
using Mult = BinaryOp<T, N, MultU, A, B>;
template <class T, std::size_t N, class A>
using SMult = ScalarOp<T, N, MultU, A>;
template <class T, std::size_t N, class A>
using Product = Combine<T, N, MultU, A>;

template <class T> struct DivU {
  static T apply(T a, T b) { return a / b; }
};
template <class T, std::size_t N, class A, class B>
using Div = BinaryOp<T, N, DivU, A, B>;
template <class T, std::size_t N, class A> using SDiv = ScalarOp<T, N, DivU, A>;

template <class T> struct MinU {
  static T apply(T a, T b) { return a > b ? b : a; }
};
template <class T, std::size_t N, class A, class B>
using ElementwiseMin = BinaryOp<T, N, MinU, A, B>;
template <class T, std::size_t N, class A> using Min = Combine<T, N, MinU, A>;

template <class T> struct MaxU {
  static T apply(T a, T b) { return a > b ? a : b; }
};
template <class T, std::size_t N, class A, class B>
using ElementwiseMax = BinaryOp<T, N, MaxU, A, B>;
template <class T, std::size_t N, class A> using Max = Combine<T, N, MaxU, A>;

} // namespace v

} // namespace hypervoxel

template <
    class A, class B, class = typename A::thisisavvec,
    class = typename B::thisisavvec,
    class = typename std::enable_if<
        std::is_same<typename A::value_type, typename B::value_type>::value &&
        A::size == B::size>::type>
hypervoxel::v::Add<typename A::value_type, A::size, A, B> operator+(const A &a, const B &b) {
  return {a, b};
}

template <
    class A, class B, class = typename A::thisisavvec,
    class = typename B::thisisavvec,
    class = typename std::enable_if<
        std::is_same<typename A::value_type, typename B::value_type>::value &&
        A::size == B::size>::type>
hypervoxel::v::Sub<typename A::value_type, A::size, A, B> operator-(const A &a, const B &b) {
  return {a, b};
}

template <
    class A, class B, class = typename A::thisisavvec,
    class = typename B::thisisavvec,
    class = typename std::enable_if<
        std::is_same<typename A::value_type, typename B::value_type>::value &&
        A::size == B::size>::type>
hypervoxel::v::Mult<typename A::value_type, A::size, A, B> operator*(const A &a, const B &b) {
  return {a, b};
}

template <
    class A, class B, class = typename A::thisisavvec,
    class = typename B::thisisavvec,
    class = typename std::enable_if<
        std::is_same<typename A::value_type, typename B::value_type>::value &&
        A::size == B::size>::type>
hypervoxel::v::Div<typename A::value_type, A::size, A, B> operator/(const A &a, const B &b) {
  return {a, b};
}

template <
    class A, class B, class = typename A::thisisavvec,
    class = typename B::thisisavvec,
    class = typename std::enable_if<
        std::is_same<typename A::value_type, typename B::value_type>::value &&
        A::size == B::size>::type>
bool operator==(const A &a, const B &b) {
  for (std::size_t i = A::size; i--;) {
    if (a[i] != b[i]) {
      return false;
    }
  }
  return true;
}

namespace hypervoxel {

namespace v {

template <
    class A, class B, class = typename A::thisisavvec,
    class = typename B::thisisavvec,
    class = typename std::enable_if<
        std::is_same<typename A::value_type, typename B::value_type>::value &&
        A::size == B::size>::type>
ElementwiseMin<typename A::value_type, A::size, A, B>
elementwiseMin(const A &a, const B &b) {
  return {a, b};
}

template <
    class A, class B, class = typename A::thisisavvec,
    class = typename B::thisisavvec,
    class = typename std::enable_if<
        std::is_same<typename A::value_type, typename B::value_type>::value &&
        A::size == B::size>::type>
ElementwiseMax<typename A::value_type, A::size, A, B>
elementwiseMax(const A &a, const B &b) {
  return {a, b};
}

template <class T, std::size_t N, class A> struct UnaryNeg {

  const A &a;

  typedef UnaryNeg<T, N - 1, A> smaller;
  typedef void thisisavvec;
  typedef T value_type;
  static const std::size_t size = N;

  UnaryNeg(const A &a) : a(a) {}

  const smaller gsmaller() { return smaller(a); }

  T operator[](std::size_t i) const { return -a[i]; }

  void evaluateInto(T *data) const {
    gsmaller().evaluateInto(data);
    data[N - 1] = operator[](N - 1);
  }
};

template <class T, class A> struct UnaryNeg<T, 0, A> {

  UnaryNeg(const A &) {}

  void evaluateInto(T *) {}
};

template <class T, std::size_t N, class A> struct ReverseScalarDiv {

  T s;
  const A &a;

  typedef ReverseScalarDiv<T, N - 1, A> smaller;
  typedef void thisisavvec;
  typedef T value_type;
  static const std::size_t size = N;

  ReverseScalarDiv(T s, const A &a) : s(s), a(a) {}

  const smaller gsmaller() const { return smaller(s, a); }

  T operator[](std::size_t i) const { return s / a[i]; }

  void evaluateInto(T *data) const {
    gsmaller().evaluateInto(data);
    data[N - 1] = operator[](N - 1);
  }
};

template <class T, class A>
struct ReverseScalarDiv<T, 0, A> {

  ReverseScalarDiv(T, const A &) {}

  void evaluateInto(T *) const {}
};

} // namespace v

} // namespace hypervoxel

template <class A, class = typename A::thisisavvec>
hypervoxel::v::UnaryNeg<typename A::value_type, A::size, A> operator-(const A &a) {
  return {a};
}

template <class A, class = typename A::thisisavvec>
hypervoxel::v::SAdd<typename A::value_type, A::size, A> operator+(const A &a,
                                                   typename A::value_type s) {
  return {a, s};
}

template <class A, class = typename A::thisisavvec>
hypervoxel::v::SAdd<typename A::value_type, A::size, A> operator+(typename A::value_type s,
                                                   const A &a) {
  return {a, s};
}

template <class A, class = typename A::thisisavvec>
hypervoxel::v::SSub<typename A::value_type, A::size, A> operator-(const A &a,
                                                   typename A::value_type s) {
  return {a, s};
}

template <class A, class = typename A::thisisavvec>
hypervoxel::v::SMult<typename A::value_type, A::size, A> operator*(const A &a,
                                                    typename A::value_type s) {
  return {a, s};
}

template <class A, class = typename A::thisisavvec>
hypervoxel::v::SMult<typename A::value_type, A::size, A> operator*(typename A::value_type s,
                                                    const A &a) {
  return {a, s};
}

template <class A, class = typename A::thisisavvec>
hypervoxel::v::SDiv<typename A::value_type, A::size, A> operator/(const A &a,
                                                   typename A::value_type s) {
  return {a, s};
}

template <class A, class = typename A::thisisavvec>
hypervoxel::v::ReverseScalarDiv<typename A::value_type, A::size, A>
operator/(typename A::value_type s, const A &a) {
  return {s, a};
}

namespace hypervoxel {

namespace v {

template <class A, class = typename A::thisisavvec>
typename A::value_type sum(const A &a) {
  return Sum<typename A::value_type, A::size, A>(a).evaluate();
}

template <class A, class = typename A::thisisavvec>
typename A::value_type min(const A &a) {
  return Min<typename A::value_type, A::size, A>(a).evaluate();
}

template <class A, class = typename A::thisisavvec>
typename A::value_type max(const A &a) {
  return Max<typename A::value_type, A::size, A>(a).evaluate();
}

template <
    class A, class B, class = typename A::thisisavvec,
    class = typename B::thisisavvec,
    class = typename std::enable_if<
        std::is_same<typename A::value_type, typename B::value_type>::value &&
        A::size == B::size>::type>
typename A::value_type dot(const A &a, const B &b) {
  return sum(a * b);
}

template <class A, class = typename A::thisisavvec>
typename A::value_type norm(const A &a) {
  return sum(a * a);
}

template <
    class A, class B, class = typename A::thisisavvec,
    class = typename B::thisisavvec,
    class = typename std::enable_if<
        std::is_same<typename A::value_type, typename B::value_type>::value &&
        A::size == B::size>::type>
typename A::value_type dist(const A &a, const B &b) {
  return norm(a - b);
}

template <class T, std::size_t N> class Vec {

  void assign(const T *other) {
    for (std::size_t i = N; i--;) {
      data[i] = other[i];
    }
  }

  template <
      class U, class = typename U::thisisavvec,
      class = typename std::enable_if<
          std::is_same<T, typename U::value_type>::value && N == U::size>::type>
  void assign(const U &other) {
    for (std::size_t i = N; i--;) {
      data[i] = other[i];
    }
  }

public:
  typedef Vec<T, N> smaller;
  typedef void thisisavvec;
  typedef T value_type;
  static const std::size_t size = N;

  T data[N];

  Vec(const std::initializer_list<T> &inil) {
    if (inil.size() != N) {
      return;
    }
    std::size_t i = 0;
    for (const T &val : inil) {
      data[i++] = val;
    }
  }

  Vec() {}

  template <
      class U, class = typename U::thisisavvec,
      class = typename std::enable_if<
          std::is_same<T, typename U::value_type>::value && N == U::size>::type>
  Vec(const U &other) {
    assign<U>(other);
  }

  template <
      class U, class = typename U::thisisavvec,
      class = typename std::enable_if<
          std::is_same<T, typename U::value_type>::value && N == U::size>::type>
  Vec<T, N> &operator=(const U &other) {
    assign<U>(other);
    return *this;
  }

  T &operator[](std::size_t i) { return data[i]; }
  const T &operator[](std::size_t i) const { return data[i]; }

  void copyFrom(const T *ptr) { assign(ptr); }

  template <
      class U, class = typename U::thisisavvec,
      class = typename std::enable_if<
          std::is_same<T, typename U::value_type>::value && N == U::size>::type>
  Vec<T, N> &operator+=(const U &other) {
    return operator=(*this + other);
  }
  template <
      class U, class = typename U::thisisavvec,
      class = typename std::enable_if<
          std::is_same<T, typename U::value_type>::value && N == U::size>::type>
  Vec<T, N> &operator-=(const U &other) {
    return operator=(*this - other);
  }
  template <
      class U, class = typename U::thisisavvec,
      class = typename std::enable_if<
          std::is_same<T, typename U::value_type>::value && N == U::size>::type>
  Vec<T, N> &operator*=(const U &other) {
    return operator=(*this *other);
  }
  template <
      class U, class = typename U::thisisavvec,
      class = typename std::enable_if<
          std::is_same<T, typename U::value_type>::value && N == U::size>::type>
  Vec<T, N> &operator/=(const U &other) {
    return operator=(*this / other);
  }

  Vec<T, N> &operator+=(T val) { return operator=(*this + val); }
  Vec<T, N> &operator-=(T val) { return operator=(*this - val); }
  Vec<T, N> &operator*=(T val) { return operator=(*this *val); }
  Vec<T, N> &operator/=(T val) { return operator=(*this / val); }
};

template <class T, T value, std::size_t N> struct ConstantVec {

  typedef ConstantVec<T, value, N - 1> smaller;
  typedef void thisisavvec;
  typedef T value_type;
  static const std::size_t size = N;

  constexpr T operator[](std::size_t) const { return value; }
};

template <std::size_t N> using IVec = Vec<std::int32_t, N>;
template <std::size_t N> using DVec = Vec<double, N>;

template <std::size_t N> struct IVecHash {

  std::size_t impl(const std::int32_t *argp) const noexcept {
    std::size_t total = IVecHash<N - 1>().impl(argp);
    return total * total * N * (N + 1) + 1;
  }

  std::size_t operator()(const IVec<N> &arg) const noexcept {
    // FIXME: WRITE ACTUAL PROVABLY GOOD HASH
    return impl(&arg[0]);
  }
};

template <> struct IVecHash<1> {

  std::size_t impl(const std::int32_t *argp) const noexcept {
    return argp[0] * argp[0] * 2 + 1;
  }

  std::size_t operator()(const IVec<1> &arg) const noexcept {
    return impl(&arg[0]);
  }
};

} // namespace v

} // namespace hypervoxel

#endif // HYPERVOXEL_VECTOR_HPP_

