typedef struct SliceDirs {

  float8 c;       /// camera
  float8 r, u, f; /// right, up, forward
  float fm;       /// distance forward (render distance)
  float rm, um;   /// r multiplier, u multiplier (fov)
} SliceDirs;

#define GETFLOOR_ENDIANESS_INDEX 0

/// do not use values out of the range of a short
int getFloor(float val) {
  val -= 0.5;
  val += 8388608 * 1.5;
  return ((short *)&val)[GETFLOOR_ENDIANESS_INDEX];
}

/// do not use values out of the range of a short
int fastRound(float val) {
  val += 8388608 * 1.5;
  return ((short *)&val)[GETFLOOR_ENDIANESS_INDEX];
}

float getMin5(float8 val) {
  float2 t = min(val.s01, val.s23);
  return min(min(t.lo, t.hi), val.s4);
}

/// using slow and general floor to prevent out-of-range problems
int8 getFloor8(float8 val) {
  return (int8)(floor(val.s0), floor(val.s1), floor(val.s2), floor(val.s3),
                floor(val.s4), floor(val.s5), floor(val.s6), floor(val.s7));
}

/// fast floor on only first five dims
int8 fastGetFloor5(float8 val) {
  return (int8)(getFloor(val.s0), getFloor(val.s1), getFloor(val.s2),
                getFloor(val.s3), getFloor(val.s4), 0, 0, 0);
}

int8 fastRound5(float8 val) {
  return (int8)(fastRound(val.s0), fastRound(val.s1), fastRound(val.s2),
                fastRound(val.s3), fastRound(val.s4), 0, 0, 0);
}

float8 int52float5(int8 val) {
  return (float8)(val.s0, val.s1, val.s2, val.s3, val.s4, 0, 0, 0);
}

/// designed to overshoot the floor on borderline values
int getHarshFloor(float val) {
  val -= 0.50001;
  val += 8388608 * 1.5;
  return ((short *)&val)[GETFLOOR_ENDIANESS_INDEX];
}

