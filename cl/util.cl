// This mirrors util.hpp, is all.
typedef struct SliceDirs {

  float8 c;       /// camera
  float8 r, u, f; /// right, up, forward
  float fm;       /// distance forward (render distance)
  float rm, um;   /// r multiplier, u multiplier (fov)
} SliceDirs;

#define GETFLOOR_ENDIANESS_INDEX 0

int fastFloor(float val) {
  val -= 0.5;
  val += 8388608 * 1.5;
  return ((short *)&val)[GETFLOOR_ENDIANESS_INDEX];
}

int harshFloor(float val) {
  val -= 0.50001;
  val += 8388608 * 1.5;
  return ((short *)&val)[GETFLOOR_ENDIANESS_INDEX];
}

int fastRound(float val) {
  val += 8388608 * 1.5;
  return ((short *)&val)[GETFLOOR_ENDIANESS_INDEX];
}

float getMin5(float8 val) {
  float2 t = min(val.s01, val.s23);
  return min(min(t.lo, t.hi), val.s4);
}

int8 getFloor8(float8 val) {
  return (int8)(floor(val.s0), floor(val.s1), floor(val.s2), floor(val.s3),
                floor(val.s4), floor(val.s5), floor(val.s6), floor(val.s7));
}

int8 fastFloor5(float8 val) {
  return (int8)(fastFloor(val.s0), fastFloor(val.s1), fastFloor(val.s2),
                fastFloor(val.s3), fastFloor(val.s4), 0, 0, 0);
}

int8 fastRound5(float8 val) {
  return (int8)(fastRound(val.s0), fastRound(val.s1), fastRound(val.s2),
                fastRound(val.s3), fastRound(val.s4), 0, 0, 0);
}

float8 int52float5(int8 val) {
  return (float8)(val.s0, val.s1, val.s2, val.s3, val.s4, 0, 0, 0);
}

