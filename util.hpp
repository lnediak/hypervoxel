#include "vector.hpp"

namespace hypervoxel {

struct SliceDirs {

  v::FVec<5> c;       /// camera
  v::FVec<5> r, u, f; /// right, up, forward
  float fm;           /// forward multiplier (render distance)
  float rm, um;       /// r and u multipliers (at f dist. 1) (aspect ratio+fov)
};

#define GETFLOOR_ENDIANESS_INDEX 0

/// do not use values out of the range of a short
int fastFloor(float val) {
  val -= 0.5;
  val += 8388608 * 1.5;
  return ((short *)&val)[GETFLOOR_ENDIANESS_INDEX];
}

/// This is basically floor(val - small_number)
int harshFloor(float val) {
  val -= 0.50001;
  val += 8388608 * 1.5;
  return ((short *)&val)[GETFLOOR_ENDIANESS_INDEX];
}

/// do not use values out of the range of a short
int fastRound(float val) {
  val += 8388608 * 1.5;
  return ((short *)&val)[GETFLOOR_ENDIANESS_INDEX];
}

struct FloorU {
  static int apply(float a) { return ((int)a) - (a < 0); }
};
struct FastFloorU {
  static int apply(float a) { return fastFloor(a); }
};
struct FastRoundU {
  static int apply(float a) { return fastRound(a); }
};
struct Int2FloatU {
  static float apply(int a) { return a; }
};

template <class T> struct AbsU {
  static T apply(T a) { return a > 0 ? a : -a; }
};
template <class T> struct L1NormU {
  static T apply(T a, T b) { return a + AbsU<T>::apply(b); }
};

template <std::size_t N, class A>
using Floor = v::UnaryOp<float, int, N, FloorU, A>;
template <std::size_t N, class A>
using FastFloor = v::UnaryOp<float, int, N, FastFloorU, A>;
template <std::size_t N, class A>
using FastRound = v::UnaryOp<float, int, N, FastRoundU, A>;
template <std::size_t N, class A>
using Int2Float = v::UnaryOp<int, float, N, Int2FloatU, A>;

template <class T, std::size_t N, class A>
using L1Norm = v::Combine<T, N, L1NormU<T>, AbsU<T>, A>;

template <class A, class = typename rem_cvr<A>::thisisavvec,
          class = typename std::enable_if<
              std::is_same<typename A::value_type, float>::value>::type>
Floor<A::size, A> vfloor(const A &a) {
  return {a};
}
template <class A, class = typename rem_cvr<A>::thisisavvec,
          class = typename std::enable_if<
              std::is_same<typename A::value_type, float>::value>::type>
FastFloor<A::size, A> vfastFloor(const A &a) {
  return {a};
}
template <class A, class = typename rem_cvr<A>::thisisavvec,
          class = typename std::enable_if<
              std::is_same<typename A::value_type, float>::value>::type>
FastRound<A::size, A> vfastRound(const A &a) {
  return {a};
}
template <class A, class = typename rem_cvr<A>::thisisavvec,
          class = typename std::enable_if<
              std::is_same<typename A::value_type, float>::value>::type>
Int2Float<A::size, A> vint2float(const A &a) {
  return {a};
}

template <class A, class = typename rem_cvr<A>::thisisavvec>
typename A::value_type l1norm(const A &a) {
  return L1Norm<typename A::value_type, A::size, A>(a).evaluate();
}

} // namespace hypervoxel

