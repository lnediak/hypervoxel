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

/// using slow and general floor to prevent out-of-range problems
int8 getFloor8(float8 val) {
  return int8(floor(val[0]), floor(val[1]), floor(val[2]), floor(val[3]),
              floor(val[4]), floor(val[5]), floor(val[6]), floor(val[7]));
}

/// designed to overshoot the floor on borderline values
int getHarshFloor(float val) {
  val -= 0.500001;
  val += 8388608 * 1.5;
  return ((short *)&val)[GETFLOOR_ENDIANESS_INDEX];
}

