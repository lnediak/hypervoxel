// AUTO-GENERATED FILE: DO NOT MODIFY
R"OPENCLPP(
#ifndef TERRAIN_NON_REP_
#define TERRAIN_NON_REP_

int fastFloor(float x) {
  int i = x;
  return i - (x < i);
}

#endif // TERRAIN_NON_REP_

uint hash3( int pos0,  int pos1,  int pos2, 
                 uint octave, uint numGradVecs) {
  // TODO: WRITE THIS
  return (pos0 * pos0 + pos1 + pos2 * pos2 * pos2 + octave) % numGradVecs;
}

float getNoise3( float scale0, float pos0,  float scale1, float pos1,  float scale2, float pos2, 
                      __global float *gradVecs, uint numGradVecs,
                      int numOctaves, float persistence) {
  float total = 0;
  float amplitude = 1;
   pos0 = (pos0 + 0.5) / scale0;
   pos1 = (pos1 + 0.5) / scale1;
   pos2 = (pos2 + 0.5) / scale2;
  
  for (uint i = numOctaves; i--;) {
     int posf0 = fastFloor(pos0);
    float vec00 = pos0 - posf0;
    float vec10 = vec00 - 1;
    float lerp0 = vec00 * vec00 * (3 - 2 * vec00);
     int posf1 = fastFloor(pos1);
    float vec01 = pos1 - posf1;
    float vec11 = vec01 - 1;
    float lerp1 = vec01 * vec01 * (3 - 2 * vec01);
     int posf2 = fastFloor(pos2);
    float vec02 = pos2 - posf2;
    float vec12 = vec02 - 1;
    float lerp2 = vec02 * vec02 * (3 - 2 * vec02);
    
    __global float *tmp;
     tmp =
        gradVecs +
        3 * hash3( posf0 + 0,  posf1 + 0,  posf2 + 0, 
                              i, numGradVecs);
    float result000 =  vec00 * tmp[0] +  vec01 * tmp[1] +  vec02 * tmp[2] + 
    0;
     tmp =
        gradVecs +
        3 * hash3( posf0 + 0,  posf1 + 0,  posf2 + 1, 
                              i, numGradVecs);
    float result001 =  vec00 * tmp[0] +  vec01 * tmp[1] +  vec12 * tmp[2] + 
    0;
     tmp =
        gradVecs +
        3 * hash3( posf0 + 0,  posf1 + 1,  posf2 + 0, 
                              i, numGradVecs);
    float result010 =  vec00 * tmp[0] +  vec11 * tmp[1] +  vec02 * tmp[2] + 
    0;
     tmp =
        gradVecs +
        3 * hash3( posf0 + 0,  posf1 + 1,  posf2 + 1, 
                              i, numGradVecs);
    float result011 =  vec00 * tmp[0] +  vec11 * tmp[1] +  vec12 * tmp[2] + 
    0;
     tmp =
        gradVecs +
        3 * hash3( posf0 + 1,  posf1 + 0,  posf2 + 0, 
                              i, numGradVecs);
    float result100 =  vec10 * tmp[0] +  vec01 * tmp[1] +  vec02 * tmp[2] + 
    0;
     tmp =
        gradVecs +
        3 * hash3( posf0 + 1,  posf1 + 0,  posf2 + 1, 
                              i, numGradVecs);
    float result101 =  vec10 * tmp[0] +  vec01 * tmp[1] +  vec12 * tmp[2] + 
    0;
     tmp =
        gradVecs +
        3 * hash3( posf0 + 1,  posf1 + 1,  posf2 + 0, 
                              i, numGradVecs);
    float result110 =  vec10 * tmp[0] +  vec11 * tmp[1] +  vec02 * tmp[2] + 
    0;
     tmp =
        gradVecs +
        3 * hash3( posf0 + 1,  posf1 + 1,  posf2 + 1, 
                              i, numGradVecs);
    float result111 =  vec10 * tmp[0] +  vec11 * tmp[1] +  vec12 * tmp[2] + 
    0;
    
      float result00 =
        result000 + lerp0 * (result100 - result000);
     float result01 =
        result001 + lerp0 * (result101 - result001);
     float result10 =
        result010 + lerp0 * (result110 - result010);
     float result11 =
        result011 + lerp0 * (result111 - result011);
    
      float result0 =
        result00 + lerp1 * (result10 - result00);
     float result1 =
        result01 + lerp1 * (result11 - result01);
    
      float result =
        result0 + lerp2 * (result1 - result0);
    
    
    total += result * amplitude;
    amplitude *= persistence;
     pos0 *= 2;
     pos1 *= 2;
     pos2 *= 2;
    
  }
  return total;
}

#ifndef TERRAIN_NON_REP_
#define TERRAIN_NON_REP_

int fastFloor(float x) {
  int i = x;
  return i - (x < i);
}

#endif // TERRAIN_NON_REP_

uint hash4( int pos0,  int pos1,  int pos2,  int pos3, 
                 uint octave, uint numGradVecs) {
  // TODO: WRITE THIS
  return (pos0 * pos0 + pos1 + pos2 * pos2 * pos2 + octave) % numGradVecs;
}

float getNoise4( float scale0, float pos0,  float scale1, float pos1,  float scale2, float pos2,  float scale3, float pos3, 
                      __global float *gradVecs, uint numGradVecs,
                      int numOctaves, float persistence) {
  float total = 0;
  float amplitude = 1;
   pos0 = (pos0 + 0.5) / scale0;
   pos1 = (pos1 + 0.5) / scale1;
   pos2 = (pos2 + 0.5) / scale2;
   pos3 = (pos3 + 0.5) / scale3;
  
  for (uint i = numOctaves; i--;) {
     int posf0 = fastFloor(pos0);
    float vec00 = pos0 - posf0;
    float vec10 = vec00 - 1;
    float lerp0 = vec00 * vec00 * (3 - 2 * vec00);
     int posf1 = fastFloor(pos1);
    float vec01 = pos1 - posf1;
    float vec11 = vec01 - 1;
    float lerp1 = vec01 * vec01 * (3 - 2 * vec01);
     int posf2 = fastFloor(pos2);
    float vec02 = pos2 - posf2;
    float vec12 = vec02 - 1;
    float lerp2 = vec02 * vec02 * (3 - 2 * vec02);
     int posf3 = fastFloor(pos3);
    float vec03 = pos3 - posf3;
    float vec13 = vec03 - 1;
    float lerp3 = vec03 * vec03 * (3 - 2 * vec03);
    
    __global float *tmp;
     tmp =
        gradVecs +
        4 * hash4( posf0 + 0,  posf1 + 0,  posf2 + 0,  posf3 + 0, 
                              i, numGradVecs);
    float result0000 =  vec00 * tmp[0] +  vec01 * tmp[1] +  vec02 * tmp[2] +  vec03 * tmp[3] + 
    0;
     tmp =
        gradVecs +
        4 * hash4( posf0 + 0,  posf1 + 0,  posf2 + 0,  posf3 + 1, 
                              i, numGradVecs);
    float result0001 =  vec00 * tmp[0] +  vec01 * tmp[1] +  vec02 * tmp[2] +  vec13 * tmp[3] + 
    0;
     tmp =
        gradVecs +
        4 * hash4( posf0 + 0,  posf1 + 0,  posf2 + 1,  posf3 + 0, 
                              i, numGradVecs);
    float result0010 =  vec00 * tmp[0] +  vec01 * tmp[1] +  vec12 * tmp[2] +  vec03 * tmp[3] + 
    0;
     tmp =
        gradVecs +
        4 * hash4( posf0 + 0,  posf1 + 0,  posf2 + 1,  posf3 + 1, 
                              i, numGradVecs);
    float result0011 =  vec00 * tmp[0] +  vec01 * tmp[1] +  vec12 * tmp[2] +  vec13 * tmp[3] + 
    0;
     tmp =
        gradVecs +
        4 * hash4( posf0 + 0,  posf1 + 1,  posf2 + 0,  posf3 + 0, 
                              i, numGradVecs);
    float result0100 =  vec00 * tmp[0] +  vec11 * tmp[1] +  vec02 * tmp[2] +  vec03 * tmp[3] + 
    0;
     tmp =
        gradVecs +
        4 * hash4( posf0 + 0,  posf1 + 1,  posf2 + 0,  posf3 + 1, 
                              i, numGradVecs);
    float result0101 =  vec00 * tmp[0] +  vec11 * tmp[1] +  vec02 * tmp[2] +  vec13 * tmp[3] + 
    0;
     tmp =
        gradVecs +
        4 * hash4( posf0 + 0,  posf1 + 1,  posf2 + 1,  posf3 + 0, 
                              i, numGradVecs);
    float result0110 =  vec00 * tmp[0] +  vec11 * tmp[1] +  vec12 * tmp[2] +  vec03 * tmp[3] + 
    0;
     tmp =
        gradVecs +
        4 * hash4( posf0 + 0,  posf1 + 1,  posf2 + 1,  posf3 + 1, 
                              i, numGradVecs);
    float result0111 =  vec00 * tmp[0] +  vec11 * tmp[1] +  vec12 * tmp[2] +  vec13 * tmp[3] + 
    0;
     tmp =
        gradVecs +
        4 * hash4( posf0 + 1,  posf1 + 0,  posf2 + 0,  posf3 + 0, 
                              i, numGradVecs);
    float result1000 =  vec10 * tmp[0] +  vec01 * tmp[1] +  vec02 * tmp[2] +  vec03 * tmp[3] + 
    0;
     tmp =
        gradVecs +
        4 * hash4( posf0 + 1,  posf1 + 0,  posf2 + 0,  posf3 + 1, 
                              i, numGradVecs);
    float result1001 =  vec10 * tmp[0] +  vec01 * tmp[1] +  vec02 * tmp[2] +  vec13 * tmp[3] + 
    0;
     tmp =
        gradVecs +
        4 * hash4( posf0 + 1,  posf1 + 0,  posf2 + 1,  posf3 + 0, 
                              i, numGradVecs);
    float result1010 =  vec10 * tmp[0] +  vec01 * tmp[1] +  vec12 * tmp[2] +  vec03 * tmp[3] + 
    0;
     tmp =
        gradVecs +
        4 * hash4( posf0 + 1,  posf1 + 0,  posf2 + 1,  posf3 + 1, 
                              i, numGradVecs);
    float result1011 =  vec10 * tmp[0] +  vec01 * tmp[1] +  vec12 * tmp[2] +  vec13 * tmp[3] + 
    0;
     tmp =
        gradVecs +
        4 * hash4( posf0 + 1,  posf1 + 1,  posf2 + 0,  posf3 + 0, 
                              i, numGradVecs);
    float result1100 =  vec10 * tmp[0] +  vec11 * tmp[1] +  vec02 * tmp[2] +  vec03 * tmp[3] + 
    0;
     tmp =
        gradVecs +
        4 * hash4( posf0 + 1,  posf1 + 1,  posf2 + 0,  posf3 + 1, 
                              i, numGradVecs);
    float result1101 =  vec10 * tmp[0] +  vec11 * tmp[1] +  vec02 * tmp[2] +  vec13 * tmp[3] + 
    0;
     tmp =
        gradVecs +
        4 * hash4( posf0 + 1,  posf1 + 1,  posf2 + 1,  posf3 + 0, 
                              i, numGradVecs);
    float result1110 =  vec10 * tmp[0] +  vec11 * tmp[1] +  vec12 * tmp[2] +  vec03 * tmp[3] + 
    0;
     tmp =
        gradVecs +
        4 * hash4( posf0 + 1,  posf1 + 1,  posf2 + 1,  posf3 + 1, 
                              i, numGradVecs);
    float result1111 =  vec10 * tmp[0] +  vec11 * tmp[1] +  vec12 * tmp[2] +  vec13 * tmp[3] + 
    0;
    
      float result000 =
        result0000 + lerp0 * (result1000 - result0000);
     float result001 =
        result0001 + lerp0 * (result1001 - result0001);
     float result010 =
        result0010 + lerp0 * (result1010 - result0010);
     float result011 =
        result0011 + lerp0 * (result1011 - result0011);
     float result100 =
        result0100 + lerp0 * (result1100 - result0100);
     float result101 =
        result0101 + lerp0 * (result1101 - result0101);
     float result110 =
        result0110 + lerp0 * (result1110 - result0110);
     float result111 =
        result0111 + lerp0 * (result1111 - result0111);
    
      float result00 =
        result000 + lerp1 * (result100 - result000);
     float result01 =
        result001 + lerp1 * (result101 - result001);
     float result10 =
        result010 + lerp1 * (result110 - result010);
     float result11 =
        result011 + lerp1 * (result111 - result011);
    
      float result0 =
        result00 + lerp2 * (result10 - result00);
     float result1 =
        result01 + lerp2 * (result11 - result01);
    
      float result =
        result0 + lerp3 * (result1 - result0);
    
    
    total += result * amplitude;
    amplitude *= persistence;
     pos0 *= 2;
     pos1 *= 2;
     pos2 *= 2;
     pos3 *= 2;
    
  }
  return total;
}

#ifndef TERRAIN_NON_REP_
#define TERRAIN_NON_REP_

int fastFloor(float x) {
  int i = x;
  return i - (x < i);
}

#endif // TERRAIN_NON_REP_

uint hash5( int pos0,  int pos1,  int pos2,  int pos3,  int pos4, 
                 uint octave, uint numGradVecs) {
  // TODO: WRITE THIS
  return (pos0 * pos0 + pos1 + pos2 * pos2 * pos2 + octave) % numGradVecs;
}

float getNoise5( float scale0, float pos0,  float scale1, float pos1,  float scale2, float pos2,  float scale3, float pos3,  float scale4, float pos4, 
                      __global float *gradVecs, uint numGradVecs,
                      int numOctaves, float persistence) {
  float total = 0;
  float amplitude = 1;
   pos0 = (pos0 + 0.5) / scale0;
   pos1 = (pos1 + 0.5) / scale1;
   pos2 = (pos2 + 0.5) / scale2;
   pos3 = (pos3 + 0.5) / scale3;
   pos4 = (pos4 + 0.5) / scale4;
  
  for (uint i = numOctaves; i--;) {
     int posf0 = fastFloor(pos0);
    float vec00 = pos0 - posf0;
    float vec10 = vec00 - 1;
    float lerp0 = vec00 * vec00 * (3 - 2 * vec00);
     int posf1 = fastFloor(pos1);
    float vec01 = pos1 - posf1;
    float vec11 = vec01 - 1;
    float lerp1 = vec01 * vec01 * (3 - 2 * vec01);
     int posf2 = fastFloor(pos2);
    float vec02 = pos2 - posf2;
    float vec12 = vec02 - 1;
    float lerp2 = vec02 * vec02 * (3 - 2 * vec02);
     int posf3 = fastFloor(pos3);
    float vec03 = pos3 - posf3;
    float vec13 = vec03 - 1;
    float lerp3 = vec03 * vec03 * (3 - 2 * vec03);
     int posf4 = fastFloor(pos4);
    float vec04 = pos4 - posf4;
    float vec14 = vec04 - 1;
    float lerp4 = vec04 * vec04 * (3 - 2 * vec04);
    
    __global float *tmp;
     tmp =
        gradVecs +
        5 * hash5( posf0 + 0,  posf1 + 0,  posf2 + 0,  posf3 + 0,  posf4 + 0, 
                              i, numGradVecs);
    float result00000 =  vec00 * tmp[0] +  vec01 * tmp[1] +  vec02 * tmp[2] +  vec03 * tmp[3] +  vec04 * tmp[4] + 
    0;
     tmp =
        gradVecs +
        5 * hash5( posf0 + 0,  posf1 + 0,  posf2 + 0,  posf3 + 0,  posf4 + 1, 
                              i, numGradVecs);
    float result00001 =  vec00 * tmp[0] +  vec01 * tmp[1] +  vec02 * tmp[2] +  vec03 * tmp[3] +  vec14 * tmp[4] + 
    0;
     tmp =
        gradVecs +
        5 * hash5( posf0 + 0,  posf1 + 0,  posf2 + 0,  posf3 + 1,  posf4 + 0, 
                              i, numGradVecs);
    float result00010 =  vec00 * tmp[0] +  vec01 * tmp[1] +  vec02 * tmp[2] +  vec13 * tmp[3] +  vec04 * tmp[4] + 
    0;
     tmp =
        gradVecs +
        5 * hash5( posf0 + 0,  posf1 + 0,  posf2 + 0,  posf3 + 1,  posf4 + 1, 
                              i, numGradVecs);
    float result00011 =  vec00 * tmp[0] +  vec01 * tmp[1] +  vec02 * tmp[2] +  vec13 * tmp[3] +  vec14 * tmp[4] + 
    0;
     tmp =
        gradVecs +
        5 * hash5( posf0 + 0,  posf1 + 0,  posf2 + 1,  posf3 + 0,  posf4 + 0, 
                              i, numGradVecs);
    float result00100 =  vec00 * tmp[0] +  vec01 * tmp[1] +  vec12 * tmp[2] +  vec03 * tmp[3] +  vec04 * tmp[4] + 
    0;
     tmp =
        gradVecs +
        5 * hash5( posf0 + 0,  posf1 + 0,  posf2 + 1,  posf3 + 0,  posf4 + 1, 
                              i, numGradVecs);
    float result00101 =  vec00 * tmp[0] +  vec01 * tmp[1] +  vec12 * tmp[2] +  vec03 * tmp[3] +  vec14 * tmp[4] + 
    0;
     tmp =
        gradVecs +
        5 * hash5( posf0 + 0,  posf1 + 0,  posf2 + 1,  posf3 + 1,  posf4 + 0, 
                              i, numGradVecs);
    float result00110 =  vec00 * tmp[0] +  vec01 * tmp[1] +  vec12 * tmp[2] +  vec13 * tmp[3] +  vec04 * tmp[4] + 
    0;
     tmp =
        gradVecs +
        5 * hash5( posf0 + 0,  posf1 + 0,  posf2 + 1,  posf3 + 1,  posf4 + 1, 
                              i, numGradVecs);
    float result00111 =  vec00 * tmp[0] +  vec01 * tmp[1] +  vec12 * tmp[2] +  vec13 * tmp[3] +  vec14 * tmp[4] + 
    0;
     tmp =
        gradVecs +
        5 * hash5( posf0 + 0,  posf1 + 1,  posf2 + 0,  posf3 + 0,  posf4 + 0, 
                              i, numGradVecs);
    float result01000 =  vec00 * tmp[0] +  vec11 * tmp[1] +  vec02 * tmp[2] +  vec03 * tmp[3] +  vec04 * tmp[4] + 
    0;
     tmp =
        gradVecs +
        5 * hash5( posf0 + 0,  posf1 + 1,  posf2 + 0,  posf3 + 0,  posf4 + 1, 
                              i, numGradVecs);
    float result01001 =  vec00 * tmp[0] +  vec11 * tmp[1] +  vec02 * tmp[2] +  vec03 * tmp[3] +  vec14 * tmp[4] + 
    0;
     tmp =
        gradVecs +
        5 * hash5( posf0 + 0,  posf1 + 1,  posf2 + 0,  posf3 + 1,  posf4 + 0, 
                              i, numGradVecs);
    float result01010 =  vec00 * tmp[0] +  vec11 * tmp[1] +  vec02 * tmp[2] +  vec13 * tmp[3] +  vec04 * tmp[4] + 
    0;
     tmp =
        gradVecs +
        5 * hash5( posf0 + 0,  posf1 + 1,  posf2 + 0,  posf3 + 1,  posf4 + 1, 
                              i, numGradVecs);
    float result01011 =  vec00 * tmp[0] +  vec11 * tmp[1] +  vec02 * tmp[2] +  vec13 * tmp[3] +  vec14 * tmp[4] + 
    0;
     tmp =
        gradVecs +
        5 * hash5( posf0 + 0,  posf1 + 1,  posf2 + 1,  posf3 + 0,  posf4 + 0, 
                              i, numGradVecs);
    float result01100 =  vec00 * tmp[0] +  vec11 * tmp[1] +  vec12 * tmp[2] +  vec03 * tmp[3] +  vec04 * tmp[4] + 
    0;
     tmp =
        gradVecs +
        5 * hash5( posf0 + 0,  posf1 + 1,  posf2 + 1,  posf3 + 0,  posf4 + 1, 
                              i, numGradVecs);
    float result01101 =  vec00 * tmp[0] +  vec11 * tmp[1] +  vec12 * tmp[2] +  vec03 * tmp[3] +  vec14 * tmp[4] + 
    0;
     tmp =
        gradVecs +
        5 * hash5( posf0 + 0,  posf1 + 1,  posf2 + 1,  posf3 + 1,  posf4 + 0, 
                              i, numGradVecs);
    float result01110 =  vec00 * tmp[0] +  vec11 * tmp[1] +  vec12 * tmp[2] +  vec13 * tmp[3] +  vec04 * tmp[4] + 
    0;
     tmp =
        gradVecs +
        5 * hash5( posf0 + 0,  posf1 + 1,  posf2 + 1,  posf3 + 1,  posf4 + 1, 
                              i, numGradVecs);
    float result01111 =  vec00 * tmp[0] +  vec11 * tmp[1] +  vec12 * tmp[2] +  vec13 * tmp[3] +  vec14 * tmp[4] + 
    0;
     tmp =
        gradVecs +
        5 * hash5( posf0 + 1,  posf1 + 0,  posf2 + 0,  posf3 + 0,  posf4 + 0, 
                              i, numGradVecs);
    float result10000 =  vec10 * tmp[0] +  vec01 * tmp[1] +  vec02 * tmp[2] +  vec03 * tmp[3] +  vec04 * tmp[4] + 
    0;
     tmp =
        gradVecs +
        5 * hash5( posf0 + 1,  posf1 + 0,  posf2 + 0,  posf3 + 0,  posf4 + 1, 
                              i, numGradVecs);
    float result10001 =  vec10 * tmp[0] +  vec01 * tmp[1] +  vec02 * tmp[2] +  vec03 * tmp[3] +  vec14 * tmp[4] + 
    0;
     tmp =
        gradVecs +
        5 * hash5( posf0 + 1,  posf1 + 0,  posf2 + 0,  posf3 + 1,  posf4 + 0, 
                              i, numGradVecs);
    float result10010 =  vec10 * tmp[0] +  vec01 * tmp[1] +  vec02 * tmp[2] +  vec13 * tmp[3] +  vec04 * tmp[4] + 
    0;
     tmp =
        gradVecs +
        5 * hash5( posf0 + 1,  posf1 + 0,  posf2 + 0,  posf3 + 1,  posf4 + 1, 
                              i, numGradVecs);
    float result10011 =  vec10 * tmp[0] +  vec01 * tmp[1] +  vec02 * tmp[2] +  vec13 * tmp[3] +  vec14 * tmp[4] + 
    0;
     tmp =
        gradVecs +
        5 * hash5( posf0 + 1,  posf1 + 0,  posf2 + 1,  posf3 + 0,  posf4 + 0, 
                              i, numGradVecs);
    float result10100 =  vec10 * tmp[0] +  vec01 * tmp[1] +  vec12 * tmp[2] +  vec03 * tmp[3] +  vec04 * tmp[4] + 
    0;
     tmp =
        gradVecs +
        5 * hash5( posf0 + 1,  posf1 + 0,  posf2 + 1,  posf3 + 0,  posf4 + 1, 
                              i, numGradVecs);
    float result10101 =  vec10 * tmp[0] +  vec01 * tmp[1] +  vec12 * tmp[2] +  vec03 * tmp[3] +  vec14 * tmp[4] + 
    0;
     tmp =
        gradVecs +
        5 * hash5( posf0 + 1,  posf1 + 0,  posf2 + 1,  posf3 + 1,  posf4 + 0, 
                              i, numGradVecs);
    float result10110 =  vec10 * tmp[0] +  vec01 * tmp[1] +  vec12 * tmp[2] +  vec13 * tmp[3] +  vec04 * tmp[4] + 
    0;
     tmp =
        gradVecs +
        5 * hash5( posf0 + 1,  posf1 + 0,  posf2 + 1,  posf3 + 1,  posf4 + 1, 
                              i, numGradVecs);
    float result10111 =  vec10 * tmp[0] +  vec01 * tmp[1] +  vec12 * tmp[2] +  vec13 * tmp[3] +  vec14 * tmp[4] + 
    0;
     tmp =
        gradVecs +
        5 * hash5( posf0 + 1,  posf1 + 1,  posf2 + 0,  posf3 + 0,  posf4 + 0, 
                              i, numGradVecs);
    float result11000 =  vec10 * tmp[0] +  vec11 * tmp[1] +  vec02 * tmp[2] +  vec03 * tmp[3] +  vec04 * tmp[4] + 
    0;
     tmp =
        gradVecs +
        5 * hash5( posf0 + 1,  posf1 + 1,  posf2 + 0,  posf3 + 0,  posf4 + 1, 
                              i, numGradVecs);
    float result11001 =  vec10 * tmp[0] +  vec11 * tmp[1] +  vec02 * tmp[2] +  vec03 * tmp[3] +  vec14 * tmp[4] + 
    0;
     tmp =
        gradVecs +
        5 * hash5( posf0 + 1,  posf1 + 1,  posf2 + 0,  posf3 + 1,  posf4 + 0, 
                              i, numGradVecs);
    float result11010 =  vec10 * tmp[0] +  vec11 * tmp[1] +  vec02 * tmp[2] +  vec13 * tmp[3] +  vec04 * tmp[4] + 
    0;
     tmp =
        gradVecs +
        5 * hash5( posf0 + 1,  posf1 + 1,  posf2 + 0,  posf3 + 1,  posf4 + 1, 
                              i, numGradVecs);
    float result11011 =  vec10 * tmp[0] +  vec11 * tmp[1] +  vec02 * tmp[2] +  vec13 * tmp[3] +  vec14 * tmp[4] + 
    0;
     tmp =
        gradVecs +
        5 * hash5( posf0 + 1,  posf1 + 1,  posf2 + 1,  posf3 + 0,  posf4 + 0, 
                              i, numGradVecs);
    float result11100 =  vec10 * tmp[0] +  vec11 * tmp[1] +  vec12 * tmp[2] +  vec03 * tmp[3] +  vec04 * tmp[4] + 
    0;
     tmp =
        gradVecs +
        5 * hash5( posf0 + 1,  posf1 + 1,  posf2 + 1,  posf3 + 0,  posf4 + 1, 
                              i, numGradVecs);
    float result11101 =  vec10 * tmp[0] +  vec11 * tmp[1] +  vec12 * tmp[2] +  vec03 * tmp[3] +  vec14 * tmp[4] + 
    0;
     tmp =
        gradVecs +
        5 * hash5( posf0 + 1,  posf1 + 1,  posf2 + 1,  posf3 + 1,  posf4 + 0, 
                              i, numGradVecs);
    float result11110 =  vec10 * tmp[0] +  vec11 * tmp[1] +  vec12 * tmp[2] +  vec13 * tmp[3] +  vec04 * tmp[4] + 
    0;
     tmp =
        gradVecs +
        5 * hash5( posf0 + 1,  posf1 + 1,  posf2 + 1,  posf3 + 1,  posf4 + 1, 
                              i, numGradVecs);
    float result11111 =  vec10 * tmp[0] +  vec11 * tmp[1] +  vec12 * tmp[2] +  vec13 * tmp[3] +  vec14 * tmp[4] + 
    0;
    
      float result0000 =
        result00000 + lerp0 * (result10000 - result00000);
     float result0001 =
        result00001 + lerp0 * (result10001 - result00001);
     float result0010 =
        result00010 + lerp0 * (result10010 - result00010);
     float result0011 =
        result00011 + lerp0 * (result10011 - result00011);
     float result0100 =
        result00100 + lerp0 * (result10100 - result00100);
     float result0101 =
        result00101 + lerp0 * (result10101 - result00101);
     float result0110 =
        result00110 + lerp0 * (result10110 - result00110);
     float result0111 =
        result00111 + lerp0 * (result10111 - result00111);
     float result1000 =
        result01000 + lerp0 * (result11000 - result01000);
     float result1001 =
        result01001 + lerp0 * (result11001 - result01001);
     float result1010 =
        result01010 + lerp0 * (result11010 - result01010);
     float result1011 =
        result01011 + lerp0 * (result11011 - result01011);
     float result1100 =
        result01100 + lerp0 * (result11100 - result01100);
     float result1101 =
        result01101 + lerp0 * (result11101 - result01101);
     float result1110 =
        result01110 + lerp0 * (result11110 - result01110);
     float result1111 =
        result01111 + lerp0 * (result11111 - result01111);
    
      float result000 =
        result0000 + lerp1 * (result1000 - result0000);
     float result001 =
        result0001 + lerp1 * (result1001 - result0001);
     float result010 =
        result0010 + lerp1 * (result1010 - result0010);
     float result011 =
        result0011 + lerp1 * (result1011 - result0011);
     float result100 =
        result0100 + lerp1 * (result1100 - result0100);
     float result101 =
        result0101 + lerp1 * (result1101 - result0101);
     float result110 =
        result0110 + lerp1 * (result1110 - result0110);
     float result111 =
        result0111 + lerp1 * (result1111 - result0111);
    
      float result00 =
        result000 + lerp2 * (result100 - result000);
     float result01 =
        result001 + lerp2 * (result101 - result001);
     float result10 =
        result010 + lerp2 * (result110 - result010);
     float result11 =
        result011 + lerp2 * (result111 - result011);
    
      float result0 =
        result00 + lerp3 * (result10 - result00);
     float result1 =
        result01 + lerp3 * (result11 - result01);
    
      float result =
        result0 + lerp4 * (result1 - result0);
    
    
    total += result * amplitude;
    amplitude *= persistence;
     pos0 *= 2;
     pos1 *= 2;
     pos2 *= 2;
     pos3 *= 2;
     pos4 *= 2;
    
  }
  return total;
}

#ifndef TERRAIN_NON_REP_
#define TERRAIN_NON_REP_

int fastFloor(float x) {
  int i = x;
  return i - (x < i);
}

#endif // TERRAIN_NON_REP_

uint hash6( int pos0,  int pos1,  int pos2,  int pos3,  int pos4,  int pos5, 
                 uint octave, uint numGradVecs) {
  // TODO: WRITE THIS
  return (pos0 * pos0 + pos1 + pos2 * pos2 * pos2 + octave) % numGradVecs;
}

float getNoise6( float scale0, float pos0,  float scale1, float pos1,  float scale2, float pos2,  float scale3, float pos3,  float scale4, float pos4,  float scale5, float pos5, 
                      __global float *gradVecs, uint numGradVecs,
                      int numOctaves, float persistence) {
  float total = 0;
  float amplitude = 1;
   pos0 = (pos0 + 0.5) / scale0;
   pos1 = (pos1 + 0.5) / scale1;
   pos2 = (pos2 + 0.5) / scale2;
   pos3 = (pos3 + 0.5) / scale3;
   pos4 = (pos4 + 0.5) / scale4;
   pos5 = (pos5 + 0.5) / scale5;
  
  for (uint i = numOctaves; i--;) {
     int posf0 = fastFloor(pos0);
    float vec00 = pos0 - posf0;
    float vec10 = vec00 - 1;
    float lerp0 = vec00 * vec00 * (3 - 2 * vec00);
     int posf1 = fastFloor(pos1);
    float vec01 = pos1 - posf1;
    float vec11 = vec01 - 1;
    float lerp1 = vec01 * vec01 * (3 - 2 * vec01);
     int posf2 = fastFloor(pos2);
    float vec02 = pos2 - posf2;
    float vec12 = vec02 - 1;
    float lerp2 = vec02 * vec02 * (3 - 2 * vec02);
     int posf3 = fastFloor(pos3);
    float vec03 = pos3 - posf3;
    float vec13 = vec03 - 1;
    float lerp3 = vec03 * vec03 * (3 - 2 * vec03);
     int posf4 = fastFloor(pos4);
    float vec04 = pos4 - posf4;
    float vec14 = vec04 - 1;
    float lerp4 = vec04 * vec04 * (3 - 2 * vec04);
     int posf5 = fastFloor(pos5);
    float vec05 = pos5 - posf5;
    float vec15 = vec05 - 1;
    float lerp5 = vec05 * vec05 * (3 - 2 * vec05);
    
    __global float *tmp;
     tmp =
        gradVecs +
        6 * hash6( posf0 + 0,  posf1 + 0,  posf2 + 0,  posf3 + 0,  posf4 + 0,  posf5 + 0, 
                              i, numGradVecs);
    float result000000 =  vec00 * tmp[0] +  vec01 * tmp[1] +  vec02 * tmp[2] +  vec03 * tmp[3] +  vec04 * tmp[4] +  vec05 * tmp[5] + 
    0;
     tmp =
        gradVecs +
        6 * hash6( posf0 + 0,  posf1 + 0,  posf2 + 0,  posf3 + 0,  posf4 + 0,  posf5 + 1, 
                              i, numGradVecs);
    float result000001 =  vec00 * tmp[0] +  vec01 * tmp[1] +  vec02 * tmp[2] +  vec03 * tmp[3] +  vec04 * tmp[4] +  vec15 * tmp[5] + 
    0;
     tmp =
        gradVecs +
        6 * hash6( posf0 + 0,  posf1 + 0,  posf2 + 0,  posf3 + 0,  posf4 + 1,  posf5 + 0, 
                              i, numGradVecs);
    float result000010 =  vec00 * tmp[0] +  vec01 * tmp[1] +  vec02 * tmp[2] +  vec03 * tmp[3] +  vec14 * tmp[4] +  vec05 * tmp[5] + 
    0;
     tmp =
        gradVecs +
        6 * hash6( posf0 + 0,  posf1 + 0,  posf2 + 0,  posf3 + 0,  posf4 + 1,  posf5 + 1, 
                              i, numGradVecs);
    float result000011 =  vec00 * tmp[0] +  vec01 * tmp[1] +  vec02 * tmp[2] +  vec03 * tmp[3] +  vec14 * tmp[4] +  vec15 * tmp[5] + 
    0;
     tmp =
        gradVecs +
        6 * hash6( posf0 + 0,  posf1 + 0,  posf2 + 0,  posf3 + 1,  posf4 + 0,  posf5 + 0, 
                              i, numGradVecs);
    float result000100 =  vec00 * tmp[0] +  vec01 * tmp[1] +  vec02 * tmp[2] +  vec13 * tmp[3] +  vec04 * tmp[4] +  vec05 * tmp[5] + 
    0;
     tmp =
        gradVecs +
        6 * hash6( posf0 + 0,  posf1 + 0,  posf2 + 0,  posf3 + 1,  posf4 + 0,  posf5 + 1, 
                              i, numGradVecs);
    float result000101 =  vec00 * tmp[0] +  vec01 * tmp[1] +  vec02 * tmp[2] +  vec13 * tmp[3] +  vec04 * tmp[4] +  vec15 * tmp[5] + 
    0;
     tmp =
        gradVecs +
        6 * hash6( posf0 + 0,  posf1 + 0,  posf2 + 0,  posf3 + 1,  posf4 + 1,  posf5 + 0, 
                              i, numGradVecs);
    float result000110 =  vec00 * tmp[0] +  vec01 * tmp[1] +  vec02 * tmp[2] +  vec13 * tmp[3] +  vec14 * tmp[4] +  vec05 * tmp[5] + 
    0;
     tmp =
        gradVecs +
        6 * hash6( posf0 + 0,  posf1 + 0,  posf2 + 0,  posf3 + 1,  posf4 + 1,  posf5 + 1, 
                              i, numGradVecs);
    float result000111 =  vec00 * tmp[0] +  vec01 * tmp[1] +  vec02 * tmp[2] +  vec13 * tmp[3] +  vec14 * tmp[4] +  vec15 * tmp[5] + 
    0;
     tmp =
        gradVecs +
        6 * hash6( posf0 + 0,  posf1 + 0,  posf2 + 1,  posf3 + 0,  posf4 + 0,  posf5 + 0, 
                              i, numGradVecs);
    float result001000 =  vec00 * tmp[0] +  vec01 * tmp[1] +  vec12 * tmp[2] +  vec03 * tmp[3] +  vec04 * tmp[4] +  vec05 * tmp[5] + 
    0;
     tmp =
        gradVecs +
        6 * hash6( posf0 + 0,  posf1 + 0,  posf2 + 1,  posf3 + 0,  posf4 + 0,  posf5 + 1, 
                              i, numGradVecs);
    float result001001 =  vec00 * tmp[0] +  vec01 * tmp[1] +  vec12 * tmp[2] +  vec03 * tmp[3] +  vec04 * tmp[4] +  vec15 * tmp[5] + 
    0;
     tmp =
        gradVecs +
        6 * hash6( posf0 + 0,  posf1 + 0,  posf2 + 1,  posf3 + 0,  posf4 + 1,  posf5 + 0, 
                              i, numGradVecs);
    float result001010 =  vec00 * tmp[0] +  vec01 * tmp[1] +  vec12 * tmp[2] +  vec03 * tmp[3] +  vec14 * tmp[4] +  vec05 * tmp[5] + 
    0;
     tmp =
        gradVecs +
        6 * hash6( posf0 + 0,  posf1 + 0,  posf2 + 1,  posf3 + 0,  posf4 + 1,  posf5 + 1, 
                              i, numGradVecs);
    float result001011 =  vec00 * tmp[0] +  vec01 * tmp[1] +  vec12 * tmp[2] +  vec03 * tmp[3] +  vec14 * tmp[4] +  vec15 * tmp[5] + 
    0;
     tmp =
        gradVecs +
        6 * hash6( posf0 + 0,  posf1 + 0,  posf2 + 1,  posf3 + 1,  posf4 + 0,  posf5 + 0, 
                              i, numGradVecs);
    float result001100 =  vec00 * tmp[0] +  vec01 * tmp[1] +  vec12 * tmp[2] +  vec13 * tmp[3] +  vec04 * tmp[4] +  vec05 * tmp[5] + 
    0;
     tmp =
        gradVecs +
        6 * hash6( posf0 + 0,  posf1 + 0,  posf2 + 1,  posf3 + 1,  posf4 + 0,  posf5 + 1, 
                              i, numGradVecs);
    float result001101 =  vec00 * tmp[0] +  vec01 * tmp[1] +  vec12 * tmp[2] +  vec13 * tmp[3] +  vec04 * tmp[4] +  vec15 * tmp[5] + 
    0;
     tmp =
        gradVecs +
        6 * hash6( posf0 + 0,  posf1 + 0,  posf2 + 1,  posf3 + 1,  posf4 + 1,  posf5 + 0, 
                              i, numGradVecs);
    float result001110 =  vec00 * tmp[0] +  vec01 * tmp[1] +  vec12 * tmp[2] +  vec13 * tmp[3] +  vec14 * tmp[4] +  vec05 * tmp[5] + 
    0;
     tmp =
        gradVecs +
        6 * hash6( posf0 + 0,  posf1 + 0,  posf2 + 1,  posf3 + 1,  posf4 + 1,  posf5 + 1, 
                              i, numGradVecs);
    float result001111 =  vec00 * tmp[0] +  vec01 * tmp[1] +  vec12 * tmp[2] +  vec13 * tmp[3] +  vec14 * tmp[4] +  vec15 * tmp[5] + 
    0;
     tmp =
        gradVecs +
        6 * hash6( posf0 + 0,  posf1 + 1,  posf2 + 0,  posf3 + 0,  posf4 + 0,  posf5 + 0, 
                              i, numGradVecs);
    float result010000 =  vec00 * tmp[0] +  vec11 * tmp[1] +  vec02 * tmp[2] +  vec03 * tmp[3] +  vec04 * tmp[4] +  vec05 * tmp[5] + 
    0;
     tmp =
        gradVecs +
        6 * hash6( posf0 + 0,  posf1 + 1,  posf2 + 0,  posf3 + 0,  posf4 + 0,  posf5 + 1, 
                              i, numGradVecs);
    float result010001 =  vec00 * tmp[0] +  vec11 * tmp[1] +  vec02 * tmp[2] +  vec03 * tmp[3] +  vec04 * tmp[4] +  vec15 * tmp[5] + 
    0;
     tmp =
        gradVecs +
        6 * hash6( posf0 + 0,  posf1 + 1,  posf2 + 0,  posf3 + 0,  posf4 + 1,  posf5 + 0, 
                              i, numGradVecs);
    float result010010 =  vec00 * tmp[0] +  vec11 * tmp[1] +  vec02 * tmp[2] +  vec03 * tmp[3] +  vec14 * tmp[4] +  vec05 * tmp[5] + 
    0;
     tmp =
        gradVecs +
        6 * hash6( posf0 + 0,  posf1 + 1,  posf2 + 0,  posf3 + 0,  posf4 + 1,  posf5 + 1, 
                              i, numGradVecs);
    float result010011 =  vec00 * tmp[0] +  vec11 * tmp[1] +  vec02 * tmp[2] +  vec03 * tmp[3] +  vec14 * tmp[4] +  vec15 * tmp[5] + 
    0;
     tmp =
        gradVecs +
        6 * hash6( posf0 + 0,  posf1 + 1,  posf2 + 0,  posf3 + 1,  posf4 + 0,  posf5 + 0, 
                              i, numGradVecs);
    float result010100 =  vec00 * tmp[0] +  vec11 * tmp[1] +  vec02 * tmp[2] +  vec13 * tmp[3] +  vec04 * tmp[4] +  vec05 * tmp[5] + 
    0;
     tmp =
        gradVecs +
        6 * hash6( posf0 + 0,  posf1 + 1,  posf2 + 0,  posf3 + 1,  posf4 + 0,  posf5 + 1, 
                              i, numGradVecs);
    float result010101 =  vec00 * tmp[0] +  vec11 * tmp[1] +  vec02 * tmp[2] +  vec13 * tmp[3] +  vec04 * tmp[4] +  vec15 * tmp[5] + 
    0;
     tmp =
        gradVecs +
        6 * hash6( posf0 + 0,  posf1 + 1,  posf2 + 0,  posf3 + 1,  posf4 + 1,  posf5 + 0, 
                              i, numGradVecs);
    float result010110 =  vec00 * tmp[0] +  vec11 * tmp[1] +  vec02 * tmp[2] +  vec13 * tmp[3] +  vec14 * tmp[4] +  vec05 * tmp[5] + 
    0;
     tmp =
        gradVecs +
        6 * hash6( posf0 + 0,  posf1 + 1,  posf2 + 0,  posf3 + 1,  posf4 + 1,  posf5 + 1, 
                              i, numGradVecs);
    float result010111 =  vec00 * tmp[0] +  vec11 * tmp[1] +  vec02 * tmp[2] +  vec13 * tmp[3] +  vec14 * tmp[4] +  vec15 * tmp[5] + 
    0;
     tmp =
        gradVecs +
        6 * hash6( posf0 + 0,  posf1 + 1,  posf2 + 1,  posf3 + 0,  posf4 + 0,  posf5 + 0, 
                              i, numGradVecs);
    float result011000 =  vec00 * tmp[0] +  vec11 * tmp[1] +  vec12 * tmp[2] +  vec03 * tmp[3] +  vec04 * tmp[4] +  vec05 * tmp[5] + 
    0;
     tmp =
        gradVecs +
        6 * hash6( posf0 + 0,  posf1 + 1,  posf2 + 1,  posf3 + 0,  posf4 + 0,  posf5 + 1, 
                              i, numGradVecs);
    float result011001 =  vec00 * tmp[0] +  vec11 * tmp[1] +  vec12 * tmp[2] +  vec03 * tmp[3] +  vec04 * tmp[4] +  vec15 * tmp[5] + 
    0;
     tmp =
        gradVecs +
        6 * hash6( posf0 + 0,  posf1 + 1,  posf2 + 1,  posf3 + 0,  posf4 + 1,  posf5 + 0, 
                              i, numGradVecs);
    float result011010 =  vec00 * tmp[0] +  vec11 * tmp[1] +  vec12 * tmp[2] +  vec03 * tmp[3] +  vec14 * tmp[4] +  vec05 * tmp[5] + 
    0;
     tmp =
        gradVecs +
        6 * hash6( posf0 + 0,  posf1 + 1,  posf2 + 1,  posf3 + 0,  posf4 + 1,  posf5 + 1, 
                              i, numGradVecs);
    float result011011 =  vec00 * tmp[0] +  vec11 * tmp[1] +  vec12 * tmp[2] +  vec03 * tmp[3] +  vec14 * tmp[4] +  vec15 * tmp[5] + 
    0;
     tmp =
        gradVecs +
        6 * hash6( posf0 + 0,  posf1 + 1,  posf2 + 1,  posf3 + 1,  posf4 + 0,  posf5 + 0, 
                              i, numGradVecs);
    float result011100 =  vec00 * tmp[0] +  vec11 * tmp[1] +  vec12 * tmp[2] +  vec13 * tmp[3] +  vec04 * tmp[4] +  vec05 * tmp[5] + 
    0;
     tmp =
        gradVecs +
        6 * hash6( posf0 + 0,  posf1 + 1,  posf2 + 1,  posf3 + 1,  posf4 + 0,  posf5 + 1, 
                              i, numGradVecs);
    float result011101 =  vec00 * tmp[0] +  vec11 * tmp[1] +  vec12 * tmp[2] +  vec13 * tmp[3] +  vec04 * tmp[4] +  vec15 * tmp[5] + 
    0;
     tmp =
        gradVecs +
        6 * hash6( posf0 + 0,  posf1 + 1,  posf2 + 1,  posf3 + 1,  posf4 + 1,  posf5 + 0, 
                              i, numGradVecs);
    float result011110 =  vec00 * tmp[0] +  vec11 * tmp[1] +  vec12 * tmp[2] +  vec13 * tmp[3] +  vec14 * tmp[4] +  vec05 * tmp[5] + 
    0;
     tmp =
        gradVecs +
        6 * hash6( posf0 + 0,  posf1 + 1,  posf2 + 1,  posf3 + 1,  posf4 + 1,  posf5 + 1, 
                              i, numGradVecs);
    float result011111 =  vec00 * tmp[0] +  vec11 * tmp[1] +  vec12 * tmp[2] +  vec13 * tmp[3] +  vec14 * tmp[4] +  vec15 * tmp[5] + 
    0;
     tmp =
        gradVecs +
        6 * hash6( posf0 + 1,  posf1 + 0,  posf2 + 0,  posf3 + 0,  posf4 + 0,  posf5 + 0, 
                              i, numGradVecs);
    float result100000 =  vec10 * tmp[0] +  vec01 * tmp[1] +  vec02 * tmp[2] +  vec03 * tmp[3] +  vec04 * tmp[4] +  vec05 * tmp[5] + 
    0;
     tmp =
        gradVecs +
        6 * hash6( posf0 + 1,  posf1 + 0,  posf2 + 0,  posf3 + 0,  posf4 + 0,  posf5 + 1, 
                              i, numGradVecs);
    float result100001 =  vec10 * tmp[0] +  vec01 * tmp[1] +  vec02 * tmp[2] +  vec03 * tmp[3] +  vec04 * tmp[4] +  vec15 * tmp[5] + 
    0;
     tmp =
        gradVecs +
        6 * hash6( posf0 + 1,  posf1 + 0,  posf2 + 0,  posf3 + 0,  posf4 + 1,  posf5 + 0, 
                              i, numGradVecs);
    float result100010 =  vec10 * tmp[0] +  vec01 * tmp[1] +  vec02 * tmp[2] +  vec03 * tmp[3] +  vec14 * tmp[4] +  vec05 * tmp[5] + 
    0;
     tmp =
        gradVecs +
        6 * hash6( posf0 + 1,  posf1 + 0,  posf2 + 0,  posf3 + 0,  posf4 + 1,  posf5 + 1, 
                              i, numGradVecs);
    float result100011 =  vec10 * tmp[0] +  vec01 * tmp[1] +  vec02 * tmp[2] +  vec03 * tmp[3] +  vec14 * tmp[4] +  vec15 * tmp[5] + 
    0;
     tmp =
        gradVecs +
        6 * hash6( posf0 + 1,  posf1 + 0,  posf2 + 0,  posf3 + 1,  posf4 + 0,  posf5 + 0, 
                              i, numGradVecs);
    float result100100 =  vec10 * tmp[0] +  vec01 * tmp[1] +  vec02 * tmp[2] +  vec13 * tmp[3] +  vec04 * tmp[4] +  vec05 * tmp[5] + 
    0;
     tmp =
        gradVecs +
        6 * hash6( posf0 + 1,  posf1 + 0,  posf2 + 0,  posf3 + 1,  posf4 + 0,  posf5 + 1, 
                              i, numGradVecs);
    float result100101 =  vec10 * tmp[0] +  vec01 * tmp[1] +  vec02 * tmp[2] +  vec13 * tmp[3] +  vec04 * tmp[4] +  vec15 * tmp[5] + 
    0;
     tmp =
        gradVecs +
        6 * hash6( posf0 + 1,  posf1 + 0,  posf2 + 0,  posf3 + 1,  posf4 + 1,  posf5 + 0, 
                              i, numGradVecs);
    float result100110 =  vec10 * tmp[0] +  vec01 * tmp[1] +  vec02 * tmp[2] +  vec13 * tmp[3] +  vec14 * tmp[4] +  vec05 * tmp[5] + 
    0;
     tmp =
        gradVecs +
        6 * hash6( posf0 + 1,  posf1 + 0,  posf2 + 0,  posf3 + 1,  posf4 + 1,  posf5 + 1, 
                              i, numGradVecs);
    float result100111 =  vec10 * tmp[0] +  vec01 * tmp[1] +  vec02 * tmp[2] +  vec13 * tmp[3] +  vec14 * tmp[4] +  vec15 * tmp[5] + 
    0;
     tmp =
        gradVecs +
        6 * hash6( posf0 + 1,  posf1 + 0,  posf2 + 1,  posf3 + 0,  posf4 + 0,  posf5 + 0, 
                              i, numGradVecs);
    float result101000 =  vec10 * tmp[0] +  vec01 * tmp[1] +  vec12 * tmp[2] +  vec03 * tmp[3] +  vec04 * tmp[4] +  vec05 * tmp[5] + 
    0;
     tmp =
        gradVecs +
        6 * hash6( posf0 + 1,  posf1 + 0,  posf2 + 1,  posf3 + 0,  posf4 + 0,  posf5 + 1, 
                              i, numGradVecs);
    float result101001 =  vec10 * tmp[0] +  vec01 * tmp[1] +  vec12 * tmp[2] +  vec03 * tmp[3] +  vec04 * tmp[4] +  vec15 * tmp[5] + 
    0;
     tmp =
        gradVecs +
        6 * hash6( posf0 + 1,  posf1 + 0,  posf2 + 1,  posf3 + 0,  posf4 + 1,  posf5 + 0, 
                              i, numGradVecs);
    float result101010 =  vec10 * tmp[0] +  vec01 * tmp[1] +  vec12 * tmp[2] +  vec03 * tmp[3] +  vec14 * tmp[4] +  vec05 * tmp[5] + 
    0;
     tmp =
        gradVecs +
        6 * hash6( posf0 + 1,  posf1 + 0,  posf2 + 1,  posf3 + 0,  posf4 + 1,  posf5 + 1, 
                              i, numGradVecs);
    float result101011 =  vec10 * tmp[0] +  vec01 * tmp[1] +  vec12 * tmp[2] +  vec03 * tmp[3] +  vec14 * tmp[4] +  vec15 * tmp[5] + 
    0;
     tmp =
        gradVecs +
        6 * hash6( posf0 + 1,  posf1 + 0,  posf2 + 1,  posf3 + 1,  posf4 + 0,  posf5 + 0, 
                              i, numGradVecs);
    float result101100 =  vec10 * tmp[0] +  vec01 * tmp[1] +  vec12 * tmp[2] +  vec13 * tmp[3] +  vec04 * tmp[4] +  vec05 * tmp[5] + 
    0;
     tmp =
        gradVecs +
        6 * hash6( posf0 + 1,  posf1 + 0,  posf2 + 1,  posf3 + 1,  posf4 + 0,  posf5 + 1, 
                              i, numGradVecs);
    float result101101 =  vec10 * tmp[0] +  vec01 * tmp[1] +  vec12 * tmp[2] +  vec13 * tmp[3] +  vec04 * tmp[4] +  vec15 * tmp[5] + 
    0;
     tmp =
        gradVecs +
        6 * hash6( posf0 + 1,  posf1 + 0,  posf2 + 1,  posf3 + 1,  posf4 + 1,  posf5 + 0, 
                              i, numGradVecs);
    float result101110 =  vec10 * tmp[0] +  vec01 * tmp[1] +  vec12 * tmp[2] +  vec13 * tmp[3] +  vec14 * tmp[4] +  vec05 * tmp[5] + 
    0;
     tmp =
        gradVecs +
        6 * hash6( posf0 + 1,  posf1 + 0,  posf2 + 1,  posf3 + 1,  posf4 + 1,  posf5 + 1, 
                              i, numGradVecs);
    float result101111 =  vec10 * tmp[0] +  vec01 * tmp[1] +  vec12 * tmp[2] +  vec13 * tmp[3] +  vec14 * tmp[4] +  vec15 * tmp[5] + 
    0;
     tmp =
        gradVecs +
        6 * hash6( posf0 + 1,  posf1 + 1,  posf2 + 0,  posf3 + 0,  posf4 + 0,  posf5 + 0, 
                              i, numGradVecs);
    float result110000 =  vec10 * tmp[0] +  vec11 * tmp[1] +  vec02 * tmp[2] +  vec03 * tmp[3] +  vec04 * tmp[4] +  vec05 * tmp[5] + 
    0;
     tmp =
        gradVecs +
        6 * hash6( posf0 + 1,  posf1 + 1,  posf2 + 0,  posf3 + 0,  posf4 + 0,  posf5 + 1, 
                              i, numGradVecs);
    float result110001 =  vec10 * tmp[0] +  vec11 * tmp[1] +  vec02 * tmp[2] +  vec03 * tmp[3] +  vec04 * tmp[4] +  vec15 * tmp[5] + 
    0;
     tmp =
        gradVecs +
        6 * hash6( posf0 + 1,  posf1 + 1,  posf2 + 0,  posf3 + 0,  posf4 + 1,  posf5 + 0, 
                              i, numGradVecs);
    float result110010 =  vec10 * tmp[0] +  vec11 * tmp[1] +  vec02 * tmp[2] +  vec03 * tmp[3] +  vec14 * tmp[4] +  vec05 * tmp[5] + 
    0;
     tmp =
        gradVecs +
        6 * hash6( posf0 + 1,  posf1 + 1,  posf2 + 0,  posf3 + 0,  posf4 + 1,  posf5 + 1, 
                              i, numGradVecs);
    float result110011 =  vec10 * tmp[0] +  vec11 * tmp[1] +  vec02 * tmp[2] +  vec03 * tmp[3] +  vec14 * tmp[4] +  vec15 * tmp[5] + 
    0;
     tmp =
        gradVecs +
        6 * hash6( posf0 + 1,  posf1 + 1,  posf2 + 0,  posf3 + 1,  posf4 + 0,  posf5 + 0, 
                              i, numGradVecs);
    float result110100 =  vec10 * tmp[0] +  vec11 * tmp[1] +  vec02 * tmp[2] +  vec13 * tmp[3] +  vec04 * tmp[4] +  vec05 * tmp[5] + 
    0;
     tmp =
        gradVecs +
        6 * hash6( posf0 + 1,  posf1 + 1,  posf2 + 0,  posf3 + 1,  posf4 + 0,  posf5 + 1, 
                              i, numGradVecs);
    float result110101 =  vec10 * tmp[0] +  vec11 * tmp[1] +  vec02 * tmp[2] +  vec13 * tmp[3] +  vec04 * tmp[4] +  vec15 * tmp[5] + 
    0;
     tmp =
        gradVecs +
        6 * hash6( posf0 + 1,  posf1 + 1,  posf2 + 0,  posf3 + 1,  posf4 + 1,  posf5 + 0, 
                              i, numGradVecs);
    float result110110 =  vec10 * tmp[0] +  vec11 * tmp[1] +  vec02 * tmp[2] +  vec13 * tmp[3] +  vec14 * tmp[4] +  vec05 * tmp[5] + 
    0;
     tmp =
        gradVecs +
        6 * hash6( posf0 + 1,  posf1 + 1,  posf2 + 0,  posf3 + 1,  posf4 + 1,  posf5 + 1, 
                              i, numGradVecs);
    float result110111 =  vec10 * tmp[0] +  vec11 * tmp[1] +  vec02 * tmp[2] +  vec13 * tmp[3] +  vec14 * tmp[4] +  vec15 * tmp[5] + 
    0;
     tmp =
        gradVecs +
        6 * hash6( posf0 + 1,  posf1 + 1,  posf2 + 1,  posf3 + 0,  posf4 + 0,  posf5 + 0, 
                              i, numGradVecs);
    float result111000 =  vec10 * tmp[0] +  vec11 * tmp[1] +  vec12 * tmp[2] +  vec03 * tmp[3] +  vec04 * tmp[4] +  vec05 * tmp[5] + 
    0;
     tmp =
        gradVecs +
        6 * hash6( posf0 + 1,  posf1 + 1,  posf2 + 1,  posf3 + 0,  posf4 + 0,  posf5 + 1, 
                              i, numGradVecs);
    float result111001 =  vec10 * tmp[0] +  vec11 * tmp[1] +  vec12 * tmp[2] +  vec03 * tmp[3] +  vec04 * tmp[4] +  vec15 * tmp[5] + 
    0;
     tmp =
        gradVecs +
        6 * hash6( posf0 + 1,  posf1 + 1,  posf2 + 1,  posf3 + 0,  posf4 + 1,  posf5 + 0, 
                              i, numGradVecs);
    float result111010 =  vec10 * tmp[0] +  vec11 * tmp[1] +  vec12 * tmp[2] +  vec03 * tmp[3] +  vec14 * tmp[4] +  vec05 * tmp[5] + 
    0;
     tmp =
        gradVecs +
        6 * hash6( posf0 + 1,  posf1 + 1,  posf2 + 1,  posf3 + 0,  posf4 + 1,  posf5 + 1, 
                              i, numGradVecs);
    float result111011 =  vec10 * tmp[0] +  vec11 * tmp[1] +  vec12 * tmp[2] +  vec03 * tmp[3] +  vec14 * tmp[4] +  vec15 * tmp[5] + 
    0;
     tmp =
        gradVecs +
        6 * hash6( posf0 + 1,  posf1 + 1,  posf2 + 1,  posf3 + 1,  posf4 + 0,  posf5 + 0, 
                              i, numGradVecs);
    float result111100 =  vec10 * tmp[0] +  vec11 * tmp[1] +  vec12 * tmp[2] +  vec13 * tmp[3] +  vec04 * tmp[4] +  vec05 * tmp[5] + 
    0;
     tmp =
        gradVecs +
        6 * hash6( posf0 + 1,  posf1 + 1,  posf2 + 1,  posf3 + 1,  posf4 + 0,  posf5 + 1, 
                              i, numGradVecs);
    float result111101 =  vec10 * tmp[0] +  vec11 * tmp[1] +  vec12 * tmp[2] +  vec13 * tmp[3] +  vec04 * tmp[4] +  vec15 * tmp[5] + 
    0;
     tmp =
        gradVecs +
        6 * hash6( posf0 + 1,  posf1 + 1,  posf2 + 1,  posf3 + 1,  posf4 + 1,  posf5 + 0, 
                              i, numGradVecs);
    float result111110 =  vec10 * tmp[0] +  vec11 * tmp[1] +  vec12 * tmp[2] +  vec13 * tmp[3] +  vec14 * tmp[4] +  vec05 * tmp[5] + 
    0;
     tmp =
        gradVecs +
        6 * hash6( posf0 + 1,  posf1 + 1,  posf2 + 1,  posf3 + 1,  posf4 + 1,  posf5 + 1, 
                              i, numGradVecs);
    float result111111 =  vec10 * tmp[0] +  vec11 * tmp[1] +  vec12 * tmp[2] +  vec13 * tmp[3] +  vec14 * tmp[4] +  vec15 * tmp[5] + 
    0;
    
      float result00000 =
        result000000 + lerp0 * (result100000 - result000000);
     float result00001 =
        result000001 + lerp0 * (result100001 - result000001);
     float result00010 =
        result000010 + lerp0 * (result100010 - result000010);
     float result00011 =
        result000011 + lerp0 * (result100011 - result000011);
     float result00100 =
        result000100 + lerp0 * (result100100 - result000100);
     float result00101 =
        result000101 + lerp0 * (result100101 - result000101);
     float result00110 =
        result000110 + lerp0 * (result100110 - result000110);
     float result00111 =
        result000111 + lerp0 * (result100111 - result000111);
     float result01000 =
        result001000 + lerp0 * (result101000 - result001000);
     float result01001 =
        result001001 + lerp0 * (result101001 - result001001);
     float result01010 =
        result001010 + lerp0 * (result101010 - result001010);
     float result01011 =
        result001011 + lerp0 * (result101011 - result001011);
     float result01100 =
        result001100 + lerp0 * (result101100 - result001100);
     float result01101 =
        result001101 + lerp0 * (result101101 - result001101);
     float result01110 =
        result001110 + lerp0 * (result101110 - result001110);
     float result01111 =
        result001111 + lerp0 * (result101111 - result001111);
     float result10000 =
        result010000 + lerp0 * (result110000 - result010000);
     float result10001 =
        result010001 + lerp0 * (result110001 - result010001);
     float result10010 =
        result010010 + lerp0 * (result110010 - result010010);
     float result10011 =
        result010011 + lerp0 * (result110011 - result010011);
     float result10100 =
        result010100 + lerp0 * (result110100 - result010100);
     float result10101 =
        result010101 + lerp0 * (result110101 - result010101);
     float result10110 =
        result010110 + lerp0 * (result110110 - result010110);
     float result10111 =
        result010111 + lerp0 * (result110111 - result010111);
     float result11000 =
        result011000 + lerp0 * (result111000 - result011000);
     float result11001 =
        result011001 + lerp0 * (result111001 - result011001);
     float result11010 =
        result011010 + lerp0 * (result111010 - result011010);
     float result11011 =
        result011011 + lerp0 * (result111011 - result011011);
     float result11100 =
        result011100 + lerp0 * (result111100 - result011100);
     float result11101 =
        result011101 + lerp0 * (result111101 - result011101);
     float result11110 =
        result011110 + lerp0 * (result111110 - result011110);
     float result11111 =
        result011111 + lerp0 * (result111111 - result011111);
    
      float result0000 =
        result00000 + lerp1 * (result10000 - result00000);
     float result0001 =
        result00001 + lerp1 * (result10001 - result00001);
     float result0010 =
        result00010 + lerp1 * (result10010 - result00010);
     float result0011 =
        result00011 + lerp1 * (result10011 - result00011);
     float result0100 =
        result00100 + lerp1 * (result10100 - result00100);
     float result0101 =
        result00101 + lerp1 * (result10101 - result00101);
     float result0110 =
        result00110 + lerp1 * (result10110 - result00110);
     float result0111 =
        result00111 + lerp1 * (result10111 - result00111);
     float result1000 =
        result01000 + lerp1 * (result11000 - result01000);
     float result1001 =
        result01001 + lerp1 * (result11001 - result01001);
     float result1010 =
        result01010 + lerp1 * (result11010 - result01010);
     float result1011 =
        result01011 + lerp1 * (result11011 - result01011);
     float result1100 =
        result01100 + lerp1 * (result11100 - result01100);
     float result1101 =
        result01101 + lerp1 * (result11101 - result01101);
     float result1110 =
        result01110 + lerp1 * (result11110 - result01110);
     float result1111 =
        result01111 + lerp1 * (result11111 - result01111);
    
      float result000 =
        result0000 + lerp2 * (result1000 - result0000);
     float result001 =
        result0001 + lerp2 * (result1001 - result0001);
     float result010 =
        result0010 + lerp2 * (result1010 - result0010);
     float result011 =
        result0011 + lerp2 * (result1011 - result0011);
     float result100 =
        result0100 + lerp2 * (result1100 - result0100);
     float result101 =
        result0101 + lerp2 * (result1101 - result0101);
     float result110 =
        result0110 + lerp2 * (result1110 - result0110);
     float result111 =
        result0111 + lerp2 * (result1111 - result0111);
    
      float result00 =
        result000 + lerp3 * (result100 - result000);
     float result01 =
        result001 + lerp3 * (result101 - result001);
     float result10 =
        result010 + lerp3 * (result110 - result010);
     float result11 =
        result011 + lerp3 * (result111 - result011);
    
      float result0 =
        result00 + lerp4 * (result10 - result00);
     float result1 =
        result01 + lerp4 * (result11 - result01);
    
      float result =
        result0 + lerp5 * (result1 - result0);
    
    
    total += result * amplitude;
    amplitude *= persistence;
     pos0 *= 2;
     pos1 *= 2;
     pos2 *= 2;
     pos3 *= 2;
     pos4 *= 2;
     pos5 *= 2;
    
  }
  return total;
}

#ifndef TERRAIN_NON_REP_
#define TERRAIN_NON_REP_

int fastFloor(float x) {
  int i = x;
  return i - (x < i);
}

#endif // TERRAIN_NON_REP_

uint hash7( int pos0,  int pos1,  int pos2,  int pos3,  int pos4,  int pos5,  int pos6, 
                 uint octave, uint numGradVecs) {
  // TODO: WRITE THIS
  return (pos0 * pos0 + pos1 + pos2 * pos2 * pos2 + octave) % numGradVecs;
}

float getNoise7( float scale0, float pos0,  float scale1, float pos1,  float scale2, float pos2,  float scale3, float pos3,  float scale4, float pos4,  float scale5, float pos5,  float scale6, float pos6, 
                      __global float *gradVecs, uint numGradVecs,
                      int numOctaves, float persistence) {
  float total = 0;
  float amplitude = 1;
   pos0 = (pos0 + 0.5) / scale0;
   pos1 = (pos1 + 0.5) / scale1;
   pos2 = (pos2 + 0.5) / scale2;
   pos3 = (pos3 + 0.5) / scale3;
   pos4 = (pos4 + 0.5) / scale4;
   pos5 = (pos5 + 0.5) / scale5;
   pos6 = (pos6 + 0.5) / scale6;
  
  for (uint i = numOctaves; i--;) {
     int posf0 = fastFloor(pos0);
    float vec00 = pos0 - posf0;
    float vec10 = vec00 - 1;
    float lerp0 = vec00 * vec00 * (3 - 2 * vec00);
     int posf1 = fastFloor(pos1);
    float vec01 = pos1 - posf1;
    float vec11 = vec01 - 1;
    float lerp1 = vec01 * vec01 * (3 - 2 * vec01);
     int posf2 = fastFloor(pos2);
    float vec02 = pos2 - posf2;
    float vec12 = vec02 - 1;
    float lerp2 = vec02 * vec02 * (3 - 2 * vec02);
     int posf3 = fastFloor(pos3);
    float vec03 = pos3 - posf3;
    float vec13 = vec03 - 1;
    float lerp3 = vec03 * vec03 * (3 - 2 * vec03);
     int posf4 = fastFloor(pos4);
    float vec04 = pos4 - posf4;
    float vec14 = vec04 - 1;
    float lerp4 = vec04 * vec04 * (3 - 2 * vec04);
     int posf5 = fastFloor(pos5);
    float vec05 = pos5 - posf5;
    float vec15 = vec05 - 1;
    float lerp5 = vec05 * vec05 * (3 - 2 * vec05);
     int posf6 = fastFloor(pos6);
    float vec06 = pos6 - posf6;
    float vec16 = vec06 - 1;
    float lerp6 = vec06 * vec06 * (3 - 2 * vec06);
    
    __global float *tmp;
     tmp =
        gradVecs +
        7 * hash7( posf0 + 0,  posf1 + 0,  posf2 + 0,  posf3 + 0,  posf4 + 0,  posf5 + 0,  posf6 + 0, 
                              i, numGradVecs);
    float result0000000 =  vec00 * tmp[0] +  vec01 * tmp[1] +  vec02 * tmp[2] +  vec03 * tmp[3] +  vec04 * tmp[4] +  vec05 * tmp[5] +  vec06 * tmp[6] + 
    0;
     tmp =
        gradVecs +
        7 * hash7( posf0 + 0,  posf1 + 0,  posf2 + 0,  posf3 + 0,  posf4 + 0,  posf5 + 0,  posf6 + 1, 
                              i, numGradVecs);
    float result0000001 =  vec00 * tmp[0] +  vec01 * tmp[1] +  vec02 * tmp[2] +  vec03 * tmp[3] +  vec04 * tmp[4] +  vec05 * tmp[5] +  vec16 * tmp[6] + 
    0;
     tmp =
        gradVecs +
        7 * hash7( posf0 + 0,  posf1 + 0,  posf2 + 0,  posf3 + 0,  posf4 + 0,  posf5 + 1,  posf6 + 0, 
                              i, numGradVecs);
    float result0000010 =  vec00 * tmp[0] +  vec01 * tmp[1] +  vec02 * tmp[2] +  vec03 * tmp[3] +  vec04 * tmp[4] +  vec15 * tmp[5] +  vec06 * tmp[6] + 
    0;
     tmp =
        gradVecs +
        7 * hash7( posf0 + 0,  posf1 + 0,  posf2 + 0,  posf3 + 0,  posf4 + 0,  posf5 + 1,  posf6 + 1, 
                              i, numGradVecs);
    float result0000011 =  vec00 * tmp[0] +  vec01 * tmp[1] +  vec02 * tmp[2] +  vec03 * tmp[3] +  vec04 * tmp[4] +  vec15 * tmp[5] +  vec16 * tmp[6] + 
    0;
     tmp =
        gradVecs +
        7 * hash7( posf0 + 0,  posf1 + 0,  posf2 + 0,  posf3 + 0,  posf4 + 1,  posf5 + 0,  posf6 + 0, 
                              i, numGradVecs);
    float result0000100 =  vec00 * tmp[0] +  vec01 * tmp[1] +  vec02 * tmp[2] +  vec03 * tmp[3] +  vec14 * tmp[4] +  vec05 * tmp[5] +  vec06 * tmp[6] + 
    0;
     tmp =
        gradVecs +
        7 * hash7( posf0 + 0,  posf1 + 0,  posf2 + 0,  posf3 + 0,  posf4 + 1,  posf5 + 0,  posf6 + 1, 
                              i, numGradVecs);
    float result0000101 =  vec00 * tmp[0] +  vec01 * tmp[1] +  vec02 * tmp[2] +  vec03 * tmp[3] +  vec14 * tmp[4] +  vec05 * tmp[5] +  vec16 * tmp[6] + 
    0;
     tmp =
        gradVecs +
        7 * hash7( posf0 + 0,  posf1 + 0,  posf2 + 0,  posf3 + 0,  posf4 + 1,  posf5 + 1,  posf6 + 0, 
                              i, numGradVecs);
    float result0000110 =  vec00 * tmp[0] +  vec01 * tmp[1] +  vec02 * tmp[2] +  vec03 * tmp[3] +  vec14 * tmp[4] +  vec15 * tmp[5] +  vec06 * tmp[6] + 
    0;
     tmp =
        gradVecs +
        7 * hash7( posf0 + 0,  posf1 + 0,  posf2 + 0,  posf3 + 0,  posf4 + 1,  posf5 + 1,  posf6 + 1, 
                              i, numGradVecs);
    float result0000111 =  vec00 * tmp[0] +  vec01 * tmp[1] +  vec02 * tmp[2] +  vec03 * tmp[3] +  vec14 * tmp[4] +  vec15 * tmp[5] +  vec16 * tmp[6] + 
    0;
     tmp =
        gradVecs +
        7 * hash7( posf0 + 0,  posf1 + 0,  posf2 + 0,  posf3 + 1,  posf4 + 0,  posf5 + 0,  posf6 + 0, 
                              i, numGradVecs);
    float result0001000 =  vec00 * tmp[0] +  vec01 * tmp[1] +  vec02 * tmp[2] +  vec13 * tmp[3] +  vec04 * tmp[4] +  vec05 * tmp[5] +  vec06 * tmp[6] + 
    0;
     tmp =
        gradVecs +
        7 * hash7( posf0 + 0,  posf1 + 0,  posf2 + 0,  posf3 + 1,  posf4 + 0,  posf5 + 0,  posf6 + 1, 
                              i, numGradVecs);
    float result0001001 =  vec00 * tmp[0] +  vec01 * tmp[1] +  vec02 * tmp[2] +  vec13 * tmp[3] +  vec04 * tmp[4] +  vec05 * tmp[5] +  vec16 * tmp[6] + 
    0;
     tmp =
        gradVecs +
        7 * hash7( posf0 + 0,  posf1 + 0,  posf2 + 0,  posf3 + 1,  posf4 + 0,  posf5 + 1,  posf6 + 0, 
                              i, numGradVecs);
    float result0001010 =  vec00 * tmp[0] +  vec01 * tmp[1] +  vec02 * tmp[2] +  vec13 * tmp[3] +  vec04 * tmp[4] +  vec15 * tmp[5] +  vec06 * tmp[6] + 
    0;
     tmp =
        gradVecs +
        7 * hash7( posf0 + 0,  posf1 + 0,  posf2 + 0,  posf3 + 1,  posf4 + 0,  posf5 + 1,  posf6 + 1, 
                              i, numGradVecs);
    float result0001011 =  vec00 * tmp[0] +  vec01 * tmp[1] +  vec02 * tmp[2] +  vec13 * tmp[3] +  vec04 * tmp[4] +  vec15 * tmp[5] +  vec16 * tmp[6] + 
    0;
     tmp =
        gradVecs +
        7 * hash7( posf0 + 0,  posf1 + 0,  posf2 + 0,  posf3 + 1,  posf4 + 1,  posf5 + 0,  posf6 + 0, 
                              i, numGradVecs);
    float result0001100 =  vec00 * tmp[0] +  vec01 * tmp[1] +  vec02 * tmp[2] +  vec13 * tmp[3] +  vec14 * tmp[4] +  vec05 * tmp[5] +  vec06 * tmp[6] + 
    0;
     tmp =
        gradVecs +
        7 * hash7( posf0 + 0,  posf1 + 0,  posf2 + 0,  posf3 + 1,  posf4 + 1,  posf5 + 0,  posf6 + 1, 
                              i, numGradVecs);
    float result0001101 =  vec00 * tmp[0] +  vec01 * tmp[1] +  vec02 * tmp[2] +  vec13 * tmp[3] +  vec14 * tmp[4] +  vec05 * tmp[5] +  vec16 * tmp[6] + 
    0;
     tmp =
        gradVecs +
        7 * hash7( posf0 + 0,  posf1 + 0,  posf2 + 0,  posf3 + 1,  posf4 + 1,  posf5 + 1,  posf6 + 0, 
                              i, numGradVecs);
    float result0001110 =  vec00 * tmp[0] +  vec01 * tmp[1] +  vec02 * tmp[2] +  vec13 * tmp[3] +  vec14 * tmp[4] +  vec15 * tmp[5] +  vec06 * tmp[6] + 
    0;
     tmp =
        gradVecs +
        7 * hash7( posf0 + 0,  posf1 + 0,  posf2 + 0,  posf3 + 1,  posf4 + 1,  posf5 + 1,  posf6 + 1, 
                              i, numGradVecs);
    float result0001111 =  vec00 * tmp[0] +  vec01 * tmp[1] +  vec02 * tmp[2] +  vec13 * tmp[3] +  vec14 * tmp[4] +  vec15 * tmp[5] +  vec16 * tmp[6] + 
    0;
     tmp =
        gradVecs +
        7 * hash7( posf0 + 0,  posf1 + 0,  posf2 + 1,  posf3 + 0,  posf4 + 0,  posf5 + 0,  posf6 + 0, 
                              i, numGradVecs);
    float result0010000 =  vec00 * tmp[0] +  vec01 * tmp[1] +  vec12 * tmp[2] +  vec03 * tmp[3] +  vec04 * tmp[4] +  vec05 * tmp[5] +  vec06 * tmp[6] + 
    0;
     tmp =
        gradVecs +
        7 * hash7( posf0 + 0,  posf1 + 0,  posf2 + 1,  posf3 + 0,  posf4 + 0,  posf5 + 0,  posf6 + 1, 
                              i, numGradVecs);
    float result0010001 =  vec00 * tmp[0] +  vec01 * tmp[1] +  vec12 * tmp[2] +  vec03 * tmp[3] +  vec04 * tmp[4] +  vec05 * tmp[5] +  vec16 * tmp[6] + 
    0;
     tmp =
        gradVecs +
        7 * hash7( posf0 + 0,  posf1 + 0,  posf2 + 1,  posf3 + 0,  posf4 + 0,  posf5 + 1,  posf6 + 0, 
                              i, numGradVecs);
    float result0010010 =  vec00 * tmp[0] +  vec01 * tmp[1] +  vec12 * tmp[2] +  vec03 * tmp[3] +  vec04 * tmp[4] +  vec15 * tmp[5] +  vec06 * tmp[6] + 
    0;
     tmp =
        gradVecs +
        7 * hash7( posf0 + 0,  posf1 + 0,  posf2 + 1,  posf3 + 0,  posf4 + 0,  posf5 + 1,  posf6 + 1, 
                              i, numGradVecs);
    float result0010011 =  vec00 * tmp[0] +  vec01 * tmp[1] +  vec12 * tmp[2] +  vec03 * tmp[3] +  vec04 * tmp[4] +  vec15 * tmp[5] +  vec16 * tmp[6] + 
    0;
     tmp =
        gradVecs +
        7 * hash7( posf0 + 0,  posf1 + 0,  posf2 + 1,  posf3 + 0,  posf4 + 1,  posf5 + 0,  posf6 + 0, 
                              i, numGradVecs);
    float result0010100 =  vec00 * tmp[0] +  vec01 * tmp[1] +  vec12 * tmp[2] +  vec03 * tmp[3] +  vec14 * tmp[4] +  vec05 * tmp[5] +  vec06 * tmp[6] + 
    0;
     tmp =
        gradVecs +
        7 * hash7( posf0 + 0,  posf1 + 0,  posf2 + 1,  posf3 + 0,  posf4 + 1,  posf5 + 0,  posf6 + 1, 
                              i, numGradVecs);
    float result0010101 =  vec00 * tmp[0] +  vec01 * tmp[1] +  vec12 * tmp[2] +  vec03 * tmp[3] +  vec14 * tmp[4] +  vec05 * tmp[5] +  vec16 * tmp[6] + 
    0;
     tmp =
        gradVecs +
        7 * hash7( posf0 + 0,  posf1 + 0,  posf2 + 1,  posf3 + 0,  posf4 + 1,  posf5 + 1,  posf6 + 0, 
                              i, numGradVecs);
    float result0010110 =  vec00 * tmp[0] +  vec01 * tmp[1] +  vec12 * tmp[2] +  vec03 * tmp[3] +  vec14 * tmp[4] +  vec15 * tmp[5] +  vec06 * tmp[6] + 
    0;
     tmp =
        gradVecs +
        7 * hash7( posf0 + 0,  posf1 + 0,  posf2 + 1,  posf3 + 0,  posf4 + 1,  posf5 + 1,  posf6 + 1, 
                              i, numGradVecs);
    float result0010111 =  vec00 * tmp[0] +  vec01 * tmp[1] +  vec12 * tmp[2] +  vec03 * tmp[3] +  vec14 * tmp[4] +  vec15 * tmp[5] +  vec16 * tmp[6] + 
    0;
     tmp =
        gradVecs +
        7 * hash7( posf0 + 0,  posf1 + 0,  posf2 + 1,  posf3 + 1,  posf4 + 0,  posf5 + 0,  posf6 + 0, 
                              i, numGradVecs);
    float result0011000 =  vec00 * tmp[0] +  vec01 * tmp[1] +  vec12 * tmp[2] +  vec13 * tmp[3] +  vec04 * tmp[4] +  vec05 * tmp[5] +  vec06 * tmp[6] + 
    0;
     tmp =
        gradVecs +
        7 * hash7( posf0 + 0,  posf1 + 0,  posf2 + 1,  posf3 + 1,  posf4 + 0,  posf5 + 0,  posf6 + 1, 
                              i, numGradVecs);
    float result0011001 =  vec00 * tmp[0] +  vec01 * tmp[1] +  vec12 * tmp[2] +  vec13 * tmp[3] +  vec04 * tmp[4] +  vec05 * tmp[5] +  vec16 * tmp[6] + 
    0;
     tmp =
        gradVecs +
        7 * hash7( posf0 + 0,  posf1 + 0,  posf2 + 1,  posf3 + 1,  posf4 + 0,  posf5 + 1,  posf6 + 0, 
                              i, numGradVecs);
    float result0011010 =  vec00 * tmp[0] +  vec01 * tmp[1] +  vec12 * tmp[2] +  vec13 * tmp[3] +  vec04 * tmp[4] +  vec15 * tmp[5] +  vec06 * tmp[6] + 
    0;
     tmp =
        gradVecs +
        7 * hash7( posf0 + 0,  posf1 + 0,  posf2 + 1,  posf3 + 1,  posf4 + 0,  posf5 + 1,  posf6 + 1, 
                              i, numGradVecs);
    float result0011011 =  vec00 * tmp[0] +  vec01 * tmp[1] +  vec12 * tmp[2] +  vec13 * tmp[3] +  vec04 * tmp[4] +  vec15 * tmp[5] +  vec16 * tmp[6] + 
    0;
     tmp =
        gradVecs +
        7 * hash7( posf0 + 0,  posf1 + 0,  posf2 + 1,  posf3 + 1,  posf4 + 1,  posf5 + 0,  posf6 + 0, 
                              i, numGradVecs);
    float result0011100 =  vec00 * tmp[0] +  vec01 * tmp[1] +  vec12 * tmp[2] +  vec13 * tmp[3] +  vec14 * tmp[4] +  vec05 * tmp[5] +  vec06 * tmp[6] + 
    0;
     tmp =
        gradVecs +
        7 * hash7( posf0 + 0,  posf1 + 0,  posf2 + 1,  posf3 + 1,  posf4 + 1,  posf5 + 0,  posf6 + 1, 
                              i, numGradVecs);
    float result0011101 =  vec00 * tmp[0] +  vec01 * tmp[1] +  vec12 * tmp[2] +  vec13 * tmp[3] +  vec14 * tmp[4] +  vec05 * tmp[5] +  vec16 * tmp[6] + 
    0;
     tmp =
        gradVecs +
        7 * hash7( posf0 + 0,  posf1 + 0,  posf2 + 1,  posf3 + 1,  posf4 + 1,  posf5 + 1,  posf6 + 0, 
                              i, numGradVecs);
    float result0011110 =  vec00 * tmp[0] +  vec01 * tmp[1] +  vec12 * tmp[2] +  vec13 * tmp[3] +  vec14 * tmp[4] +  vec15 * tmp[5] +  vec06 * tmp[6] + 
    0;
     tmp =
        gradVecs +
        7 * hash7( posf0 + 0,  posf1 + 0,  posf2 + 1,  posf3 + 1,  posf4 + 1,  posf5 + 1,  posf6 + 1, 
                              i, numGradVecs);
    float result0011111 =  vec00 * tmp[0] +  vec01 * tmp[1] +  vec12 * tmp[2] +  vec13 * tmp[3] +  vec14 * tmp[4] +  vec15 * tmp[5] +  vec16 * tmp[6] + 
    0;
     tmp =
        gradVecs +
        7 * hash7( posf0 + 0,  posf1 + 1,  posf2 + 0,  posf3 + 0,  posf4 + 0,  posf5 + 0,  posf6 + 0, 
                              i, numGradVecs);
    float result0100000 =  vec00 * tmp[0] +  vec11 * tmp[1] +  vec02 * tmp[2] +  vec03 * tmp[3] +  vec04 * tmp[4] +  vec05 * tmp[5] +  vec06 * tmp[6] + 
    0;
     tmp =
        gradVecs +
        7 * hash7( posf0 + 0,  posf1 + 1,  posf2 + 0,  posf3 + 0,  posf4 + 0,  posf5 + 0,  posf6 + 1, 
                              i, numGradVecs);
    float result0100001 =  vec00 * tmp[0] +  vec11 * tmp[1] +  vec02 * tmp[2] +  vec03 * tmp[3] +  vec04 * tmp[4] +  vec05 * tmp[5] +  vec16 * tmp[6] + 
    0;
     tmp =
        gradVecs +
        7 * hash7( posf0 + 0,  posf1 + 1,  posf2 + 0,  posf3 + 0,  posf4 + 0,  posf5 + 1,  posf6 + 0, 
                              i, numGradVecs);
    float result0100010 =  vec00 * tmp[0] +  vec11 * tmp[1] +  vec02 * tmp[2] +  vec03 * tmp[3] +  vec04 * tmp[4] +  vec15 * tmp[5] +  vec06 * tmp[6] + 
    0;
     tmp =
        gradVecs +
        7 * hash7( posf0 + 0,  posf1 + 1,  posf2 + 0,  posf3 + 0,  posf4 + 0,  posf5 + 1,  posf6 + 1, 
                              i, numGradVecs);
    float result0100011 =  vec00 * tmp[0] +  vec11 * tmp[1] +  vec02 * tmp[2] +  vec03 * tmp[3] +  vec04 * tmp[4] +  vec15 * tmp[5] +  vec16 * tmp[6] + 
    0;
     tmp =
        gradVecs +
        7 * hash7( posf0 + 0,  posf1 + 1,  posf2 + 0,  posf3 + 0,  posf4 + 1,  posf5 + 0,  posf6 + 0, 
                              i, numGradVecs);
    float result0100100 =  vec00 * tmp[0] +  vec11 * tmp[1] +  vec02 * tmp[2] +  vec03 * tmp[3] +  vec14 * tmp[4] +  vec05 * tmp[5] +  vec06 * tmp[6] + 
    0;
     tmp =
        gradVecs +
        7 * hash7( posf0 + 0,  posf1 + 1,  posf2 + 0,  posf3 + 0,  posf4 + 1,  posf5 + 0,  posf6 + 1, 
                              i, numGradVecs);
    float result0100101 =  vec00 * tmp[0] +  vec11 * tmp[1] +  vec02 * tmp[2] +  vec03 * tmp[3] +  vec14 * tmp[4] +  vec05 * tmp[5] +  vec16 * tmp[6] + 
    0;
     tmp =
        gradVecs +
        7 * hash7( posf0 + 0,  posf1 + 1,  posf2 + 0,  posf3 + 0,  posf4 + 1,  posf5 + 1,  posf6 + 0, 
                              i, numGradVecs);
    float result0100110 =  vec00 * tmp[0] +  vec11 * tmp[1] +  vec02 * tmp[2] +  vec03 * tmp[3] +  vec14 * tmp[4] +  vec15 * tmp[5] +  vec06 * tmp[6] + 
    0;
     tmp =
        gradVecs +
        7 * hash7( posf0 + 0,  posf1 + 1,  posf2 + 0,  posf3 + 0,  posf4 + 1,  posf5 + 1,  posf6 + 1, 
                              i, numGradVecs);
    float result0100111 =  vec00 * tmp[0] +  vec11 * tmp[1] +  vec02 * tmp[2] +  vec03 * tmp[3] +  vec14 * tmp[4] +  vec15 * tmp[5] +  vec16 * tmp[6] + 
    0;
     tmp =
        gradVecs +
        7 * hash7( posf0 + 0,  posf1 + 1,  posf2 + 0,  posf3 + 1,  posf4 + 0,  posf5 + 0,  posf6 + 0, 
                              i, numGradVecs);
    float result0101000 =  vec00 * tmp[0] +  vec11 * tmp[1] +  vec02 * tmp[2] +  vec13 * tmp[3] +  vec04 * tmp[4] +  vec05 * tmp[5] +  vec06 * tmp[6] + 
    0;
     tmp =
        gradVecs +
        7 * hash7( posf0 + 0,  posf1 + 1,  posf2 + 0,  posf3 + 1,  posf4 + 0,  posf5 + 0,  posf6 + 1, 
                              i, numGradVecs);
    float result0101001 =  vec00 * tmp[0] +  vec11 * tmp[1] +  vec02 * tmp[2] +  vec13 * tmp[3] +  vec04 * tmp[4] +  vec05 * tmp[5] +  vec16 * tmp[6] + 
    0;
     tmp =
        gradVecs +
        7 * hash7( posf0 + 0,  posf1 + 1,  posf2 + 0,  posf3 + 1,  posf4 + 0,  posf5 + 1,  posf6 + 0, 
                              i, numGradVecs);
    float result0101010 =  vec00 * tmp[0] +  vec11 * tmp[1] +  vec02 * tmp[2] +  vec13 * tmp[3] +  vec04 * tmp[4] +  vec15 * tmp[5] +  vec06 * tmp[6] + 
    0;
     tmp =
        gradVecs +
        7 * hash7( posf0 + 0,  posf1 + 1,  posf2 + 0,  posf3 + 1,  posf4 + 0,  posf5 + 1,  posf6 + 1, 
                              i, numGradVecs);
    float result0101011 =  vec00 * tmp[0] +  vec11 * tmp[1] +  vec02 * tmp[2] +  vec13 * tmp[3] +  vec04 * tmp[4] +  vec15 * tmp[5] +  vec16 * tmp[6] + 
    0;
     tmp =
        gradVecs +
        7 * hash7( posf0 + 0,  posf1 + 1,  posf2 + 0,  posf3 + 1,  posf4 + 1,  posf5 + 0,  posf6 + 0, 
                              i, numGradVecs);
    float result0101100 =  vec00 * tmp[0] +  vec11 * tmp[1] +  vec02 * tmp[2] +  vec13 * tmp[3] +  vec14 * tmp[4] +  vec05 * tmp[5] +  vec06 * tmp[6] + 
    0;
     tmp =
        gradVecs +
        7 * hash7( posf0 + 0,  posf1 + 1,  posf2 + 0,  posf3 + 1,  posf4 + 1,  posf5 + 0,  posf6 + 1, 
                              i, numGradVecs);
    float result0101101 =  vec00 * tmp[0] +  vec11 * tmp[1] +  vec02 * tmp[2] +  vec13 * tmp[3] +  vec14 * tmp[4] +  vec05 * tmp[5] +  vec16 * tmp[6] + 
    0;
     tmp =
        gradVecs +
        7 * hash7( posf0 + 0,  posf1 + 1,  posf2 + 0,  posf3 + 1,  posf4 + 1,  posf5 + 1,  posf6 + 0, 
                              i, numGradVecs);
    float result0101110 =  vec00 * tmp[0] +  vec11 * tmp[1] +  vec02 * tmp[2] +  vec13 * tmp[3] +  vec14 * tmp[4] +  vec15 * tmp[5] +  vec06 * tmp[6] + 
    0;
     tmp =
        gradVecs +
        7 * hash7( posf0 + 0,  posf1 + 1,  posf2 + 0,  posf3 + 1,  posf4 + 1,  posf5 + 1,  posf6 + 1, 
                              i, numGradVecs);
    float result0101111 =  vec00 * tmp[0] +  vec11 * tmp[1] +  vec02 * tmp[2] +  vec13 * tmp[3] +  vec14 * tmp[4] +  vec15 * tmp[5] +  vec16 * tmp[6] + 
    0;
     tmp =
        gradVecs +
        7 * hash7( posf0 + 0,  posf1 + 1,  posf2 + 1,  posf3 + 0,  posf4 + 0,  posf5 + 0,  posf6 + 0, 
                              i, numGradVecs);
    float result0110000 =  vec00 * tmp[0] +  vec11 * tmp[1] +  vec12 * tmp[2] +  vec03 * tmp[3] +  vec04 * tmp[4] +  vec05 * tmp[5] +  vec06 * tmp[6] + 
    0;
     tmp =
        gradVecs +
        7 * hash7( posf0 + 0,  posf1 + 1,  posf2 + 1,  posf3 + 0,  posf4 + 0,  posf5 + 0,  posf6 + 1, 
                              i, numGradVecs);
    float result0110001 =  vec00 * tmp[0] +  vec11 * tmp[1] +  vec12 * tmp[2] +  vec03 * tmp[3] +  vec04 * tmp[4] +  vec05 * tmp[5] +  vec16 * tmp[6] + 
    0;
     tmp =
        gradVecs +
        7 * hash7( posf0 + 0,  posf1 + 1,  posf2 + 1,  posf3 + 0,  posf4 + 0,  posf5 + 1,  posf6 + 0, 
                              i, numGradVecs);
    float result0110010 =  vec00 * tmp[0] +  vec11 * tmp[1] +  vec12 * tmp[2] +  vec03 * tmp[3] +  vec04 * tmp[4] +  vec15 * tmp[5] +  vec06 * tmp[6] + 
    0;
     tmp =
        gradVecs +
        7 * hash7( posf0 + 0,  posf1 + 1,  posf2 + 1,  posf3 + 0,  posf4 + 0,  posf5 + 1,  posf6 + 1, 
                              i, numGradVecs);
    float result0110011 =  vec00 * tmp[0] +  vec11 * tmp[1] +  vec12 * tmp[2] +  vec03 * tmp[3] +  vec04 * tmp[4] +  vec15 * tmp[5] +  vec16 * tmp[6] + 
    0;
     tmp =
        gradVecs +
        7 * hash7( posf0 + 0,  posf1 + 1,  posf2 + 1,  posf3 + 0,  posf4 + 1,  posf5 + 0,  posf6 + 0, 
                              i, numGradVecs);
    float result0110100 =  vec00 * tmp[0] +  vec11 * tmp[1] +  vec12 * tmp[2] +  vec03 * tmp[3] +  vec14 * tmp[4] +  vec05 * tmp[5] +  vec06 * tmp[6] + 
    0;
     tmp =
        gradVecs +
        7 * hash7( posf0 + 0,  posf1 + 1,  posf2 + 1,  posf3 + 0,  posf4 + 1,  posf5 + 0,  posf6 + 1, 
                              i, numGradVecs);
    float result0110101 =  vec00 * tmp[0] +  vec11 * tmp[1] +  vec12 * tmp[2] +  vec03 * tmp[3] +  vec14 * tmp[4] +  vec05 * tmp[5] +  vec16 * tmp[6] + 
    0;
     tmp =
        gradVecs +
        7 * hash7( posf0 + 0,  posf1 + 1,  posf2 + 1,  posf3 + 0,  posf4 + 1,  posf5 + 1,  posf6 + 0, 
                              i, numGradVecs);
    float result0110110 =  vec00 * tmp[0] +  vec11 * tmp[1] +  vec12 * tmp[2] +  vec03 * tmp[3] +  vec14 * tmp[4] +  vec15 * tmp[5] +  vec06 * tmp[6] + 
    0;
     tmp =
        gradVecs +
        7 * hash7( posf0 + 0,  posf1 + 1,  posf2 + 1,  posf3 + 0,  posf4 + 1,  posf5 + 1,  posf6 + 1, 
                              i, numGradVecs);
    float result0110111 =  vec00 * tmp[0] +  vec11 * tmp[1] +  vec12 * tmp[2] +  vec03 * tmp[3] +  vec14 * tmp[4] +  vec15 * tmp[5] +  vec16 * tmp[6] + 
    0;
     tmp =
        gradVecs +
        7 * hash7( posf0 + 0,  posf1 + 1,  posf2 + 1,  posf3 + 1,  posf4 + 0,  posf5 + 0,  posf6 + 0, 
                              i, numGradVecs);
    float result0111000 =  vec00 * tmp[0] +  vec11 * tmp[1] +  vec12 * tmp[2] +  vec13 * tmp[3] +  vec04 * tmp[4] +  vec05 * tmp[5] +  vec06 * tmp[6] + 
    0;
     tmp =
        gradVecs +
        7 * hash7( posf0 + 0,  posf1 + 1,  posf2 + 1,  posf3 + 1,  posf4 + 0,  posf5 + 0,  posf6 + 1, 
                              i, numGradVecs);
    float result0111001 =  vec00 * tmp[0] +  vec11 * tmp[1] +  vec12 * tmp[2] +  vec13 * tmp[3] +  vec04 * tmp[4] +  vec05 * tmp[5] +  vec16 * tmp[6] + 
    0;
     tmp =
        gradVecs +
        7 * hash7( posf0 + 0,  posf1 + 1,  posf2 + 1,  posf3 + 1,  posf4 + 0,  posf5 + 1,  posf6 + 0, 
                              i, numGradVecs);
    float result0111010 =  vec00 * tmp[0] +  vec11 * tmp[1] +  vec12 * tmp[2] +  vec13 * tmp[3] +  vec04 * tmp[4] +  vec15 * tmp[5] +  vec06 * tmp[6] + 
    0;
     tmp =
        gradVecs +
        7 * hash7( posf0 + 0,  posf1 + 1,  posf2 + 1,  posf3 + 1,  posf4 + 0,  posf5 + 1,  posf6 + 1, 
                              i, numGradVecs);
    float result0111011 =  vec00 * tmp[0] +  vec11 * tmp[1] +  vec12 * tmp[2] +  vec13 * tmp[3] +  vec04 * tmp[4] +  vec15 * tmp[5] +  vec16 * tmp[6] + 
    0;
     tmp =
        gradVecs +
        7 * hash7( posf0 + 0,  posf1 + 1,  posf2 + 1,  posf3 + 1,  posf4 + 1,  posf5 + 0,  posf6 + 0, 
                              i, numGradVecs);
    float result0111100 =  vec00 * tmp[0] +  vec11 * tmp[1] +  vec12 * tmp[2] +  vec13 * tmp[3] +  vec14 * tmp[4] +  vec05 * tmp[5] +  vec06 * tmp[6] + 
    0;
     tmp =
        gradVecs +
        7 * hash7( posf0 + 0,  posf1 + 1,  posf2 + 1,  posf3 + 1,  posf4 + 1,  posf5 + 0,  posf6 + 1, 
                              i, numGradVecs);
    float result0111101 =  vec00 * tmp[0] +  vec11 * tmp[1] +  vec12 * tmp[2] +  vec13 * tmp[3] +  vec14 * tmp[4] +  vec05 * tmp[5] +  vec16 * tmp[6] + 
    0;
     tmp =
        gradVecs +
        7 * hash7( posf0 + 0,  posf1 + 1,  posf2 + 1,  posf3 + 1,  posf4 + 1,  posf5 + 1,  posf6 + 0, 
                              i, numGradVecs);
    float result0111110 =  vec00 * tmp[0] +  vec11 * tmp[1] +  vec12 * tmp[2] +  vec13 * tmp[3] +  vec14 * tmp[4] +  vec15 * tmp[5] +  vec06 * tmp[6] + 
    0;
     tmp =
        gradVecs +
        7 * hash7( posf0 + 0,  posf1 + 1,  posf2 + 1,  posf3 + 1,  posf4 + 1,  posf5 + 1,  posf6 + 1, 
                              i, numGradVecs);
    float result0111111 =  vec00 * tmp[0] +  vec11 * tmp[1] +  vec12 * tmp[2] +  vec13 * tmp[3] +  vec14 * tmp[4] +  vec15 * tmp[5] +  vec16 * tmp[6] + 
    0;
     tmp =
        gradVecs +
        7 * hash7( posf0 + 1,  posf1 + 0,  posf2 + 0,  posf3 + 0,  posf4 + 0,  posf5 + 0,  posf6 + 0, 
                              i, numGradVecs);
    float result1000000 =  vec10 * tmp[0] +  vec01 * tmp[1] +  vec02 * tmp[2] +  vec03 * tmp[3] +  vec04 * tmp[4] +  vec05 * tmp[5] +  vec06 * tmp[6] + 
    0;
     tmp =
        gradVecs +
        7 * hash7( posf0 + 1,  posf1 + 0,  posf2 + 0,  posf3 + 0,  posf4 + 0,  posf5 + 0,  posf6 + 1, 
                              i, numGradVecs);
    float result1000001 =  vec10 * tmp[0] +  vec01 * tmp[1] +  vec02 * tmp[2] +  vec03 * tmp[3] +  vec04 * tmp[4] +  vec05 * tmp[5] +  vec16 * tmp[6] + 
    0;
     tmp =
        gradVecs +
        7 * hash7( posf0 + 1,  posf1 + 0,  posf2 + 0,  posf3 + 0,  posf4 + 0,  posf5 + 1,  posf6 + 0, 
                              i, numGradVecs);
    float result1000010 =  vec10 * tmp[0] +  vec01 * tmp[1] +  vec02 * tmp[2] +  vec03 * tmp[3] +  vec04 * tmp[4] +  vec15 * tmp[5] +  vec06 * tmp[6] + 
    0;
     tmp =
        gradVecs +
        7 * hash7( posf0 + 1,  posf1 + 0,  posf2 + 0,  posf3 + 0,  posf4 + 0,  posf5 + 1,  posf6 + 1, 
                              i, numGradVecs);
    float result1000011 =  vec10 * tmp[0] +  vec01 * tmp[1] +  vec02 * tmp[2] +  vec03 * tmp[3] +  vec04 * tmp[4] +  vec15 * tmp[5] +  vec16 * tmp[6] + 
    0;
     tmp =
        gradVecs +
        7 * hash7( posf0 + 1,  posf1 + 0,  posf2 + 0,  posf3 + 0,  posf4 + 1,  posf5 + 0,  posf6 + 0, 
                              i, numGradVecs);
    float result1000100 =  vec10 * tmp[0] +  vec01 * tmp[1] +  vec02 * tmp[2] +  vec03 * tmp[3] +  vec14 * tmp[4] +  vec05 * tmp[5] +  vec06 * tmp[6] + 
    0;
     tmp =
        gradVecs +
        7 * hash7( posf0 + 1,  posf1 + 0,  posf2 + 0,  posf3 + 0,  posf4 + 1,  posf5 + 0,  posf6 + 1, 
                              i, numGradVecs);
    float result1000101 =  vec10 * tmp[0] +  vec01 * tmp[1] +  vec02 * tmp[2] +  vec03 * tmp[3] +  vec14 * tmp[4] +  vec05 * tmp[5] +  vec16 * tmp[6] + 
    0;
     tmp =
        gradVecs +
        7 * hash7( posf0 + 1,  posf1 + 0,  posf2 + 0,  posf3 + 0,  posf4 + 1,  posf5 + 1,  posf6 + 0, 
                              i, numGradVecs);
    float result1000110 =  vec10 * tmp[0] +  vec01 * tmp[1] +  vec02 * tmp[2] +  vec03 * tmp[3] +  vec14 * tmp[4] +  vec15 * tmp[5] +  vec06 * tmp[6] + 
    0;
     tmp =
        gradVecs +
        7 * hash7( posf0 + 1,  posf1 + 0,  posf2 + 0,  posf3 + 0,  posf4 + 1,  posf5 + 1,  posf6 + 1, 
                              i, numGradVecs);
    float result1000111 =  vec10 * tmp[0] +  vec01 * tmp[1] +  vec02 * tmp[2] +  vec03 * tmp[3] +  vec14 * tmp[4] +  vec15 * tmp[5] +  vec16 * tmp[6] + 
    0;
     tmp =
        gradVecs +
        7 * hash7( posf0 + 1,  posf1 + 0,  posf2 + 0,  posf3 + 1,  posf4 + 0,  posf5 + 0,  posf6 + 0, 
                              i, numGradVecs);
    float result1001000 =  vec10 * tmp[0] +  vec01 * tmp[1] +  vec02 * tmp[2] +  vec13 * tmp[3] +  vec04 * tmp[4] +  vec05 * tmp[5] +  vec06 * tmp[6] + 
    0;
     tmp =
        gradVecs +
        7 * hash7( posf0 + 1,  posf1 + 0,  posf2 + 0,  posf3 + 1,  posf4 + 0,  posf5 + 0,  posf6 + 1, 
                              i, numGradVecs);
    float result1001001 =  vec10 * tmp[0] +  vec01 * tmp[1] +  vec02 * tmp[2] +  vec13 * tmp[3] +  vec04 * tmp[4] +  vec05 * tmp[5] +  vec16 * tmp[6] + 
    0;
     tmp =
        gradVecs +
        7 * hash7( posf0 + 1,  posf1 + 0,  posf2 + 0,  posf3 + 1,  posf4 + 0,  posf5 + 1,  posf6 + 0, 
                              i, numGradVecs);
    float result1001010 =  vec10 * tmp[0] +  vec01 * tmp[1] +  vec02 * tmp[2] +  vec13 * tmp[3] +  vec04 * tmp[4] +  vec15 * tmp[5] +  vec06 * tmp[6] + 
    0;
     tmp =
        gradVecs +
        7 * hash7( posf0 + 1,  posf1 + 0,  posf2 + 0,  posf3 + 1,  posf4 + 0,  posf5 + 1,  posf6 + 1, 
                              i, numGradVecs);
    float result1001011 =  vec10 * tmp[0] +  vec01 * tmp[1] +  vec02 * tmp[2] +  vec13 * tmp[3] +  vec04 * tmp[4] +  vec15 * tmp[5] +  vec16 * tmp[6] + 
    0;
     tmp =
        gradVecs +
        7 * hash7( posf0 + 1,  posf1 + 0,  posf2 + 0,  posf3 + 1,  posf4 + 1,  posf5 + 0,  posf6 + 0, 
                              i, numGradVecs);
    float result1001100 =  vec10 * tmp[0] +  vec01 * tmp[1] +  vec02 * tmp[2] +  vec13 * tmp[3] +  vec14 * tmp[4] +  vec05 * tmp[5] +  vec06 * tmp[6] + 
    0;
     tmp =
        gradVecs +
        7 * hash7( posf0 + 1,  posf1 + 0,  posf2 + 0,  posf3 + 1,  posf4 + 1,  posf5 + 0,  posf6 + 1, 
                              i, numGradVecs);
    float result1001101 =  vec10 * tmp[0] +  vec01 * tmp[1] +  vec02 * tmp[2] +  vec13 * tmp[3] +  vec14 * tmp[4] +  vec05 * tmp[5] +  vec16 * tmp[6] + 
    0;
     tmp =
        gradVecs +
        7 * hash7( posf0 + 1,  posf1 + 0,  posf2 + 0,  posf3 + 1,  posf4 + 1,  posf5 + 1,  posf6 + 0, 
                              i, numGradVecs);
    float result1001110 =  vec10 * tmp[0] +  vec01 * tmp[1] +  vec02 * tmp[2] +  vec13 * tmp[3] +  vec14 * tmp[4] +  vec15 * tmp[5] +  vec06 * tmp[6] + 
    0;
     tmp =
        gradVecs +
        7 * hash7( posf0 + 1,  posf1 + 0,  posf2 + 0,  posf3 + 1,  posf4 + 1,  posf5 + 1,  posf6 + 1, 
                              i, numGradVecs);
    float result1001111 =  vec10 * tmp[0] +  vec01 * tmp[1] +  vec02 * tmp[2] +  vec13 * tmp[3] +  vec14 * tmp[4] +  vec15 * tmp[5] +  vec16 * tmp[6] + 
    0;
     tmp =
        gradVecs +
        7 * hash7( posf0 + 1,  posf1 + 0,  posf2 + 1,  posf3 + 0,  posf4 + 0,  posf5 + 0,  posf6 + 0, 
                              i, numGradVecs);
    float result1010000 =  vec10 * tmp[0] +  vec01 * tmp[1] +  vec12 * tmp[2] +  vec03 * tmp[3] +  vec04 * tmp[4] +  vec05 * tmp[5] +  vec06 * tmp[6] + 
    0;
     tmp =
        gradVecs +
        7 * hash7( posf0 + 1,  posf1 + 0,  posf2 + 1,  posf3 + 0,  posf4 + 0,  posf5 + 0,  posf6 + 1, 
                              i, numGradVecs);
    float result1010001 =  vec10 * tmp[0] +  vec01 * tmp[1] +  vec12 * tmp[2] +  vec03 * tmp[3] +  vec04 * tmp[4] +  vec05 * tmp[5] +  vec16 * tmp[6] + 
    0;
     tmp =
        gradVecs +
        7 * hash7( posf0 + 1,  posf1 + 0,  posf2 + 1,  posf3 + 0,  posf4 + 0,  posf5 + 1,  posf6 + 0, 
                              i, numGradVecs);
    float result1010010 =  vec10 * tmp[0] +  vec01 * tmp[1] +  vec12 * tmp[2] +  vec03 * tmp[3] +  vec04 * tmp[4] +  vec15 * tmp[5] +  vec06 * tmp[6] + 
    0;
     tmp =
        gradVecs +
        7 * hash7( posf0 + 1,  posf1 + 0,  posf2 + 1,  posf3 + 0,  posf4 + 0,  posf5 + 1,  posf6 + 1, 
                              i, numGradVecs);
    float result1010011 =  vec10 * tmp[0] +  vec01 * tmp[1] +  vec12 * tmp[2] +  vec03 * tmp[3] +  vec04 * tmp[4] +  vec15 * tmp[5] +  vec16 * tmp[6] + 
    0;
     tmp =
        gradVecs +
        7 * hash7( posf0 + 1,  posf1 + 0,  posf2 + 1,  posf3 + 0,  posf4 + 1,  posf5 + 0,  posf6 + 0, 
                              i, numGradVecs);
    float result1010100 =  vec10 * tmp[0] +  vec01 * tmp[1] +  vec12 * tmp[2] +  vec03 * tmp[3] +  vec14 * tmp[4] +  vec05 * tmp[5] +  vec06 * tmp[6] + 
    0;
     tmp =
        gradVecs +
        7 * hash7( posf0 + 1,  posf1 + 0,  posf2 + 1,  posf3 + 0,  posf4 + 1,  posf5 + 0,  posf6 + 1, 
                              i, numGradVecs);
    float result1010101 =  vec10 * tmp[0] +  vec01 * tmp[1] +  vec12 * tmp[2] +  vec03 * tmp[3] +  vec14 * tmp[4] +  vec05 * tmp[5] +  vec16 * tmp[6] + 
    0;
     tmp =
        gradVecs +
        7 * hash7( posf0 + 1,  posf1 + 0,  posf2 + 1,  posf3 + 0,  posf4 + 1,  posf5 + 1,  posf6 + 0, 
                              i, numGradVecs);
    float result1010110 =  vec10 * tmp[0] +  vec01 * tmp[1] +  vec12 * tmp[2] +  vec03 * tmp[3] +  vec14 * tmp[4] +  vec15 * tmp[5] +  vec06 * tmp[6] + 
    0;
     tmp =
        gradVecs +
        7 * hash7( posf0 + 1,  posf1 + 0,  posf2 + 1,  posf3 + 0,  posf4 + 1,  posf5 + 1,  posf6 + 1, 
                              i, numGradVecs);
    float result1010111 =  vec10 * tmp[0] +  vec01 * tmp[1] +  vec12 * tmp[2] +  vec03 * tmp[3] +  vec14 * tmp[4] +  vec15 * tmp[5] +  vec16 * tmp[6] + 
    0;
     tmp =
        gradVecs +
        7 * hash7( posf0 + 1,  posf1 + 0,  posf2 + 1,  posf3 + 1,  posf4 + 0,  posf5 + 0,  posf6 + 0, 
                              i, numGradVecs);
    float result1011000 =  vec10 * tmp[0] +  vec01 * tmp[1] +  vec12 * tmp[2] +  vec13 * tmp[3] +  vec04 * tmp[4] +  vec05 * tmp[5] +  vec06 * tmp[6] + 
    0;
     tmp =
        gradVecs +
        7 * hash7( posf0 + 1,  posf1 + 0,  posf2 + 1,  posf3 + 1,  posf4 + 0,  posf5 + 0,  posf6 + 1, 
                              i, numGradVecs);
    float result1011001 =  vec10 * tmp[0] +  vec01 * tmp[1] +  vec12 * tmp[2] +  vec13 * tmp[3] +  vec04 * tmp[4] +  vec05 * tmp[5] +  vec16 * tmp[6] + 
    0;
     tmp =
        gradVecs +
        7 * hash7( posf0 + 1,  posf1 + 0,  posf2 + 1,  posf3 + 1,  posf4 + 0,  posf5 + 1,  posf6 + 0, 
                              i, numGradVecs);
    float result1011010 =  vec10 * tmp[0] +  vec01 * tmp[1] +  vec12 * tmp[2] +  vec13 * tmp[3] +  vec04 * tmp[4] +  vec15 * tmp[5] +  vec06 * tmp[6] + 
    0;
     tmp =
        gradVecs +
        7 * hash7( posf0 + 1,  posf1 + 0,  posf2 + 1,  posf3 + 1,  posf4 + 0,  posf5 + 1,  posf6 + 1, 
                              i, numGradVecs);
    float result1011011 =  vec10 * tmp[0] +  vec01 * tmp[1] +  vec12 * tmp[2] +  vec13 * tmp[3] +  vec04 * tmp[4] +  vec15 * tmp[5] +  vec16 * tmp[6] + 
    0;
     tmp =
        gradVecs +
        7 * hash7( posf0 + 1,  posf1 + 0,  posf2 + 1,  posf3 + 1,  posf4 + 1,  posf5 + 0,  posf6 + 0, 
                              i, numGradVecs);
    float result1011100 =  vec10 * tmp[0] +  vec01 * tmp[1] +  vec12 * tmp[2] +  vec13 * tmp[3] +  vec14 * tmp[4] +  vec05 * tmp[5] +  vec06 * tmp[6] + 
    0;
     tmp =
        gradVecs +
        7 * hash7( posf0 + 1,  posf1 + 0,  posf2 + 1,  posf3 + 1,  posf4 + 1,  posf5 + 0,  posf6 + 1, 
                              i, numGradVecs);
    float result1011101 =  vec10 * tmp[0] +  vec01 * tmp[1] +  vec12 * tmp[2] +  vec13 * tmp[3] +  vec14 * tmp[4] +  vec05 * tmp[5] +  vec16 * tmp[6] + 
    0;
     tmp =
        gradVecs +
        7 * hash7( posf0 + 1,  posf1 + 0,  posf2 + 1,  posf3 + 1,  posf4 + 1,  posf5 + 1,  posf6 + 0, 
                              i, numGradVecs);
    float result1011110 =  vec10 * tmp[0] +  vec01 * tmp[1] +  vec12 * tmp[2] +  vec13 * tmp[3] +  vec14 * tmp[4] +  vec15 * tmp[5] +  vec06 * tmp[6] + 
    0;
     tmp =
        gradVecs +
        7 * hash7( posf0 + 1,  posf1 + 0,  posf2 + 1,  posf3 + 1,  posf4 + 1,  posf5 + 1,  posf6 + 1, 
                              i, numGradVecs);
    float result1011111 =  vec10 * tmp[0] +  vec01 * tmp[1] +  vec12 * tmp[2] +  vec13 * tmp[3] +  vec14 * tmp[4] +  vec15 * tmp[5] +  vec16 * tmp[6] + 
    0;
     tmp =
        gradVecs +
        7 * hash7( posf0 + 1,  posf1 + 1,  posf2 + 0,  posf3 + 0,  posf4 + 0,  posf5 + 0,  posf6 + 0, 
                              i, numGradVecs);
    float result1100000 =  vec10 * tmp[0] +  vec11 * tmp[1] +  vec02 * tmp[2] +  vec03 * tmp[3] +  vec04 * tmp[4] +  vec05 * tmp[5] +  vec06 * tmp[6] + 
    0;
     tmp =
        gradVecs +
        7 * hash7( posf0 + 1,  posf1 + 1,  posf2 + 0,  posf3 + 0,  posf4 + 0,  posf5 + 0,  posf6 + 1, 
                              i, numGradVecs);
    float result1100001 =  vec10 * tmp[0] +  vec11 * tmp[1] +  vec02 * tmp[2] +  vec03 * tmp[3] +  vec04 * tmp[4] +  vec05 * tmp[5] +  vec16 * tmp[6] + 
    0;
     tmp =
        gradVecs +
        7 * hash7( posf0 + 1,  posf1 + 1,  posf2 + 0,  posf3 + 0,  posf4 + 0,  posf5 + 1,  posf6 + 0, 
                              i, numGradVecs);
    float result1100010 =  vec10 * tmp[0] +  vec11 * tmp[1] +  vec02 * tmp[2] +  vec03 * tmp[3] +  vec04 * tmp[4] +  vec15 * tmp[5] +  vec06 * tmp[6] + 
    0;
     tmp =
        gradVecs +
        7 * hash7( posf0 + 1,  posf1 + 1,  posf2 + 0,  posf3 + 0,  posf4 + 0,  posf5 + 1,  posf6 + 1, 
                              i, numGradVecs);
    float result1100011 =  vec10 * tmp[0] +  vec11 * tmp[1] +  vec02 * tmp[2] +  vec03 * tmp[3] +  vec04 * tmp[4] +  vec15 * tmp[5] +  vec16 * tmp[6] + 
    0;
     tmp =
        gradVecs +
        7 * hash7( posf0 + 1,  posf1 + 1,  posf2 + 0,  posf3 + 0,  posf4 + 1,  posf5 + 0,  posf6 + 0, 
                              i, numGradVecs);
    float result1100100 =  vec10 * tmp[0] +  vec11 * tmp[1] +  vec02 * tmp[2] +  vec03 * tmp[3] +  vec14 * tmp[4] +  vec05 * tmp[5] +  vec06 * tmp[6] + 
    0;
     tmp =
        gradVecs +
        7 * hash7( posf0 + 1,  posf1 + 1,  posf2 + 0,  posf3 + 0,  posf4 + 1,  posf5 + 0,  posf6 + 1, 
                              i, numGradVecs);
    float result1100101 =  vec10 * tmp[0] +  vec11 * tmp[1] +  vec02 * tmp[2] +  vec03 * tmp[3] +  vec14 * tmp[4] +  vec05 * tmp[5] +  vec16 * tmp[6] + 
    0;
     tmp =
        gradVecs +
        7 * hash7( posf0 + 1,  posf1 + 1,  posf2 + 0,  posf3 + 0,  posf4 + 1,  posf5 + 1,  posf6 + 0, 
                              i, numGradVecs);
    float result1100110 =  vec10 * tmp[0] +  vec11 * tmp[1] +  vec02 * tmp[2] +  vec03 * tmp[3] +  vec14 * tmp[4] +  vec15 * tmp[5] +  vec06 * tmp[6] + 
    0;
     tmp =
        gradVecs +
        7 * hash7( posf0 + 1,  posf1 + 1,  posf2 + 0,  posf3 + 0,  posf4 + 1,  posf5 + 1,  posf6 + 1, 
                              i, numGradVecs);
    float result1100111 =  vec10 * tmp[0] +  vec11 * tmp[1] +  vec02 * tmp[2] +  vec03 * tmp[3] +  vec14 * tmp[4] +  vec15 * tmp[5] +  vec16 * tmp[6] + 
    0;
     tmp =
        gradVecs +
        7 * hash7( posf0 + 1,  posf1 + 1,  posf2 + 0,  posf3 + 1,  posf4 + 0,  posf5 + 0,  posf6 + 0, 
                              i, numGradVecs);
    float result1101000 =  vec10 * tmp[0] +  vec11 * tmp[1] +  vec02 * tmp[2] +  vec13 * tmp[3] +  vec04 * tmp[4] +  vec05 * tmp[5] +  vec06 * tmp[6] + 
    0;
     tmp =
        gradVecs +
        7 * hash7( posf0 + 1,  posf1 + 1,  posf2 + 0,  posf3 + 1,  posf4 + 0,  posf5 + 0,  posf6 + 1, 
                              i, numGradVecs);
    float result1101001 =  vec10 * tmp[0] +  vec11 * tmp[1] +  vec02 * tmp[2] +  vec13 * tmp[3] +  vec04 * tmp[4] +  vec05 * tmp[5] +  vec16 * tmp[6] + 
    0;
     tmp =
        gradVecs +
        7 * hash7( posf0 + 1,  posf1 + 1,  posf2 + 0,  posf3 + 1,  posf4 + 0,  posf5 + 1,  posf6 + 0, 
                              i, numGradVecs);
    float result1101010 =  vec10 * tmp[0] +  vec11 * tmp[1] +  vec02 * tmp[2] +  vec13 * tmp[3] +  vec04 * tmp[4] +  vec15 * tmp[5] +  vec06 * tmp[6] + 
    0;
     tmp =
        gradVecs +
        7 * hash7( posf0 + 1,  posf1 + 1,  posf2 + 0,  posf3 + 1,  posf4 + 0,  posf5 + 1,  posf6 + 1, 
                              i, numGradVecs);
    float result1101011 =  vec10 * tmp[0] +  vec11 * tmp[1] +  vec02 * tmp[2] +  vec13 * tmp[3] +  vec04 * tmp[4] +  vec15 * tmp[5] +  vec16 * tmp[6] + 
    0;
     tmp =
        gradVecs +
        7 * hash7( posf0 + 1,  posf1 + 1,  posf2 + 0,  posf3 + 1,  posf4 + 1,  posf5 + 0,  posf6 + 0, 
                              i, numGradVecs);
    float result1101100 =  vec10 * tmp[0] +  vec11 * tmp[1] +  vec02 * tmp[2] +  vec13 * tmp[3] +  vec14 * tmp[4] +  vec05 * tmp[5] +  vec06 * tmp[6] + 
    0;
     tmp =
        gradVecs +
        7 * hash7( posf0 + 1,  posf1 + 1,  posf2 + 0,  posf3 + 1,  posf4 + 1,  posf5 + 0,  posf6 + 1, 
                              i, numGradVecs);
    float result1101101 =  vec10 * tmp[0] +  vec11 * tmp[1] +  vec02 * tmp[2] +  vec13 * tmp[3] +  vec14 * tmp[4] +  vec05 * tmp[5] +  vec16 * tmp[6] + 
    0;
     tmp =
        gradVecs +
        7 * hash7( posf0 + 1,  posf1 + 1,  posf2 + 0,  posf3 + 1,  posf4 + 1,  posf5 + 1,  posf6 + 0, 
                              i, numGradVecs);
    float result1101110 =  vec10 * tmp[0] +  vec11 * tmp[1] +  vec02 * tmp[2] +  vec13 * tmp[3] +  vec14 * tmp[4] +  vec15 * tmp[5] +  vec06 * tmp[6] + 
    0;
     tmp =
        gradVecs +
        7 * hash7( posf0 + 1,  posf1 + 1,  posf2 + 0,  posf3 + 1,  posf4 + 1,  posf5 + 1,  posf6 + 1, 
                              i, numGradVecs);
    float result1101111 =  vec10 * tmp[0] +  vec11 * tmp[1] +  vec02 * tmp[2] +  vec13 * tmp[3] +  vec14 * tmp[4] +  vec15 * tmp[5] +  vec16 * tmp[6] + 
    0;
     tmp =
        gradVecs +
        7 * hash7( posf0 + 1,  posf1 + 1,  posf2 + 1,  posf3 + 0,  posf4 + 0,  posf5 + 0,  posf6 + 0, 
                              i, numGradVecs);
    float result1110000 =  vec10 * tmp[0] +  vec11 * tmp[1] +  vec12 * tmp[2] +  vec03 * tmp[3] +  vec04 * tmp[4] +  vec05 * tmp[5] +  vec06 * tmp[6] + 
    0;
     tmp =
        gradVecs +
        7 * hash7( posf0 + 1,  posf1 + 1,  posf2 + 1,  posf3 + 0,  posf4 + 0,  posf5 + 0,  posf6 + 1, 
                              i, numGradVecs);
    float result1110001 =  vec10 * tmp[0] +  vec11 * tmp[1] +  vec12 * tmp[2] +  vec03 * tmp[3] +  vec04 * tmp[4] +  vec05 * tmp[5] +  vec16 * tmp[6] + 
    0;
     tmp =
        gradVecs +
        7 * hash7( posf0 + 1,  posf1 + 1,  posf2 + 1,  posf3 + 0,  posf4 + 0,  posf5 + 1,  posf6 + 0, 
                              i, numGradVecs);
    float result1110010 =  vec10 * tmp[0] +  vec11 * tmp[1] +  vec12 * tmp[2] +  vec03 * tmp[3] +  vec04 * tmp[4] +  vec15 * tmp[5] +  vec06 * tmp[6] + 
    0;
     tmp =
        gradVecs +
        7 * hash7( posf0 + 1,  posf1 + 1,  posf2 + 1,  posf3 + 0,  posf4 + 0,  posf5 + 1,  posf6 + 1, 
                              i, numGradVecs);
    float result1110011 =  vec10 * tmp[0] +  vec11 * tmp[1] +  vec12 * tmp[2] +  vec03 * tmp[3] +  vec04 * tmp[4] +  vec15 * tmp[5] +  vec16 * tmp[6] + 
    0;
     tmp =
        gradVecs +
        7 * hash7( posf0 + 1,  posf1 + 1,  posf2 + 1,  posf3 + 0,  posf4 + 1,  posf5 + 0,  posf6 + 0, 
                              i, numGradVecs);
    float result1110100 =  vec10 * tmp[0] +  vec11 * tmp[1] +  vec12 * tmp[2] +  vec03 * tmp[3] +  vec14 * tmp[4] +  vec05 * tmp[5] +  vec06 * tmp[6] + 
    0;
     tmp =
        gradVecs +
        7 * hash7( posf0 + 1,  posf1 + 1,  posf2 + 1,  posf3 + 0,  posf4 + 1,  posf5 + 0,  posf6 + 1, 
                              i, numGradVecs);
    float result1110101 =  vec10 * tmp[0] +  vec11 * tmp[1] +  vec12 * tmp[2] +  vec03 * tmp[3] +  vec14 * tmp[4] +  vec05 * tmp[5] +  vec16 * tmp[6] + 
    0;
     tmp =
        gradVecs +
        7 * hash7( posf0 + 1,  posf1 + 1,  posf2 + 1,  posf3 + 0,  posf4 + 1,  posf5 + 1,  posf6 + 0, 
                              i, numGradVecs);
    float result1110110 =  vec10 * tmp[0] +  vec11 * tmp[1] +  vec12 * tmp[2] +  vec03 * tmp[3] +  vec14 * tmp[4] +  vec15 * tmp[5] +  vec06 * tmp[6] + 
    0;
     tmp =
        gradVecs +
        7 * hash7( posf0 + 1,  posf1 + 1,  posf2 + 1,  posf3 + 0,  posf4 + 1,  posf5 + 1,  posf6 + 1, 
                              i, numGradVecs);
    float result1110111 =  vec10 * tmp[0] +  vec11 * tmp[1] +  vec12 * tmp[2] +  vec03 * tmp[3] +  vec14 * tmp[4] +  vec15 * tmp[5] +  vec16 * tmp[6] + 
    0;
     tmp =
        gradVecs +
        7 * hash7( posf0 + 1,  posf1 + 1,  posf2 + 1,  posf3 + 1,  posf4 + 0,  posf5 + 0,  posf6 + 0, 
                              i, numGradVecs);
    float result1111000 =  vec10 * tmp[0] +  vec11 * tmp[1] +  vec12 * tmp[2] +  vec13 * tmp[3] +  vec04 * tmp[4] +  vec05 * tmp[5] +  vec06 * tmp[6] + 
    0;
     tmp =
        gradVecs +
        7 * hash7( posf0 + 1,  posf1 + 1,  posf2 + 1,  posf3 + 1,  posf4 + 0,  posf5 + 0,  posf6 + 1, 
                              i, numGradVecs);
    float result1111001 =  vec10 * tmp[0] +  vec11 * tmp[1] +  vec12 * tmp[2] +  vec13 * tmp[3] +  vec04 * tmp[4] +  vec05 * tmp[5] +  vec16 * tmp[6] + 
    0;
     tmp =
        gradVecs +
        7 * hash7( posf0 + 1,  posf1 + 1,  posf2 + 1,  posf3 + 1,  posf4 + 0,  posf5 + 1,  posf6 + 0, 
                              i, numGradVecs);
    float result1111010 =  vec10 * tmp[0] +  vec11 * tmp[1] +  vec12 * tmp[2] +  vec13 * tmp[3] +  vec04 * tmp[4] +  vec15 * tmp[5] +  vec06 * tmp[6] + 
    0;
     tmp =
        gradVecs +
        7 * hash7( posf0 + 1,  posf1 + 1,  posf2 + 1,  posf3 + 1,  posf4 + 0,  posf5 + 1,  posf6 + 1, 
                              i, numGradVecs);
    float result1111011 =  vec10 * tmp[0] +  vec11 * tmp[1] +  vec12 * tmp[2] +  vec13 * tmp[3] +  vec04 * tmp[4] +  vec15 * tmp[5] +  vec16 * tmp[6] + 
    0;
     tmp =
        gradVecs +
        7 * hash7( posf0 + 1,  posf1 + 1,  posf2 + 1,  posf3 + 1,  posf4 + 1,  posf5 + 0,  posf6 + 0, 
                              i, numGradVecs);
    float result1111100 =  vec10 * tmp[0] +  vec11 * tmp[1] +  vec12 * tmp[2] +  vec13 * tmp[3] +  vec14 * tmp[4] +  vec05 * tmp[5] +  vec06 * tmp[6] + 
    0;
     tmp =
        gradVecs +
        7 * hash7( posf0 + 1,  posf1 + 1,  posf2 + 1,  posf3 + 1,  posf4 + 1,  posf5 + 0,  posf6 + 1, 
                              i, numGradVecs);
    float result1111101 =  vec10 * tmp[0] +  vec11 * tmp[1] +  vec12 * tmp[2] +  vec13 * tmp[3] +  vec14 * tmp[4] +  vec05 * tmp[5] +  vec16 * tmp[6] + 
    0;
     tmp =
        gradVecs +
        7 * hash7( posf0 + 1,  posf1 + 1,  posf2 + 1,  posf3 + 1,  posf4 + 1,  posf5 + 1,  posf6 + 0, 
                              i, numGradVecs);
    float result1111110 =  vec10 * tmp[0] +  vec11 * tmp[1] +  vec12 * tmp[2] +  vec13 * tmp[3] +  vec14 * tmp[4] +  vec15 * tmp[5] +  vec06 * tmp[6] + 
    0;
     tmp =
        gradVecs +
        7 * hash7( posf0 + 1,  posf1 + 1,  posf2 + 1,  posf3 + 1,  posf4 + 1,  posf5 + 1,  posf6 + 1, 
                              i, numGradVecs);
    float result1111111 =  vec10 * tmp[0] +  vec11 * tmp[1] +  vec12 * tmp[2] +  vec13 * tmp[3] +  vec14 * tmp[4] +  vec15 * tmp[5] +  vec16 * tmp[6] + 
    0;
    
      float result000000 =
        result0000000 + lerp0 * (result1000000 - result0000000);
     float result000001 =
        result0000001 + lerp0 * (result1000001 - result0000001);
     float result000010 =
        result0000010 + lerp0 * (result1000010 - result0000010);
     float result000011 =
        result0000011 + lerp0 * (result1000011 - result0000011);
     float result000100 =
        result0000100 + lerp0 * (result1000100 - result0000100);
     float result000101 =
        result0000101 + lerp0 * (result1000101 - result0000101);
     float result000110 =
        result0000110 + lerp0 * (result1000110 - result0000110);
     float result000111 =
        result0000111 + lerp0 * (result1000111 - result0000111);
     float result001000 =
        result0001000 + lerp0 * (result1001000 - result0001000);
     float result001001 =
        result0001001 + lerp0 * (result1001001 - result0001001);
     float result001010 =
        result0001010 + lerp0 * (result1001010 - result0001010);
     float result001011 =
        result0001011 + lerp0 * (result1001011 - result0001011);
     float result001100 =
        result0001100 + lerp0 * (result1001100 - result0001100);
     float result001101 =
        result0001101 + lerp0 * (result1001101 - result0001101);
     float result001110 =
        result0001110 + lerp0 * (result1001110 - result0001110);
     float result001111 =
        result0001111 + lerp0 * (result1001111 - result0001111);
     float result010000 =
        result0010000 + lerp0 * (result1010000 - result0010000);
     float result010001 =
        result0010001 + lerp0 * (result1010001 - result0010001);
     float result010010 =
        result0010010 + lerp0 * (result1010010 - result0010010);
     float result010011 =
        result0010011 + lerp0 * (result1010011 - result0010011);
     float result010100 =
        result0010100 + lerp0 * (result1010100 - result0010100);
     float result010101 =
        result0010101 + lerp0 * (result1010101 - result0010101);
     float result010110 =
        result0010110 + lerp0 * (result1010110 - result0010110);
     float result010111 =
        result0010111 + lerp0 * (result1010111 - result0010111);
     float result011000 =
        result0011000 + lerp0 * (result1011000 - result0011000);
     float result011001 =
        result0011001 + lerp0 * (result1011001 - result0011001);
     float result011010 =
        result0011010 + lerp0 * (result1011010 - result0011010);
     float result011011 =
        result0011011 + lerp0 * (result1011011 - result0011011);
     float result011100 =
        result0011100 + lerp0 * (result1011100 - result0011100);
     float result011101 =
        result0011101 + lerp0 * (result1011101 - result0011101);
     float result011110 =
        result0011110 + lerp0 * (result1011110 - result0011110);
     float result011111 =
        result0011111 + lerp0 * (result1011111 - result0011111);
     float result100000 =
        result0100000 + lerp0 * (result1100000 - result0100000);
     float result100001 =
        result0100001 + lerp0 * (result1100001 - result0100001);
     float result100010 =
        result0100010 + lerp0 * (result1100010 - result0100010);
     float result100011 =
        result0100011 + lerp0 * (result1100011 - result0100011);
     float result100100 =
        result0100100 + lerp0 * (result1100100 - result0100100);
     float result100101 =
        result0100101 + lerp0 * (result1100101 - result0100101);
     float result100110 =
        result0100110 + lerp0 * (result1100110 - result0100110);
     float result100111 =
        result0100111 + lerp0 * (result1100111 - result0100111);
     float result101000 =
        result0101000 + lerp0 * (result1101000 - result0101000);
     float result101001 =
        result0101001 + lerp0 * (result1101001 - result0101001);
     float result101010 =
        result0101010 + lerp0 * (result1101010 - result0101010);
     float result101011 =
        result0101011 + lerp0 * (result1101011 - result0101011);
     float result101100 =
        result0101100 + lerp0 * (result1101100 - result0101100);
     float result101101 =
        result0101101 + lerp0 * (result1101101 - result0101101);
     float result101110 =
        result0101110 + lerp0 * (result1101110 - result0101110);
     float result101111 =
        result0101111 + lerp0 * (result1101111 - result0101111);
     float result110000 =
        result0110000 + lerp0 * (result1110000 - result0110000);
     float result110001 =
        result0110001 + lerp0 * (result1110001 - result0110001);
     float result110010 =
        result0110010 + lerp0 * (result1110010 - result0110010);
     float result110011 =
        result0110011 + lerp0 * (result1110011 - result0110011);
     float result110100 =
        result0110100 + lerp0 * (result1110100 - result0110100);
     float result110101 =
        result0110101 + lerp0 * (result1110101 - result0110101);
     float result110110 =
        result0110110 + lerp0 * (result1110110 - result0110110);
     float result110111 =
        result0110111 + lerp0 * (result1110111 - result0110111);
     float result111000 =
        result0111000 + lerp0 * (result1111000 - result0111000);
     float result111001 =
        result0111001 + lerp0 * (result1111001 - result0111001);
     float result111010 =
        result0111010 + lerp0 * (result1111010 - result0111010);
     float result111011 =
        result0111011 + lerp0 * (result1111011 - result0111011);
     float result111100 =
        result0111100 + lerp0 * (result1111100 - result0111100);
     float result111101 =
        result0111101 + lerp0 * (result1111101 - result0111101);
     float result111110 =
        result0111110 + lerp0 * (result1111110 - result0111110);
     float result111111 =
        result0111111 + lerp0 * (result1111111 - result0111111);
    
      float result00000 =
        result000000 + lerp1 * (result100000 - result000000);
     float result00001 =
        result000001 + lerp1 * (result100001 - result000001);
     float result00010 =
        result000010 + lerp1 * (result100010 - result000010);
     float result00011 =
        result000011 + lerp1 * (result100011 - result000011);
     float result00100 =
        result000100 + lerp1 * (result100100 - result000100);
     float result00101 =
        result000101 + lerp1 * (result100101 - result000101);
     float result00110 =
        result000110 + lerp1 * (result100110 - result000110);
     float result00111 =
        result000111 + lerp1 * (result100111 - result000111);
     float result01000 =
        result001000 + lerp1 * (result101000 - result001000);
     float result01001 =
        result001001 + lerp1 * (result101001 - result001001);
     float result01010 =
        result001010 + lerp1 * (result101010 - result001010);
     float result01011 =
        result001011 + lerp1 * (result101011 - result001011);
     float result01100 =
        result001100 + lerp1 * (result101100 - result001100);
     float result01101 =
        result001101 + lerp1 * (result101101 - result001101);
     float result01110 =
        result001110 + lerp1 * (result101110 - result001110);
     float result01111 =
        result001111 + lerp1 * (result101111 - result001111);
     float result10000 =
        result010000 + lerp1 * (result110000 - result010000);
     float result10001 =
        result010001 + lerp1 * (result110001 - result010001);
     float result10010 =
        result010010 + lerp1 * (result110010 - result010010);
     float result10011 =
        result010011 + lerp1 * (result110011 - result010011);
     float result10100 =
        result010100 + lerp1 * (result110100 - result010100);
     float result10101 =
        result010101 + lerp1 * (result110101 - result010101);
     float result10110 =
        result010110 + lerp1 * (result110110 - result010110);
     float result10111 =
        result010111 + lerp1 * (result110111 - result010111);
     float result11000 =
        result011000 + lerp1 * (result111000 - result011000);
     float result11001 =
        result011001 + lerp1 * (result111001 - result011001);
     float result11010 =
        result011010 + lerp1 * (result111010 - result011010);
     float result11011 =
        result011011 + lerp1 * (result111011 - result011011);
     float result11100 =
        result011100 + lerp1 * (result111100 - result011100);
     float result11101 =
        result011101 + lerp1 * (result111101 - result011101);
     float result11110 =
        result011110 + lerp1 * (result111110 - result011110);
     float result11111 =
        result011111 + lerp1 * (result111111 - result011111);
    
      float result0000 =
        result00000 + lerp2 * (result10000 - result00000);
     float result0001 =
        result00001 + lerp2 * (result10001 - result00001);
     float result0010 =
        result00010 + lerp2 * (result10010 - result00010);
     float result0011 =
        result00011 + lerp2 * (result10011 - result00011);
     float result0100 =
        result00100 + lerp2 * (result10100 - result00100);
     float result0101 =
        result00101 + lerp2 * (result10101 - result00101);
     float result0110 =
        result00110 + lerp2 * (result10110 - result00110);
     float result0111 =
        result00111 + lerp2 * (result10111 - result00111);
     float result1000 =
        result01000 + lerp2 * (result11000 - result01000);
     float result1001 =
        result01001 + lerp2 * (result11001 - result01001);
     float result1010 =
        result01010 + lerp2 * (result11010 - result01010);
     float result1011 =
        result01011 + lerp2 * (result11011 - result01011);
     float result1100 =
        result01100 + lerp2 * (result11100 - result01100);
     float result1101 =
        result01101 + lerp2 * (result11101 - result01101);
     float result1110 =
        result01110 + lerp2 * (result11110 - result01110);
     float result1111 =
        result01111 + lerp2 * (result11111 - result01111);
    
      float result000 =
        result0000 + lerp3 * (result1000 - result0000);
     float result001 =
        result0001 + lerp3 * (result1001 - result0001);
     float result010 =
        result0010 + lerp3 * (result1010 - result0010);
     float result011 =
        result0011 + lerp3 * (result1011 - result0011);
     float result100 =
        result0100 + lerp3 * (result1100 - result0100);
     float result101 =
        result0101 + lerp3 * (result1101 - result0101);
     float result110 =
        result0110 + lerp3 * (result1110 - result0110);
     float result111 =
        result0111 + lerp3 * (result1111 - result0111);
    
      float result00 =
        result000 + lerp4 * (result100 - result000);
     float result01 =
        result001 + lerp4 * (result101 - result001);
     float result10 =
        result010 + lerp4 * (result110 - result010);
     float result11 =
        result011 + lerp4 * (result111 - result011);
    
      float result0 =
        result00 + lerp5 * (result10 - result00);
     float result1 =
        result01 + lerp5 * (result11 - result01);
    
      float result =
        result0 + lerp6 * (result1 - result0);
    
    
    total += result * amplitude;
    amplitude *= persistence;
     pos0 *= 2;
     pos1 *= 2;
     pos2 *= 2;
     pos3 *= 2;
     pos4 *= 2;
     pos5 *= 2;
     pos6 *= 2;
    
  }
  return total;
}

#ifndef TERRAIN_NON_REP_
#define TERRAIN_NON_REP_

int fastFloor(float x) {
  int i = x;
  return i - (x < i);
}

#endif // TERRAIN_NON_REP_

uint hash8( int pos0,  int pos1,  int pos2,  int pos3,  int pos4,  int pos5,  int pos6,  int pos7, 
                 uint octave, uint numGradVecs) {
  // TODO: WRITE THIS
  return (pos0 * pos0 + pos1 + pos2 * pos2 * pos2 + octave) % numGradVecs;
}

float getNoise8( float scale0, float pos0,  float scale1, float pos1,  float scale2, float pos2,  float scale3, float pos3,  float scale4, float pos4,  float scale5, float pos5,  float scale6, float pos6,  float scale7, float pos7, 
                      __global float *gradVecs, uint numGradVecs,
                      int numOctaves, float persistence) {
  float total = 0;
  float amplitude = 1;
   pos0 = (pos0 + 0.5) / scale0;
   pos1 = (pos1 + 0.5) / scale1;
   pos2 = (pos2 + 0.5) / scale2;
   pos3 = (pos3 + 0.5) / scale3;
   pos4 = (pos4 + 0.5) / scale4;
   pos5 = (pos5 + 0.5) / scale5;
   pos6 = (pos6 + 0.5) / scale6;
   pos7 = (pos7 + 0.5) / scale7;
  
  for (uint i = numOctaves; i--;) {
     int posf0 = fastFloor(pos0);
    float vec00 = pos0 - posf0;
    float vec10 = vec00 - 1;
    float lerp0 = vec00 * vec00 * (3 - 2 * vec00);
     int posf1 = fastFloor(pos1);
    float vec01 = pos1 - posf1;
    float vec11 = vec01 - 1;
    float lerp1 = vec01 * vec01 * (3 - 2 * vec01);
     int posf2 = fastFloor(pos2);
    float vec02 = pos2 - posf2;
    float vec12 = vec02 - 1;
    float lerp2 = vec02 * vec02 * (3 - 2 * vec02);
     int posf3 = fastFloor(pos3);
    float vec03 = pos3 - posf3;
    float vec13 = vec03 - 1;
    float lerp3 = vec03 * vec03 * (3 - 2 * vec03);
     int posf4 = fastFloor(pos4);
    float vec04 = pos4 - posf4;
    float vec14 = vec04 - 1;
    float lerp4 = vec04 * vec04 * (3 - 2 * vec04);
     int posf5 = fastFloor(pos5);
    float vec05 = pos5 - posf5;
    float vec15 = vec05 - 1;
    float lerp5 = vec05 * vec05 * (3 - 2 * vec05);
     int posf6 = fastFloor(pos6);
    float vec06 = pos6 - posf6;
    float vec16 = vec06 - 1;
    float lerp6 = vec06 * vec06 * (3 - 2 * vec06);
     int posf7 = fastFloor(pos7);
    float vec07 = pos7 - posf7;
    float vec17 = vec07 - 1;
    float lerp7 = vec07 * vec07 * (3 - 2 * vec07);
    
    __global float *tmp;
     tmp =
        gradVecs +
        8 * hash8( posf0 + 0,  posf1 + 0,  posf2 + 0,  posf3 + 0,  posf4 + 0,  posf5 + 0,  posf6 + 0,  posf7 + 0, 
                              i, numGradVecs);
    float result00000000 =  vec00 * tmp[0] +  vec01 * tmp[1] +  vec02 * tmp[2] +  vec03 * tmp[3] +  vec04 * tmp[4] +  vec05 * tmp[5] +  vec06 * tmp[6] +  vec07 * tmp[7] + 
    0;
     tmp =
        gradVecs +
        8 * hash8( posf0 + 0,  posf1 + 0,  posf2 + 0,  posf3 + 0,  posf4 + 0,  posf5 + 0,  posf6 + 0,  posf7 + 1, 
                              i, numGradVecs);
    float result00000001 =  vec00 * tmp[0] +  vec01 * tmp[1] +  vec02 * tmp[2] +  vec03 * tmp[3] +  vec04 * tmp[4] +  vec05 * tmp[5] +  vec06 * tmp[6] +  vec17 * tmp[7] + 
    0;
     tmp =
        gradVecs +
        8 * hash8( posf0 + 0,  posf1 + 0,  posf2 + 0,  posf3 + 0,  posf4 + 0,  posf5 + 0,  posf6 + 1,  posf7 + 0, 
                              i, numGradVecs);
    float result00000010 =  vec00 * tmp[0] +  vec01 * tmp[1] +  vec02 * tmp[2] +  vec03 * tmp[3] +  vec04 * tmp[4] +  vec05 * tmp[5] +  vec16 * tmp[6] +  vec07 * tmp[7] + 
    0;
     tmp =
        gradVecs +
        8 * hash8( posf0 + 0,  posf1 + 0,  posf2 + 0,  posf3 + 0,  posf4 + 0,  posf5 + 0,  posf6 + 1,  posf7 + 1, 
                              i, numGradVecs);
    float result00000011 =  vec00 * tmp[0] +  vec01 * tmp[1] +  vec02 * tmp[2] +  vec03 * tmp[3] +  vec04 * tmp[4] +  vec05 * tmp[5] +  vec16 * tmp[6] +  vec17 * tmp[7] + 
    0;
     tmp =
        gradVecs +
        8 * hash8( posf0 + 0,  posf1 + 0,  posf2 + 0,  posf3 + 0,  posf4 + 0,  posf5 + 1,  posf6 + 0,  posf7 + 0, 
                              i, numGradVecs);
    float result00000100 =  vec00 * tmp[0] +  vec01 * tmp[1] +  vec02 * tmp[2] +  vec03 * tmp[3] +  vec04 * tmp[4] +  vec15 * tmp[5] +  vec06 * tmp[6] +  vec07 * tmp[7] + 
    0;
     tmp =
        gradVecs +
        8 * hash8( posf0 + 0,  posf1 + 0,  posf2 + 0,  posf3 + 0,  posf4 + 0,  posf5 + 1,  posf6 + 0,  posf7 + 1, 
                              i, numGradVecs);
    float result00000101 =  vec00 * tmp[0] +  vec01 * tmp[1] +  vec02 * tmp[2] +  vec03 * tmp[3] +  vec04 * tmp[4] +  vec15 * tmp[5] +  vec06 * tmp[6] +  vec17 * tmp[7] + 
    0;
     tmp =
        gradVecs +
        8 * hash8( posf0 + 0,  posf1 + 0,  posf2 + 0,  posf3 + 0,  posf4 + 0,  posf5 + 1,  posf6 + 1,  posf7 + 0, 
                              i, numGradVecs);
    float result00000110 =  vec00 * tmp[0] +  vec01 * tmp[1] +  vec02 * tmp[2] +  vec03 * tmp[3] +  vec04 * tmp[4] +  vec15 * tmp[5] +  vec16 * tmp[6] +  vec07 * tmp[7] + 
    0;
     tmp =
        gradVecs +
        8 * hash8( posf0 + 0,  posf1 + 0,  posf2 + 0,  posf3 + 0,  posf4 + 0,  posf5 + 1,  posf6 + 1,  posf7 + 1, 
                              i, numGradVecs);
    float result00000111 =  vec00 * tmp[0] +  vec01 * tmp[1] +  vec02 * tmp[2] +  vec03 * tmp[3] +  vec04 * tmp[4] +  vec15 * tmp[5] +  vec16 * tmp[6] +  vec17 * tmp[7] + 
    0;
     tmp =
        gradVecs +
        8 * hash8( posf0 + 0,  posf1 + 0,  posf2 + 0,  posf3 + 0,  posf4 + 1,  posf5 + 0,  posf6 + 0,  posf7 + 0, 
                              i, numGradVecs);
    float result00001000 =  vec00 * tmp[0] +  vec01 * tmp[1] +  vec02 * tmp[2] +  vec03 * tmp[3] +  vec14 * tmp[4] +  vec05 * tmp[5] +  vec06 * tmp[6] +  vec07 * tmp[7] + 
    0;
     tmp =
        gradVecs +
        8 * hash8( posf0 + 0,  posf1 + 0,  posf2 + 0,  posf3 + 0,  posf4 + 1,  posf5 + 0,  posf6 + 0,  posf7 + 1, 
                              i, numGradVecs);
    float result00001001 =  vec00 * tmp[0] +  vec01 * tmp[1] +  vec02 * tmp[2] +  vec03 * tmp[3] +  vec14 * tmp[4] +  vec05 * tmp[5] +  vec06 * tmp[6] +  vec17 * tmp[7] + 
    0;
     tmp =
        gradVecs +
        8 * hash8( posf0 + 0,  posf1 + 0,  posf2 + 0,  posf3 + 0,  posf4 + 1,  posf5 + 0,  posf6 + 1,  posf7 + 0, 
                              i, numGradVecs);
    float result00001010 =  vec00 * tmp[0] +  vec01 * tmp[1] +  vec02 * tmp[2] +  vec03 * tmp[3] +  vec14 * tmp[4] +  vec05 * tmp[5] +  vec16 * tmp[6] +  vec07 * tmp[7] + 
    0;
     tmp =
        gradVecs +
        8 * hash8( posf0 + 0,  posf1 + 0,  posf2 + 0,  posf3 + 0,  posf4 + 1,  posf5 + 0,  posf6 + 1,  posf7 + 1, 
                              i, numGradVecs);
    float result00001011 =  vec00 * tmp[0] +  vec01 * tmp[1] +  vec02 * tmp[2] +  vec03 * tmp[3] +  vec14 * tmp[4] +  vec05 * tmp[5] +  vec16 * tmp[6] +  vec17 * tmp[7] + 
    0;
     tmp =
        gradVecs +
        8 * hash8( posf0 + 0,  posf1 + 0,  posf2 + 0,  posf3 + 0,  posf4 + 1,  posf5 + 1,  posf6 + 0,  posf7 + 0, 
                              i, numGradVecs);
    float result00001100 =  vec00 * tmp[0] +  vec01 * tmp[1] +  vec02 * tmp[2] +  vec03 * tmp[3] +  vec14 * tmp[4] +  vec15 * tmp[5] +  vec06 * tmp[6] +  vec07 * tmp[7] + 
    0;
     tmp =
        gradVecs +
        8 * hash8( posf0 + 0,  posf1 + 0,  posf2 + 0,  posf3 + 0,  posf4 + 1,  posf5 + 1,  posf6 + 0,  posf7 + 1, 
                              i, numGradVecs);
    float result00001101 =  vec00 * tmp[0] +  vec01 * tmp[1] +  vec02 * tmp[2] +  vec03 * tmp[3] +  vec14 * tmp[4] +  vec15 * tmp[5] +  vec06 * tmp[6] +  vec17 * tmp[7] + 
    0;
     tmp =
        gradVecs +
        8 * hash8( posf0 + 0,  posf1 + 0,  posf2 + 0,  posf3 + 0,  posf4 + 1,  posf5 + 1,  posf6 + 1,  posf7 + 0, 
                              i, numGradVecs);
    float result00001110 =  vec00 * tmp[0] +  vec01 * tmp[1] +  vec02 * tmp[2] +  vec03 * tmp[3] +  vec14 * tmp[4] +  vec15 * tmp[5] +  vec16 * tmp[6] +  vec07 * tmp[7] + 
    0;
     tmp =
        gradVecs +
        8 * hash8( posf0 + 0,  posf1 + 0,  posf2 + 0,  posf3 + 0,  posf4 + 1,  posf5 + 1,  posf6 + 1,  posf7 + 1, 
                              i, numGradVecs);
    float result00001111 =  vec00 * tmp[0] +  vec01 * tmp[1] +  vec02 * tmp[2] +  vec03 * tmp[3] +  vec14 * tmp[4] +  vec15 * tmp[5] +  vec16 * tmp[6] +  vec17 * tmp[7] + 
    0;
     tmp =
        gradVecs +
        8 * hash8( posf0 + 0,  posf1 + 0,  posf2 + 0,  posf3 + 1,  posf4 + 0,  posf5 + 0,  posf6 + 0,  posf7 + 0, 
                              i, numGradVecs);
    float result00010000 =  vec00 * tmp[0] +  vec01 * tmp[1] +  vec02 * tmp[2] +  vec13 * tmp[3] +  vec04 * tmp[4] +  vec05 * tmp[5] +  vec06 * tmp[6] +  vec07 * tmp[7] + 
    0;
     tmp =
        gradVecs +
        8 * hash8( posf0 + 0,  posf1 + 0,  posf2 + 0,  posf3 + 1,  posf4 + 0,  posf5 + 0,  posf6 + 0,  posf7 + 1, 
                              i, numGradVecs);
    float result00010001 =  vec00 * tmp[0] +  vec01 * tmp[1] +  vec02 * tmp[2] +  vec13 * tmp[3] +  vec04 * tmp[4] +  vec05 * tmp[5] +  vec06 * tmp[6] +  vec17 * tmp[7] + 
    0;
     tmp =
        gradVecs +
        8 * hash8( posf0 + 0,  posf1 + 0,  posf2 + 0,  posf3 + 1,  posf4 + 0,  posf5 + 0,  posf6 + 1,  posf7 + 0, 
                              i, numGradVecs);
    float result00010010 =  vec00 * tmp[0] +  vec01 * tmp[1] +  vec02 * tmp[2] +  vec13 * tmp[3] +  vec04 * tmp[4] +  vec05 * tmp[5] +  vec16 * tmp[6] +  vec07 * tmp[7] + 
    0;
     tmp =
        gradVecs +
        8 * hash8( posf0 + 0,  posf1 + 0,  posf2 + 0,  posf3 + 1,  posf4 + 0,  posf5 + 0,  posf6 + 1,  posf7 + 1, 
                              i, numGradVecs);
    float result00010011 =  vec00 * tmp[0] +  vec01 * tmp[1] +  vec02 * tmp[2] +  vec13 * tmp[3] +  vec04 * tmp[4] +  vec05 * tmp[5] +  vec16 * tmp[6] +  vec17 * tmp[7] + 
    0;
     tmp =
        gradVecs +
        8 * hash8( posf0 + 0,  posf1 + 0,  posf2 + 0,  posf3 + 1,  posf4 + 0,  posf5 + 1,  posf6 + 0,  posf7 + 0, 
                              i, numGradVecs);
    float result00010100 =  vec00 * tmp[0] +  vec01 * tmp[1] +  vec02 * tmp[2] +  vec13 * tmp[3] +  vec04 * tmp[4] +  vec15 * tmp[5] +  vec06 * tmp[6] +  vec07 * tmp[7] + 
    0;
     tmp =
        gradVecs +
        8 * hash8( posf0 + 0,  posf1 + 0,  posf2 + 0,  posf3 + 1,  posf4 + 0,  posf5 + 1,  posf6 + 0,  posf7 + 1, 
                              i, numGradVecs);
    float result00010101 =  vec00 * tmp[0] +  vec01 * tmp[1] +  vec02 * tmp[2] +  vec13 * tmp[3] +  vec04 * tmp[4] +  vec15 * tmp[5] +  vec06 * tmp[6] +  vec17 * tmp[7] + 
    0;
     tmp =
        gradVecs +
        8 * hash8( posf0 + 0,  posf1 + 0,  posf2 + 0,  posf3 + 1,  posf4 + 0,  posf5 + 1,  posf6 + 1,  posf7 + 0, 
                              i, numGradVecs);
    float result00010110 =  vec00 * tmp[0] +  vec01 * tmp[1] +  vec02 * tmp[2] +  vec13 * tmp[3] +  vec04 * tmp[4] +  vec15 * tmp[5] +  vec16 * tmp[6] +  vec07 * tmp[7] + 
    0;
     tmp =
        gradVecs +
        8 * hash8( posf0 + 0,  posf1 + 0,  posf2 + 0,  posf3 + 1,  posf4 + 0,  posf5 + 1,  posf6 + 1,  posf7 + 1, 
                              i, numGradVecs);
    float result00010111 =  vec00 * tmp[0] +  vec01 * tmp[1] +  vec02 * tmp[2] +  vec13 * tmp[3] +  vec04 * tmp[4] +  vec15 * tmp[5] +  vec16 * tmp[6] +  vec17 * tmp[7] + 
    0;
     tmp =
        gradVecs +
        8 * hash8( posf0 + 0,  posf1 + 0,  posf2 + 0,  posf3 + 1,  posf4 + 1,  posf5 + 0,  posf6 + 0,  posf7 + 0, 
                              i, numGradVecs);
    float result00011000 =  vec00 * tmp[0] +  vec01 * tmp[1] +  vec02 * tmp[2] +  vec13 * tmp[3] +  vec14 * tmp[4] +  vec05 * tmp[5] +  vec06 * tmp[6] +  vec07 * tmp[7] + 
    0;
     tmp =
        gradVecs +
        8 * hash8( posf0 + 0,  posf1 + 0,  posf2 + 0,  posf3 + 1,  posf4 + 1,  posf5 + 0,  posf6 + 0,  posf7 + 1, 
                              i, numGradVecs);
    float result00011001 =  vec00 * tmp[0] +  vec01 * tmp[1] +  vec02 * tmp[2] +  vec13 * tmp[3] +  vec14 * tmp[4] +  vec05 * tmp[5] +  vec06 * tmp[6] +  vec17 * tmp[7] + 
    0;
     tmp =
        gradVecs +
        8 * hash8( posf0 + 0,  posf1 + 0,  posf2 + 0,  posf3 + 1,  posf4 + 1,  posf5 + 0,  posf6 + 1,  posf7 + 0, 
                              i, numGradVecs);
    float result00011010 =  vec00 * tmp[0] +  vec01 * tmp[1] +  vec02 * tmp[2] +  vec13 * tmp[3] +  vec14 * tmp[4] +  vec05 * tmp[5] +  vec16 * tmp[6] +  vec07 * tmp[7] + 
    0;
     tmp =
        gradVecs +
        8 * hash8( posf0 + 0,  posf1 + 0,  posf2 + 0,  posf3 + 1,  posf4 + 1,  posf5 + 0,  posf6 + 1,  posf7 + 1, 
                              i, numGradVecs);
    float result00011011 =  vec00 * tmp[0] +  vec01 * tmp[1] +  vec02 * tmp[2] +  vec13 * tmp[3] +  vec14 * tmp[4] +  vec05 * tmp[5] +  vec16 * tmp[6] +  vec17 * tmp[7] + 
    0;
     tmp =
        gradVecs +
        8 * hash8( posf0 + 0,  posf1 + 0,  posf2 + 0,  posf3 + 1,  posf4 + 1,  posf5 + 1,  posf6 + 0,  posf7 + 0, 
                              i, numGradVecs);
    float result00011100 =  vec00 * tmp[0] +  vec01 * tmp[1] +  vec02 * tmp[2] +  vec13 * tmp[3] +  vec14 * tmp[4] +  vec15 * tmp[5] +  vec06 * tmp[6] +  vec07 * tmp[7] + 
    0;
     tmp =
        gradVecs +
        8 * hash8( posf0 + 0,  posf1 + 0,  posf2 + 0,  posf3 + 1,  posf4 + 1,  posf5 + 1,  posf6 + 0,  posf7 + 1, 
                              i, numGradVecs);
    float result00011101 =  vec00 * tmp[0] +  vec01 * tmp[1] +  vec02 * tmp[2] +  vec13 * tmp[3] +  vec14 * tmp[4] +  vec15 * tmp[5] +  vec06 * tmp[6] +  vec17 * tmp[7] + 
    0;
     tmp =
        gradVecs +
        8 * hash8( posf0 + 0,  posf1 + 0,  posf2 + 0,  posf3 + 1,  posf4 + 1,  posf5 + 1,  posf6 + 1,  posf7 + 0, 
                              i, numGradVecs);
    float result00011110 =  vec00 * tmp[0] +  vec01 * tmp[1] +  vec02 * tmp[2] +  vec13 * tmp[3] +  vec14 * tmp[4] +  vec15 * tmp[5] +  vec16 * tmp[6] +  vec07 * tmp[7] + 
    0;
     tmp =
        gradVecs +
        8 * hash8( posf0 + 0,  posf1 + 0,  posf2 + 0,  posf3 + 1,  posf4 + 1,  posf5 + 1,  posf6 + 1,  posf7 + 1, 
                              i, numGradVecs);
    float result00011111 =  vec00 * tmp[0] +  vec01 * tmp[1] +  vec02 * tmp[2] +  vec13 * tmp[3] +  vec14 * tmp[4] +  vec15 * tmp[5] +  vec16 * tmp[6] +  vec17 * tmp[7] + 
    0;
     tmp =
        gradVecs +
        8 * hash8( posf0 + 0,  posf1 + 0,  posf2 + 1,  posf3 + 0,  posf4 + 0,  posf5 + 0,  posf6 + 0,  posf7 + 0, 
                              i, numGradVecs);
    float result00100000 =  vec00 * tmp[0] +  vec01 * tmp[1] +  vec12 * tmp[2] +  vec03 * tmp[3] +  vec04 * tmp[4] +  vec05 * tmp[5] +  vec06 * tmp[6] +  vec07 * tmp[7] + 
    0;
     tmp =
        gradVecs +
        8 * hash8( posf0 + 0,  posf1 + 0,  posf2 + 1,  posf3 + 0,  posf4 + 0,  posf5 + 0,  posf6 + 0,  posf7 + 1, 
                              i, numGradVecs);
    float result00100001 =  vec00 * tmp[0] +  vec01 * tmp[1] +  vec12 * tmp[2] +  vec03 * tmp[3] +  vec04 * tmp[4] +  vec05 * tmp[5] +  vec06 * tmp[6] +  vec17 * tmp[7] + 
    0;
     tmp =
        gradVecs +
        8 * hash8( posf0 + 0,  posf1 + 0,  posf2 + 1,  posf3 + 0,  posf4 + 0,  posf5 + 0,  posf6 + 1,  posf7 + 0, 
                              i, numGradVecs);
    float result00100010 =  vec00 * tmp[0] +  vec01 * tmp[1] +  vec12 * tmp[2] +  vec03 * tmp[3] +  vec04 * tmp[4] +  vec05 * tmp[5] +  vec16 * tmp[6] +  vec07 * tmp[7] + 
    0;
     tmp =
        gradVecs +
        8 * hash8( posf0 + 0,  posf1 + 0,  posf2 + 1,  posf3 + 0,  posf4 + 0,  posf5 + 0,  posf6 + 1,  posf7 + 1, 
                              i, numGradVecs);
    float result00100011 =  vec00 * tmp[0] +  vec01 * tmp[1] +  vec12 * tmp[2] +  vec03 * tmp[3] +  vec04 * tmp[4] +  vec05 * tmp[5] +  vec16 * tmp[6] +  vec17 * tmp[7] + 
    0;
     tmp =
        gradVecs +
        8 * hash8( posf0 + 0,  posf1 + 0,  posf2 + 1,  posf3 + 0,  posf4 + 0,  posf5 + 1,  posf6 + 0,  posf7 + 0, 
                              i, numGradVecs);
    float result00100100 =  vec00 * tmp[0] +  vec01 * tmp[1] +  vec12 * tmp[2] +  vec03 * tmp[3] +  vec04 * tmp[4] +  vec15 * tmp[5] +  vec06 * tmp[6] +  vec07 * tmp[7] + 
    0;
     tmp =
        gradVecs +
        8 * hash8( posf0 + 0,  posf1 + 0,  posf2 + 1,  posf3 + 0,  posf4 + 0,  posf5 + 1,  posf6 + 0,  posf7 + 1, 
                              i, numGradVecs);
    float result00100101 =  vec00 * tmp[0] +  vec01 * tmp[1] +  vec12 * tmp[2] +  vec03 * tmp[3] +  vec04 * tmp[4] +  vec15 * tmp[5] +  vec06 * tmp[6] +  vec17 * tmp[7] + 
    0;
     tmp =
        gradVecs +
        8 * hash8( posf0 + 0,  posf1 + 0,  posf2 + 1,  posf3 + 0,  posf4 + 0,  posf5 + 1,  posf6 + 1,  posf7 + 0, 
                              i, numGradVecs);
    float result00100110 =  vec00 * tmp[0] +  vec01 * tmp[1] +  vec12 * tmp[2] +  vec03 * tmp[3] +  vec04 * tmp[4] +  vec15 * tmp[5] +  vec16 * tmp[6] +  vec07 * tmp[7] + 
    0;
     tmp =
        gradVecs +
        8 * hash8( posf0 + 0,  posf1 + 0,  posf2 + 1,  posf3 + 0,  posf4 + 0,  posf5 + 1,  posf6 + 1,  posf7 + 1, 
                              i, numGradVecs);
    float result00100111 =  vec00 * tmp[0] +  vec01 * tmp[1] +  vec12 * tmp[2] +  vec03 * tmp[3] +  vec04 * tmp[4] +  vec15 * tmp[5] +  vec16 * tmp[6] +  vec17 * tmp[7] + 
    0;
     tmp =
        gradVecs +
        8 * hash8( posf0 + 0,  posf1 + 0,  posf2 + 1,  posf3 + 0,  posf4 + 1,  posf5 + 0,  posf6 + 0,  posf7 + 0, 
                              i, numGradVecs);
    float result00101000 =  vec00 * tmp[0] +  vec01 * tmp[1] +  vec12 * tmp[2] +  vec03 * tmp[3] +  vec14 * tmp[4] +  vec05 * tmp[5] +  vec06 * tmp[6] +  vec07 * tmp[7] + 
    0;
     tmp =
        gradVecs +
        8 * hash8( posf0 + 0,  posf1 + 0,  posf2 + 1,  posf3 + 0,  posf4 + 1,  posf5 + 0,  posf6 + 0,  posf7 + 1, 
                              i, numGradVecs);
    float result00101001 =  vec00 * tmp[0] +  vec01 * tmp[1] +  vec12 * tmp[2] +  vec03 * tmp[3] +  vec14 * tmp[4] +  vec05 * tmp[5] +  vec06 * tmp[6] +  vec17 * tmp[7] + 
    0;
     tmp =
        gradVecs +
        8 * hash8( posf0 + 0,  posf1 + 0,  posf2 + 1,  posf3 + 0,  posf4 + 1,  posf5 + 0,  posf6 + 1,  posf7 + 0, 
                              i, numGradVecs);
    float result00101010 =  vec00 * tmp[0] +  vec01 * tmp[1] +  vec12 * tmp[2] +  vec03 * tmp[3] +  vec14 * tmp[4] +  vec05 * tmp[5] +  vec16 * tmp[6] +  vec07 * tmp[7] + 
    0;
     tmp =
        gradVecs +
        8 * hash8( posf0 + 0,  posf1 + 0,  posf2 + 1,  posf3 + 0,  posf4 + 1,  posf5 + 0,  posf6 + 1,  posf7 + 1, 
                              i, numGradVecs);
    float result00101011 =  vec00 * tmp[0] +  vec01 * tmp[1] +  vec12 * tmp[2] +  vec03 * tmp[3] +  vec14 * tmp[4] +  vec05 * tmp[5] +  vec16 * tmp[6] +  vec17 * tmp[7] + 
    0;
     tmp =
        gradVecs +
        8 * hash8( posf0 + 0,  posf1 + 0,  posf2 + 1,  posf3 + 0,  posf4 + 1,  posf5 + 1,  posf6 + 0,  posf7 + 0, 
                              i, numGradVecs);
    float result00101100 =  vec00 * tmp[0] +  vec01 * tmp[1] +  vec12 * tmp[2] +  vec03 * tmp[3] +  vec14 * tmp[4] +  vec15 * tmp[5] +  vec06 * tmp[6] +  vec07 * tmp[7] + 
    0;
     tmp =
        gradVecs +
        8 * hash8( posf0 + 0,  posf1 + 0,  posf2 + 1,  posf3 + 0,  posf4 + 1,  posf5 + 1,  posf6 + 0,  posf7 + 1, 
                              i, numGradVecs);
    float result00101101 =  vec00 * tmp[0] +  vec01 * tmp[1] +  vec12 * tmp[2] +  vec03 * tmp[3] +  vec14 * tmp[4] +  vec15 * tmp[5] +  vec06 * tmp[6] +  vec17 * tmp[7] + 
    0;
     tmp =
        gradVecs +
        8 * hash8( posf0 + 0,  posf1 + 0,  posf2 + 1,  posf3 + 0,  posf4 + 1,  posf5 + 1,  posf6 + 1,  posf7 + 0, 
                              i, numGradVecs);
    float result00101110 =  vec00 * tmp[0] +  vec01 * tmp[1] +  vec12 * tmp[2] +  vec03 * tmp[3] +  vec14 * tmp[4] +  vec15 * tmp[5] +  vec16 * tmp[6] +  vec07 * tmp[7] + 
    0;
     tmp =
        gradVecs +
        8 * hash8( posf0 + 0,  posf1 + 0,  posf2 + 1,  posf3 + 0,  posf4 + 1,  posf5 + 1,  posf6 + 1,  posf7 + 1, 
                              i, numGradVecs);
    float result00101111 =  vec00 * tmp[0] +  vec01 * tmp[1] +  vec12 * tmp[2] +  vec03 * tmp[3] +  vec14 * tmp[4] +  vec15 * tmp[5] +  vec16 * tmp[6] +  vec17 * tmp[7] + 
    0;
     tmp =
        gradVecs +
        8 * hash8( posf0 + 0,  posf1 + 0,  posf2 + 1,  posf3 + 1,  posf4 + 0,  posf5 + 0,  posf6 + 0,  posf7 + 0, 
                              i, numGradVecs);
    float result00110000 =  vec00 * tmp[0] +  vec01 * tmp[1] +  vec12 * tmp[2] +  vec13 * tmp[3] +  vec04 * tmp[4] +  vec05 * tmp[5] +  vec06 * tmp[6] +  vec07 * tmp[7] + 
    0;
     tmp =
        gradVecs +
        8 * hash8( posf0 + 0,  posf1 + 0,  posf2 + 1,  posf3 + 1,  posf4 + 0,  posf5 + 0,  posf6 + 0,  posf7 + 1, 
                              i, numGradVecs);
    float result00110001 =  vec00 * tmp[0] +  vec01 * tmp[1] +  vec12 * tmp[2] +  vec13 * tmp[3] +  vec04 * tmp[4] +  vec05 * tmp[5] +  vec06 * tmp[6] +  vec17 * tmp[7] + 
    0;
     tmp =
        gradVecs +
        8 * hash8( posf0 + 0,  posf1 + 0,  posf2 + 1,  posf3 + 1,  posf4 + 0,  posf5 + 0,  posf6 + 1,  posf7 + 0, 
                              i, numGradVecs);
    float result00110010 =  vec00 * tmp[0] +  vec01 * tmp[1] +  vec12 * tmp[2] +  vec13 * tmp[3] +  vec04 * tmp[4] +  vec05 * tmp[5] +  vec16 * tmp[6] +  vec07 * tmp[7] + 
    0;
     tmp =
        gradVecs +
        8 * hash8( posf0 + 0,  posf1 + 0,  posf2 + 1,  posf3 + 1,  posf4 + 0,  posf5 + 0,  posf6 + 1,  posf7 + 1, 
                              i, numGradVecs);
    float result00110011 =  vec00 * tmp[0] +  vec01 * tmp[1] +  vec12 * tmp[2] +  vec13 * tmp[3] +  vec04 * tmp[4] +  vec05 * tmp[5] +  vec16 * tmp[6] +  vec17 * tmp[7] + 
    0;
     tmp =
        gradVecs +
        8 * hash8( posf0 + 0,  posf1 + 0,  posf2 + 1,  posf3 + 1,  posf4 + 0,  posf5 + 1,  posf6 + 0,  posf7 + 0, 
                              i, numGradVecs);
    float result00110100 =  vec00 * tmp[0] +  vec01 * tmp[1] +  vec12 * tmp[2] +  vec13 * tmp[3] +  vec04 * tmp[4] +  vec15 * tmp[5] +  vec06 * tmp[6] +  vec07 * tmp[7] + 
    0;
     tmp =
        gradVecs +
        8 * hash8( posf0 + 0,  posf1 + 0,  posf2 + 1,  posf3 + 1,  posf4 + 0,  posf5 + 1,  posf6 + 0,  posf7 + 1, 
                              i, numGradVecs);
    float result00110101 =  vec00 * tmp[0] +  vec01 * tmp[1] +  vec12 * tmp[2] +  vec13 * tmp[3] +  vec04 * tmp[4] +  vec15 * tmp[5] +  vec06 * tmp[6] +  vec17 * tmp[7] + 
    0;
     tmp =
        gradVecs +
        8 * hash8( posf0 + 0,  posf1 + 0,  posf2 + 1,  posf3 + 1,  posf4 + 0,  posf5 + 1,  posf6 + 1,  posf7 + 0, 
                              i, numGradVecs);
    float result00110110 =  vec00 * tmp[0] +  vec01 * tmp[1] +  vec12 * tmp[2] +  vec13 * tmp[3] +  vec04 * tmp[4] +  vec15 * tmp[5] +  vec16 * tmp[6] +  vec07 * tmp[7] + 
    0;
     tmp =
        gradVecs +
        8 * hash8( posf0 + 0,  posf1 + 0,  posf2 + 1,  posf3 + 1,  posf4 + 0,  posf5 + 1,  posf6 + 1,  posf7 + 1, 
                              i, numGradVecs);
    float result00110111 =  vec00 * tmp[0] +  vec01 * tmp[1] +  vec12 * tmp[2] +  vec13 * tmp[3] +  vec04 * tmp[4] +  vec15 * tmp[5] +  vec16 * tmp[6] +  vec17 * tmp[7] + 
    0;
     tmp =
        gradVecs +
        8 * hash8( posf0 + 0,  posf1 + 0,  posf2 + 1,  posf3 + 1,  posf4 + 1,  posf5 + 0,  posf6 + 0,  posf7 + 0, 
                              i, numGradVecs);
    float result00111000 =  vec00 * tmp[0] +  vec01 * tmp[1] +  vec12 * tmp[2] +  vec13 * tmp[3] +  vec14 * tmp[4] +  vec05 * tmp[5] +  vec06 * tmp[6] +  vec07 * tmp[7] + 
    0;
     tmp =
        gradVecs +
        8 * hash8( posf0 + 0,  posf1 + 0,  posf2 + 1,  posf3 + 1,  posf4 + 1,  posf5 + 0,  posf6 + 0,  posf7 + 1, 
                              i, numGradVecs);
    float result00111001 =  vec00 * tmp[0] +  vec01 * tmp[1] +  vec12 * tmp[2] +  vec13 * tmp[3] +  vec14 * tmp[4] +  vec05 * tmp[5] +  vec06 * tmp[6] +  vec17 * tmp[7] + 
    0;
     tmp =
        gradVecs +
        8 * hash8( posf0 + 0,  posf1 + 0,  posf2 + 1,  posf3 + 1,  posf4 + 1,  posf5 + 0,  posf6 + 1,  posf7 + 0, 
                              i, numGradVecs);
    float result00111010 =  vec00 * tmp[0] +  vec01 * tmp[1] +  vec12 * tmp[2] +  vec13 * tmp[3] +  vec14 * tmp[4] +  vec05 * tmp[5] +  vec16 * tmp[6] +  vec07 * tmp[7] + 
    0;
     tmp =
        gradVecs +
        8 * hash8( posf0 + 0,  posf1 + 0,  posf2 + 1,  posf3 + 1,  posf4 + 1,  posf5 + 0,  posf6 + 1,  posf7 + 1, 
                              i, numGradVecs);
    float result00111011 =  vec00 * tmp[0] +  vec01 * tmp[1] +  vec12 * tmp[2] +  vec13 * tmp[3] +  vec14 * tmp[4] +  vec05 * tmp[5] +  vec16 * tmp[6] +  vec17 * tmp[7] + 
    0;
     tmp =
        gradVecs +
        8 * hash8( posf0 + 0,  posf1 + 0,  posf2 + 1,  posf3 + 1,  posf4 + 1,  posf5 + 1,  posf6 + 0,  posf7 + 0, 
                              i, numGradVecs);
    float result00111100 =  vec00 * tmp[0] +  vec01 * tmp[1] +  vec12 * tmp[2] +  vec13 * tmp[3] +  vec14 * tmp[4] +  vec15 * tmp[5] +  vec06 * tmp[6] +  vec07 * tmp[7] + 
    0;
     tmp =
        gradVecs +
        8 * hash8( posf0 + 0,  posf1 + 0,  posf2 + 1,  posf3 + 1,  posf4 + 1,  posf5 + 1,  posf6 + 0,  posf7 + 1, 
                              i, numGradVecs);
    float result00111101 =  vec00 * tmp[0] +  vec01 * tmp[1] +  vec12 * tmp[2] +  vec13 * tmp[3] +  vec14 * tmp[4] +  vec15 * tmp[5] +  vec06 * tmp[6] +  vec17 * tmp[7] + 
    0;
     tmp =
        gradVecs +
        8 * hash8( posf0 + 0,  posf1 + 0,  posf2 + 1,  posf3 + 1,  posf4 + 1,  posf5 + 1,  posf6 + 1,  posf7 + 0, 
                              i, numGradVecs);
    float result00111110 =  vec00 * tmp[0] +  vec01 * tmp[1] +  vec12 * tmp[2] +  vec13 * tmp[3] +  vec14 * tmp[4] +  vec15 * tmp[5] +  vec16 * tmp[6] +  vec07 * tmp[7] + 
    0;
     tmp =
        gradVecs +
        8 * hash8( posf0 + 0,  posf1 + 0,  posf2 + 1,  posf3 + 1,  posf4 + 1,  posf5 + 1,  posf6 + 1,  posf7 + 1, 
                              i, numGradVecs);
    float result00111111 =  vec00 * tmp[0] +  vec01 * tmp[1] +  vec12 * tmp[2] +  vec13 * tmp[3] +  vec14 * tmp[4] +  vec15 * tmp[5] +  vec16 * tmp[6] +  vec17 * tmp[7] + 
    0;
     tmp =
        gradVecs +
        8 * hash8( posf0 + 0,  posf1 + 1,  posf2 + 0,  posf3 + 0,  posf4 + 0,  posf5 + 0,  posf6 + 0,  posf7 + 0, 
                              i, numGradVecs);
    float result01000000 =  vec00 * tmp[0] +  vec11 * tmp[1] +  vec02 * tmp[2] +  vec03 * tmp[3] +  vec04 * tmp[4] +  vec05 * tmp[5] +  vec06 * tmp[6] +  vec07 * tmp[7] + 
    0;
     tmp =
        gradVecs +
        8 * hash8( posf0 + 0,  posf1 + 1,  posf2 + 0,  posf3 + 0,  posf4 + 0,  posf5 + 0,  posf6 + 0,  posf7 + 1, 
                              i, numGradVecs);
    float result01000001 =  vec00 * tmp[0] +  vec11 * tmp[1] +  vec02 * tmp[2] +  vec03 * tmp[3] +  vec04 * tmp[4] +  vec05 * tmp[5] +  vec06 * tmp[6] +  vec17 * tmp[7] + 
    0;
     tmp =
        gradVecs +
        8 * hash8( posf0 + 0,  posf1 + 1,  posf2 + 0,  posf3 + 0,  posf4 + 0,  posf5 + 0,  posf6 + 1,  posf7 + 0, 
                              i, numGradVecs);
    float result01000010 =  vec00 * tmp[0] +  vec11 * tmp[1] +  vec02 * tmp[2] +  vec03 * tmp[3] +  vec04 * tmp[4] +  vec05 * tmp[5] +  vec16 * tmp[6] +  vec07 * tmp[7] + 
    0;
     tmp =
        gradVecs +
        8 * hash8( posf0 + 0,  posf1 + 1,  posf2 + 0,  posf3 + 0,  posf4 + 0,  posf5 + 0,  posf6 + 1,  posf7 + 1, 
                              i, numGradVecs);
    float result01000011 =  vec00 * tmp[0] +  vec11 * tmp[1] +  vec02 * tmp[2] +  vec03 * tmp[3] +  vec04 * tmp[4] +  vec05 * tmp[5] +  vec16 * tmp[6] +  vec17 * tmp[7] + 
    0;
     tmp =
        gradVecs +
        8 * hash8( posf0 + 0,  posf1 + 1,  posf2 + 0,  posf3 + 0,  posf4 + 0,  posf5 + 1,  posf6 + 0,  posf7 + 0, 
                              i, numGradVecs);
    float result01000100 =  vec00 * tmp[0] +  vec11 * tmp[1] +  vec02 * tmp[2] +  vec03 * tmp[3] +  vec04 * tmp[4] +  vec15 * tmp[5] +  vec06 * tmp[6] +  vec07 * tmp[7] + 
    0;
     tmp =
        gradVecs +
        8 * hash8( posf0 + 0,  posf1 + 1,  posf2 + 0,  posf3 + 0,  posf4 + 0,  posf5 + 1,  posf6 + 0,  posf7 + 1, 
                              i, numGradVecs);
    float result01000101 =  vec00 * tmp[0] +  vec11 * tmp[1] +  vec02 * tmp[2] +  vec03 * tmp[3] +  vec04 * tmp[4] +  vec15 * tmp[5] +  vec06 * tmp[6] +  vec17 * tmp[7] + 
    0;
     tmp =
        gradVecs +
        8 * hash8( posf0 + 0,  posf1 + 1,  posf2 + 0,  posf3 + 0,  posf4 + 0,  posf5 + 1,  posf6 + 1,  posf7 + 0, 
                              i, numGradVecs);
    float result01000110 =  vec00 * tmp[0] +  vec11 * tmp[1] +  vec02 * tmp[2] +  vec03 * tmp[3] +  vec04 * tmp[4] +  vec15 * tmp[5] +  vec16 * tmp[6] +  vec07 * tmp[7] + 
    0;
     tmp =
        gradVecs +
        8 * hash8( posf0 + 0,  posf1 + 1,  posf2 + 0,  posf3 + 0,  posf4 + 0,  posf5 + 1,  posf6 + 1,  posf7 + 1, 
                              i, numGradVecs);
    float result01000111 =  vec00 * tmp[0] +  vec11 * tmp[1] +  vec02 * tmp[2] +  vec03 * tmp[3] +  vec04 * tmp[4] +  vec15 * tmp[5] +  vec16 * tmp[6] +  vec17 * tmp[7] + 
    0;
     tmp =
        gradVecs +
        8 * hash8( posf0 + 0,  posf1 + 1,  posf2 + 0,  posf3 + 0,  posf4 + 1,  posf5 + 0,  posf6 + 0,  posf7 + 0, 
                              i, numGradVecs);
    float result01001000 =  vec00 * tmp[0] +  vec11 * tmp[1] +  vec02 * tmp[2] +  vec03 * tmp[3] +  vec14 * tmp[4] +  vec05 * tmp[5] +  vec06 * tmp[6] +  vec07 * tmp[7] + 
    0;
     tmp =
        gradVecs +
        8 * hash8( posf0 + 0,  posf1 + 1,  posf2 + 0,  posf3 + 0,  posf4 + 1,  posf5 + 0,  posf6 + 0,  posf7 + 1, 
                              i, numGradVecs);
    float result01001001 =  vec00 * tmp[0] +  vec11 * tmp[1] +  vec02 * tmp[2] +  vec03 * tmp[3] +  vec14 * tmp[4] +  vec05 * tmp[5] +  vec06 * tmp[6] +  vec17 * tmp[7] + 
    0;
     tmp =
        gradVecs +
        8 * hash8( posf0 + 0,  posf1 + 1,  posf2 + 0,  posf3 + 0,  posf4 + 1,  posf5 + 0,  posf6 + 1,  posf7 + 0, 
                              i, numGradVecs);
    float result01001010 =  vec00 * tmp[0] +  vec11 * tmp[1] +  vec02 * tmp[2] +  vec03 * tmp[3] +  vec14 * tmp[4] +  vec05 * tmp[5] +  vec16 * tmp[6] +  vec07 * tmp[7] + 
    0;
     tmp =
        gradVecs +
        8 * hash8( posf0 + 0,  posf1 + 1,  posf2 + 0,  posf3 + 0,  posf4 + 1,  posf5 + 0,  posf6 + 1,  posf7 + 1, 
                              i, numGradVecs);
    float result01001011 =  vec00 * tmp[0] +  vec11 * tmp[1] +  vec02 * tmp[2] +  vec03 * tmp[3] +  vec14 * tmp[4] +  vec05 * tmp[5] +  vec16 * tmp[6] +  vec17 * tmp[7] + 
    0;
     tmp =
        gradVecs +
        8 * hash8( posf0 + 0,  posf1 + 1,  posf2 + 0,  posf3 + 0,  posf4 + 1,  posf5 + 1,  posf6 + 0,  posf7 + 0, 
                              i, numGradVecs);
    float result01001100 =  vec00 * tmp[0] +  vec11 * tmp[1] +  vec02 * tmp[2] +  vec03 * tmp[3] +  vec14 * tmp[4] +  vec15 * tmp[5] +  vec06 * tmp[6] +  vec07 * tmp[7] + 
    0;
     tmp =
        gradVecs +
        8 * hash8( posf0 + 0,  posf1 + 1,  posf2 + 0,  posf3 + 0,  posf4 + 1,  posf5 + 1,  posf6 + 0,  posf7 + 1, 
                              i, numGradVecs);
    float result01001101 =  vec00 * tmp[0] +  vec11 * tmp[1] +  vec02 * tmp[2] +  vec03 * tmp[3] +  vec14 * tmp[4] +  vec15 * tmp[5] +  vec06 * tmp[6] +  vec17 * tmp[7] + 
    0;
     tmp =
        gradVecs +
        8 * hash8( posf0 + 0,  posf1 + 1,  posf2 + 0,  posf3 + 0,  posf4 + 1,  posf5 + 1,  posf6 + 1,  posf7 + 0, 
                              i, numGradVecs);
    float result01001110 =  vec00 * tmp[0] +  vec11 * tmp[1] +  vec02 * tmp[2] +  vec03 * tmp[3] +  vec14 * tmp[4] +  vec15 * tmp[5] +  vec16 * tmp[6] +  vec07 * tmp[7] + 
    0;
     tmp =
        gradVecs +
        8 * hash8( posf0 + 0,  posf1 + 1,  posf2 + 0,  posf3 + 0,  posf4 + 1,  posf5 + 1,  posf6 + 1,  posf7 + 1, 
                              i, numGradVecs);
    float result01001111 =  vec00 * tmp[0] +  vec11 * tmp[1] +  vec02 * tmp[2] +  vec03 * tmp[3] +  vec14 * tmp[4] +  vec15 * tmp[5] +  vec16 * tmp[6] +  vec17 * tmp[7] + 
    0;
     tmp =
        gradVecs +
        8 * hash8( posf0 + 0,  posf1 + 1,  posf2 + 0,  posf3 + 1,  posf4 + 0,  posf5 + 0,  posf6 + 0,  posf7 + 0, 
                              i, numGradVecs);
    float result01010000 =  vec00 * tmp[0] +  vec11 * tmp[1] +  vec02 * tmp[2] +  vec13 * tmp[3] +  vec04 * tmp[4] +  vec05 * tmp[5] +  vec06 * tmp[6] +  vec07 * tmp[7] + 
    0;
     tmp =
        gradVecs +
        8 * hash8( posf0 + 0,  posf1 + 1,  posf2 + 0,  posf3 + 1,  posf4 + 0,  posf5 + 0,  posf6 + 0,  posf7 + 1, 
                              i, numGradVecs);
    float result01010001 =  vec00 * tmp[0] +  vec11 * tmp[1] +  vec02 * tmp[2] +  vec13 * tmp[3] +  vec04 * tmp[4] +  vec05 * tmp[5] +  vec06 * tmp[6] +  vec17 * tmp[7] + 
    0;
     tmp =
        gradVecs +
        8 * hash8( posf0 + 0,  posf1 + 1,  posf2 + 0,  posf3 + 1,  posf4 + 0,  posf5 + 0,  posf6 + 1,  posf7 + 0, 
                              i, numGradVecs);
    float result01010010 =  vec00 * tmp[0] +  vec11 * tmp[1] +  vec02 * tmp[2] +  vec13 * tmp[3] +  vec04 * tmp[4] +  vec05 * tmp[5] +  vec16 * tmp[6] +  vec07 * tmp[7] + 
    0;
     tmp =
        gradVecs +
        8 * hash8( posf0 + 0,  posf1 + 1,  posf2 + 0,  posf3 + 1,  posf4 + 0,  posf5 + 0,  posf6 + 1,  posf7 + 1, 
                              i, numGradVecs);
    float result01010011 =  vec00 * tmp[0] +  vec11 * tmp[1] +  vec02 * tmp[2] +  vec13 * tmp[3] +  vec04 * tmp[4] +  vec05 * tmp[5] +  vec16 * tmp[6] +  vec17 * tmp[7] + 
    0;
     tmp =
        gradVecs +
        8 * hash8( posf0 + 0,  posf1 + 1,  posf2 + 0,  posf3 + 1,  posf4 + 0,  posf5 + 1,  posf6 + 0,  posf7 + 0, 
                              i, numGradVecs);
    float result01010100 =  vec00 * tmp[0] +  vec11 * tmp[1] +  vec02 * tmp[2] +  vec13 * tmp[3] +  vec04 * tmp[4] +  vec15 * tmp[5] +  vec06 * tmp[6] +  vec07 * tmp[7] + 
    0;
     tmp =
        gradVecs +
        8 * hash8( posf0 + 0,  posf1 + 1,  posf2 + 0,  posf3 + 1,  posf4 + 0,  posf5 + 1,  posf6 + 0,  posf7 + 1, 
                              i, numGradVecs);
    float result01010101 =  vec00 * tmp[0] +  vec11 * tmp[1] +  vec02 * tmp[2] +  vec13 * tmp[3] +  vec04 * tmp[4] +  vec15 * tmp[5] +  vec06 * tmp[6] +  vec17 * tmp[7] + 
    0;
     tmp =
        gradVecs +
        8 * hash8( posf0 + 0,  posf1 + 1,  posf2 + 0,  posf3 + 1,  posf4 + 0,  posf5 + 1,  posf6 + 1,  posf7 + 0, 
                              i, numGradVecs);
    float result01010110 =  vec00 * tmp[0] +  vec11 * tmp[1] +  vec02 * tmp[2] +  vec13 * tmp[3] +  vec04 * tmp[4] +  vec15 * tmp[5] +  vec16 * tmp[6] +  vec07 * tmp[7] + 
    0;
     tmp =
        gradVecs +
        8 * hash8( posf0 + 0,  posf1 + 1,  posf2 + 0,  posf3 + 1,  posf4 + 0,  posf5 + 1,  posf6 + 1,  posf7 + 1, 
                              i, numGradVecs);
    float result01010111 =  vec00 * tmp[0] +  vec11 * tmp[1] +  vec02 * tmp[2] +  vec13 * tmp[3] +  vec04 * tmp[4] +  vec15 * tmp[5] +  vec16 * tmp[6] +  vec17 * tmp[7] + 
    0;
     tmp =
        gradVecs +
        8 * hash8( posf0 + 0,  posf1 + 1,  posf2 + 0,  posf3 + 1,  posf4 + 1,  posf5 + 0,  posf6 + 0,  posf7 + 0, 
                              i, numGradVecs);
    float result01011000 =  vec00 * tmp[0] +  vec11 * tmp[1] +  vec02 * tmp[2] +  vec13 * tmp[3] +  vec14 * tmp[4] +  vec05 * tmp[5] +  vec06 * tmp[6] +  vec07 * tmp[7] + 
    0;
     tmp =
        gradVecs +
        8 * hash8( posf0 + 0,  posf1 + 1,  posf2 + 0,  posf3 + 1,  posf4 + 1,  posf5 + 0,  posf6 + 0,  posf7 + 1, 
                              i, numGradVecs);
    float result01011001 =  vec00 * tmp[0] +  vec11 * tmp[1] +  vec02 * tmp[2] +  vec13 * tmp[3] +  vec14 * tmp[4] +  vec05 * tmp[5] +  vec06 * tmp[6] +  vec17 * tmp[7] + 
    0;
     tmp =
        gradVecs +
        8 * hash8( posf0 + 0,  posf1 + 1,  posf2 + 0,  posf3 + 1,  posf4 + 1,  posf5 + 0,  posf6 + 1,  posf7 + 0, 
                              i, numGradVecs);
    float result01011010 =  vec00 * tmp[0] +  vec11 * tmp[1] +  vec02 * tmp[2] +  vec13 * tmp[3] +  vec14 * tmp[4] +  vec05 * tmp[5] +  vec16 * tmp[6] +  vec07 * tmp[7] + 
    0;
     tmp =
        gradVecs +
        8 * hash8( posf0 + 0,  posf1 + 1,  posf2 + 0,  posf3 + 1,  posf4 + 1,  posf5 + 0,  posf6 + 1,  posf7 + 1, 
                              i, numGradVecs);
    float result01011011 =  vec00 * tmp[0] +  vec11 * tmp[1] +  vec02 * tmp[2] +  vec13 * tmp[3] +  vec14 * tmp[4] +  vec05 * tmp[5] +  vec16 * tmp[6] +  vec17 * tmp[7] + 
    0;
     tmp =
        gradVecs +
        8 * hash8( posf0 + 0,  posf1 + 1,  posf2 + 0,  posf3 + 1,  posf4 + 1,  posf5 + 1,  posf6 + 0,  posf7 + 0, 
                              i, numGradVecs);
    float result01011100 =  vec00 * tmp[0] +  vec11 * tmp[1] +  vec02 * tmp[2] +  vec13 * tmp[3] +  vec14 * tmp[4] +  vec15 * tmp[5] +  vec06 * tmp[6] +  vec07 * tmp[7] + 
    0;
     tmp =
        gradVecs +
        8 * hash8( posf0 + 0,  posf1 + 1,  posf2 + 0,  posf3 + 1,  posf4 + 1,  posf5 + 1,  posf6 + 0,  posf7 + 1, 
                              i, numGradVecs);
    float result01011101 =  vec00 * tmp[0] +  vec11 * tmp[1] +  vec02 * tmp[2] +  vec13 * tmp[3] +  vec14 * tmp[4] +  vec15 * tmp[5] +  vec06 * tmp[6] +  vec17 * tmp[7] + 
    0;
     tmp =
        gradVecs +
        8 * hash8( posf0 + 0,  posf1 + 1,  posf2 + 0,  posf3 + 1,  posf4 + 1,  posf5 + 1,  posf6 + 1,  posf7 + 0, 
                              i, numGradVecs);
    float result01011110 =  vec00 * tmp[0] +  vec11 * tmp[1] +  vec02 * tmp[2] +  vec13 * tmp[3] +  vec14 * tmp[4] +  vec15 * tmp[5] +  vec16 * tmp[6] +  vec07 * tmp[7] + 
    0;
     tmp =
        gradVecs +
        8 * hash8( posf0 + 0,  posf1 + 1,  posf2 + 0,  posf3 + 1,  posf4 + 1,  posf5 + 1,  posf6 + 1,  posf7 + 1, 
                              i, numGradVecs);
    float result01011111 =  vec00 * tmp[0] +  vec11 * tmp[1] +  vec02 * tmp[2] +  vec13 * tmp[3] +  vec14 * tmp[4] +  vec15 * tmp[5] +  vec16 * tmp[6] +  vec17 * tmp[7] + 
    0;
     tmp =
        gradVecs +
        8 * hash8( posf0 + 0,  posf1 + 1,  posf2 + 1,  posf3 + 0,  posf4 + 0,  posf5 + 0,  posf6 + 0,  posf7 + 0, 
                              i, numGradVecs);
    float result01100000 =  vec00 * tmp[0] +  vec11 * tmp[1] +  vec12 * tmp[2] +  vec03 * tmp[3] +  vec04 * tmp[4] +  vec05 * tmp[5] +  vec06 * tmp[6] +  vec07 * tmp[7] + 
    0;
     tmp =
        gradVecs +
        8 * hash8( posf0 + 0,  posf1 + 1,  posf2 + 1,  posf3 + 0,  posf4 + 0,  posf5 + 0,  posf6 + 0,  posf7 + 1, 
                              i, numGradVecs);
    float result01100001 =  vec00 * tmp[0] +  vec11 * tmp[1] +  vec12 * tmp[2] +  vec03 * tmp[3] +  vec04 * tmp[4] +  vec05 * tmp[5] +  vec06 * tmp[6] +  vec17 * tmp[7] + 
    0;
     tmp =
        gradVecs +
        8 * hash8( posf0 + 0,  posf1 + 1,  posf2 + 1,  posf3 + 0,  posf4 + 0,  posf5 + 0,  posf6 + 1,  posf7 + 0, 
                              i, numGradVecs);
    float result01100010 =  vec00 * tmp[0] +  vec11 * tmp[1] +  vec12 * tmp[2] +  vec03 * tmp[3] +  vec04 * tmp[4] +  vec05 * tmp[5] +  vec16 * tmp[6] +  vec07 * tmp[7] + 
    0;
     tmp =
        gradVecs +
        8 * hash8( posf0 + 0,  posf1 + 1,  posf2 + 1,  posf3 + 0,  posf4 + 0,  posf5 + 0,  posf6 + 1,  posf7 + 1, 
                              i, numGradVecs);
    float result01100011 =  vec00 * tmp[0] +  vec11 * tmp[1] +  vec12 * tmp[2] +  vec03 * tmp[3] +  vec04 * tmp[4] +  vec05 * tmp[5] +  vec16 * tmp[6] +  vec17 * tmp[7] + 
    0;
     tmp =
        gradVecs +
        8 * hash8( posf0 + 0,  posf1 + 1,  posf2 + 1,  posf3 + 0,  posf4 + 0,  posf5 + 1,  posf6 + 0,  posf7 + 0, 
                              i, numGradVecs);
    float result01100100 =  vec00 * tmp[0] +  vec11 * tmp[1] +  vec12 * tmp[2] +  vec03 * tmp[3] +  vec04 * tmp[4] +  vec15 * tmp[5] +  vec06 * tmp[6] +  vec07 * tmp[7] + 
    0;
     tmp =
        gradVecs +
        8 * hash8( posf0 + 0,  posf1 + 1,  posf2 + 1,  posf3 + 0,  posf4 + 0,  posf5 + 1,  posf6 + 0,  posf7 + 1, 
                              i, numGradVecs);
    float result01100101 =  vec00 * tmp[0] +  vec11 * tmp[1] +  vec12 * tmp[2] +  vec03 * tmp[3] +  vec04 * tmp[4] +  vec15 * tmp[5] +  vec06 * tmp[6] +  vec17 * tmp[7] + 
    0;
     tmp =
        gradVecs +
        8 * hash8( posf0 + 0,  posf1 + 1,  posf2 + 1,  posf3 + 0,  posf4 + 0,  posf5 + 1,  posf6 + 1,  posf7 + 0, 
                              i, numGradVecs);
    float result01100110 =  vec00 * tmp[0] +  vec11 * tmp[1] +  vec12 * tmp[2] +  vec03 * tmp[3] +  vec04 * tmp[4] +  vec15 * tmp[5] +  vec16 * tmp[6] +  vec07 * tmp[7] + 
    0;
     tmp =
        gradVecs +
        8 * hash8( posf0 + 0,  posf1 + 1,  posf2 + 1,  posf3 + 0,  posf4 + 0,  posf5 + 1,  posf6 + 1,  posf7 + 1, 
                              i, numGradVecs);
    float result01100111 =  vec00 * tmp[0] +  vec11 * tmp[1] +  vec12 * tmp[2] +  vec03 * tmp[3] +  vec04 * tmp[4] +  vec15 * tmp[5] +  vec16 * tmp[6] +  vec17 * tmp[7] + 
    0;
     tmp =
        gradVecs +
        8 * hash8( posf0 + 0,  posf1 + 1,  posf2 + 1,  posf3 + 0,  posf4 + 1,  posf5 + 0,  posf6 + 0,  posf7 + 0, 
                              i, numGradVecs);
    float result01101000 =  vec00 * tmp[0] +  vec11 * tmp[1] +  vec12 * tmp[2] +  vec03 * tmp[3] +  vec14 * tmp[4] +  vec05 * tmp[5] +  vec06 * tmp[6] +  vec07 * tmp[7] + 
    0;
     tmp =
        gradVecs +
        8 * hash8( posf0 + 0,  posf1 + 1,  posf2 + 1,  posf3 + 0,  posf4 + 1,  posf5 + 0,  posf6 + 0,  posf7 + 1, 
                              i, numGradVecs);
    float result01101001 =  vec00 * tmp[0] +  vec11 * tmp[1] +  vec12 * tmp[2] +  vec03 * tmp[3] +  vec14 * tmp[4] +  vec05 * tmp[5] +  vec06 * tmp[6] +  vec17 * tmp[7] + 
    0;
     tmp =
        gradVecs +
        8 * hash8( posf0 + 0,  posf1 + 1,  posf2 + 1,  posf3 + 0,  posf4 + 1,  posf5 + 0,  posf6 + 1,  posf7 + 0, 
                              i, numGradVecs);
    float result01101010 =  vec00 * tmp[0] +  vec11 * tmp[1] +  vec12 * tmp[2] +  vec03 * tmp[3] +  vec14 * tmp[4] +  vec05 * tmp[5] +  vec16 * tmp[6] +  vec07 * tmp[7] + 
    0;
     tmp =
        gradVecs +
        8 * hash8( posf0 + 0,  posf1 + 1,  posf2 + 1,  posf3 + 0,  posf4 + 1,  posf5 + 0,  posf6 + 1,  posf7 + 1, 
                              i, numGradVecs);
    float result01101011 =  vec00 * tmp[0] +  vec11 * tmp[1] +  vec12 * tmp[2] +  vec03 * tmp[3] +  vec14 * tmp[4] +  vec05 * tmp[5] +  vec16 * tmp[6] +  vec17 * tmp[7] + 
    0;
     tmp =
        gradVecs +
        8 * hash8( posf0 + 0,  posf1 + 1,  posf2 + 1,  posf3 + 0,  posf4 + 1,  posf5 + 1,  posf6 + 0,  posf7 + 0, 
                              i, numGradVecs);
    float result01101100 =  vec00 * tmp[0] +  vec11 * tmp[1] +  vec12 * tmp[2] +  vec03 * tmp[3] +  vec14 * tmp[4] +  vec15 * tmp[5] +  vec06 * tmp[6] +  vec07 * tmp[7] + 
    0;
     tmp =
        gradVecs +
        8 * hash8( posf0 + 0,  posf1 + 1,  posf2 + 1,  posf3 + 0,  posf4 + 1,  posf5 + 1,  posf6 + 0,  posf7 + 1, 
                              i, numGradVecs);
    float result01101101 =  vec00 * tmp[0] +  vec11 * tmp[1] +  vec12 * tmp[2] +  vec03 * tmp[3] +  vec14 * tmp[4] +  vec15 * tmp[5] +  vec06 * tmp[6] +  vec17 * tmp[7] + 
    0;
     tmp =
        gradVecs +
        8 * hash8( posf0 + 0,  posf1 + 1,  posf2 + 1,  posf3 + 0,  posf4 + 1,  posf5 + 1,  posf6 + 1,  posf7 + 0, 
                              i, numGradVecs);
    float result01101110 =  vec00 * tmp[0] +  vec11 * tmp[1] +  vec12 * tmp[2] +  vec03 * tmp[3] +  vec14 * tmp[4] +  vec15 * tmp[5] +  vec16 * tmp[6] +  vec07 * tmp[7] + 
    0;
     tmp =
        gradVecs +
        8 * hash8( posf0 + 0,  posf1 + 1,  posf2 + 1,  posf3 + 0,  posf4 + 1,  posf5 + 1,  posf6 + 1,  posf7 + 1, 
                              i, numGradVecs);
    float result01101111 =  vec00 * tmp[0] +  vec11 * tmp[1] +  vec12 * tmp[2] +  vec03 * tmp[3] +  vec14 * tmp[4] +  vec15 * tmp[5] +  vec16 * tmp[6] +  vec17 * tmp[7] + 
    0;
     tmp =
        gradVecs +
        8 * hash8( posf0 + 0,  posf1 + 1,  posf2 + 1,  posf3 + 1,  posf4 + 0,  posf5 + 0,  posf6 + 0,  posf7 + 0, 
                              i, numGradVecs);
    float result01110000 =  vec00 * tmp[0] +  vec11 * tmp[1] +  vec12 * tmp[2] +  vec13 * tmp[3] +  vec04 * tmp[4] +  vec05 * tmp[5] +  vec06 * tmp[6] +  vec07 * tmp[7] + 
    0;
     tmp =
        gradVecs +
        8 * hash8( posf0 + 0,  posf1 + 1,  posf2 + 1,  posf3 + 1,  posf4 + 0,  posf5 + 0,  posf6 + 0,  posf7 + 1, 
                              i, numGradVecs);
    float result01110001 =  vec00 * tmp[0] +  vec11 * tmp[1] +  vec12 * tmp[2] +  vec13 * tmp[3] +  vec04 * tmp[4] +  vec05 * tmp[5] +  vec06 * tmp[6] +  vec17 * tmp[7] + 
    0;
     tmp =
        gradVecs +
        8 * hash8( posf0 + 0,  posf1 + 1,  posf2 + 1,  posf3 + 1,  posf4 + 0,  posf5 + 0,  posf6 + 1,  posf7 + 0, 
                              i, numGradVecs);
    float result01110010 =  vec00 * tmp[0] +  vec11 * tmp[1] +  vec12 * tmp[2] +  vec13 * tmp[3] +  vec04 * tmp[4] +  vec05 * tmp[5] +  vec16 * tmp[6] +  vec07 * tmp[7] + 
    0;
     tmp =
        gradVecs +
        8 * hash8( posf0 + 0,  posf1 + 1,  posf2 + 1,  posf3 + 1,  posf4 + 0,  posf5 + 0,  posf6 + 1,  posf7 + 1, 
                              i, numGradVecs);
    float result01110011 =  vec00 * tmp[0] +  vec11 * tmp[1] +  vec12 * tmp[2] +  vec13 * tmp[3] +  vec04 * tmp[4] +  vec05 * tmp[5] +  vec16 * tmp[6] +  vec17 * tmp[7] + 
    0;
     tmp =
        gradVecs +
        8 * hash8( posf0 + 0,  posf1 + 1,  posf2 + 1,  posf3 + 1,  posf4 + 0,  posf5 + 1,  posf6 + 0,  posf7 + 0, 
                              i, numGradVecs);
    float result01110100 =  vec00 * tmp[0] +  vec11 * tmp[1] +  vec12 * tmp[2] +  vec13 * tmp[3] +  vec04 * tmp[4] +  vec15 * tmp[5] +  vec06 * tmp[6] +  vec07 * tmp[7] + 
    0;
     tmp =
        gradVecs +
        8 * hash8( posf0 + 0,  posf1 + 1,  posf2 + 1,  posf3 + 1,  posf4 + 0,  posf5 + 1,  posf6 + 0,  posf7 + 1, 
                              i, numGradVecs);
    float result01110101 =  vec00 * tmp[0] +  vec11 * tmp[1] +  vec12 * tmp[2] +  vec13 * tmp[3] +  vec04 * tmp[4] +  vec15 * tmp[5] +  vec06 * tmp[6] +  vec17 * tmp[7] + 
    0;
     tmp =
        gradVecs +
        8 * hash8( posf0 + 0,  posf1 + 1,  posf2 + 1,  posf3 + 1,  posf4 + 0,  posf5 + 1,  posf6 + 1,  posf7 + 0, 
                              i, numGradVecs);
    float result01110110 =  vec00 * tmp[0] +  vec11 * tmp[1] +  vec12 * tmp[2] +  vec13 * tmp[3] +  vec04 * tmp[4] +  vec15 * tmp[5] +  vec16 * tmp[6] +  vec07 * tmp[7] + 
    0;
     tmp =
        gradVecs +
        8 * hash8( posf0 + 0,  posf1 + 1,  posf2 + 1,  posf3 + 1,  posf4 + 0,  posf5 + 1,  posf6 + 1,  posf7 + 1, 
                              i, numGradVecs);
    float result01110111 =  vec00 * tmp[0] +  vec11 * tmp[1] +  vec12 * tmp[2] +  vec13 * tmp[3] +  vec04 * tmp[4] +  vec15 * tmp[5] +  vec16 * tmp[6] +  vec17 * tmp[7] + 
    0;
     tmp =
        gradVecs +
        8 * hash8( posf0 + 0,  posf1 + 1,  posf2 + 1,  posf3 + 1,  posf4 + 1,  posf5 + 0,  posf6 + 0,  posf7 + 0, 
                              i, numGradVecs);
    float result01111000 =  vec00 * tmp[0] +  vec11 * tmp[1] +  vec12 * tmp[2] +  vec13 * tmp[3] +  vec14 * tmp[4] +  vec05 * tmp[5] +  vec06 * tmp[6] +  vec07 * tmp[7] + 
    0;
     tmp =
        gradVecs +
        8 * hash8( posf0 + 0,  posf1 + 1,  posf2 + 1,  posf3 + 1,  posf4 + 1,  posf5 + 0,  posf6 + 0,  posf7 + 1, 
                              i, numGradVecs);
    float result01111001 =  vec00 * tmp[0] +  vec11 * tmp[1] +  vec12 * tmp[2] +  vec13 * tmp[3] +  vec14 * tmp[4] +  vec05 * tmp[5] +  vec06 * tmp[6] +  vec17 * tmp[7] + 
    0;
     tmp =
        gradVecs +
        8 * hash8( posf0 + 0,  posf1 + 1,  posf2 + 1,  posf3 + 1,  posf4 + 1,  posf5 + 0,  posf6 + 1,  posf7 + 0, 
                              i, numGradVecs);
    float result01111010 =  vec00 * tmp[0] +  vec11 * tmp[1] +  vec12 * tmp[2] +  vec13 * tmp[3] +  vec14 * tmp[4] +  vec05 * tmp[5] +  vec16 * tmp[6] +  vec07 * tmp[7] + 
    0;
     tmp =
        gradVecs +
        8 * hash8( posf0 + 0,  posf1 + 1,  posf2 + 1,  posf3 + 1,  posf4 + 1,  posf5 + 0,  posf6 + 1,  posf7 + 1, 
                              i, numGradVecs);
    float result01111011 =  vec00 * tmp[0] +  vec11 * tmp[1] +  vec12 * tmp[2] +  vec13 * tmp[3] +  vec14 * tmp[4] +  vec05 * tmp[5] +  vec16 * tmp[6] +  vec17 * tmp[7] + 
    0;
     tmp =
        gradVecs +
        8 * hash8( posf0 + 0,  posf1 + 1,  posf2 + 1,  posf3 + 1,  posf4 + 1,  posf5 + 1,  posf6 + 0,  posf7 + 0, 
                              i, numGradVecs);
    float result01111100 =  vec00 * tmp[0] +  vec11 * tmp[1] +  vec12 * tmp[2] +  vec13 * tmp[3] +  vec14 * tmp[4] +  vec15 * tmp[5] +  vec06 * tmp[6] +  vec07 * tmp[7] + 
    0;
     tmp =
        gradVecs +
        8 * hash8( posf0 + 0,  posf1 + 1,  posf2 + 1,  posf3 + 1,  posf4 + 1,  posf5 + 1,  posf6 + 0,  posf7 + 1, 
                              i, numGradVecs);
    float result01111101 =  vec00 * tmp[0] +  vec11 * tmp[1] +  vec12 * tmp[2] +  vec13 * tmp[3] +  vec14 * tmp[4] +  vec15 * tmp[5] +  vec06 * tmp[6] +  vec17 * tmp[7] + 
    0;
     tmp =
        gradVecs +
        8 * hash8( posf0 + 0,  posf1 + 1,  posf2 + 1,  posf3 + 1,  posf4 + 1,  posf5 + 1,  posf6 + 1,  posf7 + 0, 
                              i, numGradVecs);
    float result01111110 =  vec00 * tmp[0] +  vec11 * tmp[1] +  vec12 * tmp[2] +  vec13 * tmp[3] +  vec14 * tmp[4] +  vec15 * tmp[5] +  vec16 * tmp[6] +  vec07 * tmp[7] + 
    0;
     tmp =
        gradVecs +
        8 * hash8( posf0 + 0,  posf1 + 1,  posf2 + 1,  posf3 + 1,  posf4 + 1,  posf5 + 1,  posf6 + 1,  posf7 + 1, 
                              i, numGradVecs);
    float result01111111 =  vec00 * tmp[0] +  vec11 * tmp[1] +  vec12 * tmp[2] +  vec13 * tmp[3] +  vec14 * tmp[4] +  vec15 * tmp[5] +  vec16 * tmp[6] +  vec17 * tmp[7] + 
    0;
     tmp =
        gradVecs +
        8 * hash8( posf0 + 1,  posf1 + 0,  posf2 + 0,  posf3 + 0,  posf4 + 0,  posf5 + 0,  posf6 + 0,  posf7 + 0, 
                              i, numGradVecs);
    float result10000000 =  vec10 * tmp[0] +  vec01 * tmp[1] +  vec02 * tmp[2] +  vec03 * tmp[3] +  vec04 * tmp[4] +  vec05 * tmp[5] +  vec06 * tmp[6] +  vec07 * tmp[7] + 
    0;
     tmp =
        gradVecs +
        8 * hash8( posf0 + 1,  posf1 + 0,  posf2 + 0,  posf3 + 0,  posf4 + 0,  posf5 + 0,  posf6 + 0,  posf7 + 1, 
                              i, numGradVecs);
    float result10000001 =  vec10 * tmp[0] +  vec01 * tmp[1] +  vec02 * tmp[2] +  vec03 * tmp[3] +  vec04 * tmp[4] +  vec05 * tmp[5] +  vec06 * tmp[6] +  vec17 * tmp[7] + 
    0;
     tmp =
        gradVecs +
        8 * hash8( posf0 + 1,  posf1 + 0,  posf2 + 0,  posf3 + 0,  posf4 + 0,  posf5 + 0,  posf6 + 1,  posf7 + 0, 
                              i, numGradVecs);
    float result10000010 =  vec10 * tmp[0] +  vec01 * tmp[1] +  vec02 * tmp[2] +  vec03 * tmp[3] +  vec04 * tmp[4] +  vec05 * tmp[5] +  vec16 * tmp[6] +  vec07 * tmp[7] + 
    0;
     tmp =
        gradVecs +
        8 * hash8( posf0 + 1,  posf1 + 0,  posf2 + 0,  posf3 + 0,  posf4 + 0,  posf5 + 0,  posf6 + 1,  posf7 + 1, 
                              i, numGradVecs);
    float result10000011 =  vec10 * tmp[0] +  vec01 * tmp[1] +  vec02 * tmp[2] +  vec03 * tmp[3] +  vec04 * tmp[4] +  vec05 * tmp[5] +  vec16 * tmp[6] +  vec17 * tmp[7] + 
    0;
     tmp =
        gradVecs +
        8 * hash8( posf0 + 1,  posf1 + 0,  posf2 + 0,  posf3 + 0,  posf4 + 0,  posf5 + 1,  posf6 + 0,  posf7 + 0, 
                              i, numGradVecs);
    float result10000100 =  vec10 * tmp[0] +  vec01 * tmp[1] +  vec02 * tmp[2] +  vec03 * tmp[3] +  vec04 * tmp[4] +  vec15 * tmp[5] +  vec06 * tmp[6] +  vec07 * tmp[7] + 
    0;
     tmp =
        gradVecs +
        8 * hash8( posf0 + 1,  posf1 + 0,  posf2 + 0,  posf3 + 0,  posf4 + 0,  posf5 + 1,  posf6 + 0,  posf7 + 1, 
                              i, numGradVecs);
    float result10000101 =  vec10 * tmp[0] +  vec01 * tmp[1] +  vec02 * tmp[2] +  vec03 * tmp[3] +  vec04 * tmp[4] +  vec15 * tmp[5] +  vec06 * tmp[6] +  vec17 * tmp[7] + 
    0;
     tmp =
        gradVecs +
        8 * hash8( posf0 + 1,  posf1 + 0,  posf2 + 0,  posf3 + 0,  posf4 + 0,  posf5 + 1,  posf6 + 1,  posf7 + 0, 
                              i, numGradVecs);
    float result10000110 =  vec10 * tmp[0] +  vec01 * tmp[1] +  vec02 * tmp[2] +  vec03 * tmp[3] +  vec04 * tmp[4] +  vec15 * tmp[5] +  vec16 * tmp[6] +  vec07 * tmp[7] + 
    0;
     tmp =
        gradVecs +
        8 * hash8( posf0 + 1,  posf1 + 0,  posf2 + 0,  posf3 + 0,  posf4 + 0,  posf5 + 1,  posf6 + 1,  posf7 + 1, 
                              i, numGradVecs);
    float result10000111 =  vec10 * tmp[0] +  vec01 * tmp[1] +  vec02 * tmp[2] +  vec03 * tmp[3] +  vec04 * tmp[4] +  vec15 * tmp[5] +  vec16 * tmp[6] +  vec17 * tmp[7] + 
    0;
     tmp =
        gradVecs +
        8 * hash8( posf0 + 1,  posf1 + 0,  posf2 + 0,  posf3 + 0,  posf4 + 1,  posf5 + 0,  posf6 + 0,  posf7 + 0, 
                              i, numGradVecs);
    float result10001000 =  vec10 * tmp[0] +  vec01 * tmp[1] +  vec02 * tmp[2] +  vec03 * tmp[3] +  vec14 * tmp[4] +  vec05 * tmp[5] +  vec06 * tmp[6] +  vec07 * tmp[7] + 
    0;
     tmp =
        gradVecs +
        8 * hash8( posf0 + 1,  posf1 + 0,  posf2 + 0,  posf3 + 0,  posf4 + 1,  posf5 + 0,  posf6 + 0,  posf7 + 1, 
                              i, numGradVecs);
    float result10001001 =  vec10 * tmp[0] +  vec01 * tmp[1] +  vec02 * tmp[2] +  vec03 * tmp[3] +  vec14 * tmp[4] +  vec05 * tmp[5] +  vec06 * tmp[6] +  vec17 * tmp[7] + 
    0;
     tmp =
        gradVecs +
        8 * hash8( posf0 + 1,  posf1 + 0,  posf2 + 0,  posf3 + 0,  posf4 + 1,  posf5 + 0,  posf6 + 1,  posf7 + 0, 
                              i, numGradVecs);
    float result10001010 =  vec10 * tmp[0] +  vec01 * tmp[1] +  vec02 * tmp[2] +  vec03 * tmp[3] +  vec14 * tmp[4] +  vec05 * tmp[5] +  vec16 * tmp[6] +  vec07 * tmp[7] + 
    0;
     tmp =
        gradVecs +
        8 * hash8( posf0 + 1,  posf1 + 0,  posf2 + 0,  posf3 + 0,  posf4 + 1,  posf5 + 0,  posf6 + 1,  posf7 + 1, 
                              i, numGradVecs);
    float result10001011 =  vec10 * tmp[0] +  vec01 * tmp[1] +  vec02 * tmp[2] +  vec03 * tmp[3] +  vec14 * tmp[4] +  vec05 * tmp[5] +  vec16 * tmp[6] +  vec17 * tmp[7] + 
    0;
     tmp =
        gradVecs +
        8 * hash8( posf0 + 1,  posf1 + 0,  posf2 + 0,  posf3 + 0,  posf4 + 1,  posf5 + 1,  posf6 + 0,  posf7 + 0, 
                              i, numGradVecs);
    float result10001100 =  vec10 * tmp[0] +  vec01 * tmp[1] +  vec02 * tmp[2] +  vec03 * tmp[3] +  vec14 * tmp[4] +  vec15 * tmp[5] +  vec06 * tmp[6] +  vec07 * tmp[7] + 
    0;
     tmp =
        gradVecs +
        8 * hash8( posf0 + 1,  posf1 + 0,  posf2 + 0,  posf3 + 0,  posf4 + 1,  posf5 + 1,  posf6 + 0,  posf7 + 1, 
                              i, numGradVecs);
    float result10001101 =  vec10 * tmp[0] +  vec01 * tmp[1] +  vec02 * tmp[2] +  vec03 * tmp[3] +  vec14 * tmp[4] +  vec15 * tmp[5] +  vec06 * tmp[6] +  vec17 * tmp[7] + 
    0;
     tmp =
        gradVecs +
        8 * hash8( posf0 + 1,  posf1 + 0,  posf2 + 0,  posf3 + 0,  posf4 + 1,  posf5 + 1,  posf6 + 1,  posf7 + 0, 
                              i, numGradVecs);
    float result10001110 =  vec10 * tmp[0] +  vec01 * tmp[1] +  vec02 * tmp[2] +  vec03 * tmp[3] +  vec14 * tmp[4] +  vec15 * tmp[5] +  vec16 * tmp[6] +  vec07 * tmp[7] + 
    0;
     tmp =
        gradVecs +
        8 * hash8( posf0 + 1,  posf1 + 0,  posf2 + 0,  posf3 + 0,  posf4 + 1,  posf5 + 1,  posf6 + 1,  posf7 + 1, 
                              i, numGradVecs);
    float result10001111 =  vec10 * tmp[0] +  vec01 * tmp[1] +  vec02 * tmp[2] +  vec03 * tmp[3] +  vec14 * tmp[4] +  vec15 * tmp[5] +  vec16 * tmp[6] +  vec17 * tmp[7] + 
    0;
     tmp =
        gradVecs +
        8 * hash8( posf0 + 1,  posf1 + 0,  posf2 + 0,  posf3 + 1,  posf4 + 0,  posf5 + 0,  posf6 + 0,  posf7 + 0, 
                              i, numGradVecs);
    float result10010000 =  vec10 * tmp[0] +  vec01 * tmp[1] +  vec02 * tmp[2] +  vec13 * tmp[3] +  vec04 * tmp[4] +  vec05 * tmp[5] +  vec06 * tmp[6] +  vec07 * tmp[7] + 
    0;
     tmp =
        gradVecs +
        8 * hash8( posf0 + 1,  posf1 + 0,  posf2 + 0,  posf3 + 1,  posf4 + 0,  posf5 + 0,  posf6 + 0,  posf7 + 1, 
                              i, numGradVecs);
    float result10010001 =  vec10 * tmp[0] +  vec01 * tmp[1] +  vec02 * tmp[2] +  vec13 * tmp[3] +  vec04 * tmp[4] +  vec05 * tmp[5] +  vec06 * tmp[6] +  vec17 * tmp[7] + 
    0;
     tmp =
        gradVecs +
        8 * hash8( posf0 + 1,  posf1 + 0,  posf2 + 0,  posf3 + 1,  posf4 + 0,  posf5 + 0,  posf6 + 1,  posf7 + 0, 
                              i, numGradVecs);
    float result10010010 =  vec10 * tmp[0] +  vec01 * tmp[1] +  vec02 * tmp[2] +  vec13 * tmp[3] +  vec04 * tmp[4] +  vec05 * tmp[5] +  vec16 * tmp[6] +  vec07 * tmp[7] + 
    0;
     tmp =
        gradVecs +
        8 * hash8( posf0 + 1,  posf1 + 0,  posf2 + 0,  posf3 + 1,  posf4 + 0,  posf5 + 0,  posf6 + 1,  posf7 + 1, 
                              i, numGradVecs);
    float result10010011 =  vec10 * tmp[0] +  vec01 * tmp[1] +  vec02 * tmp[2] +  vec13 * tmp[3] +  vec04 * tmp[4] +  vec05 * tmp[5] +  vec16 * tmp[6] +  vec17 * tmp[7] + 
    0;
     tmp =
        gradVecs +
        8 * hash8( posf0 + 1,  posf1 + 0,  posf2 + 0,  posf3 + 1,  posf4 + 0,  posf5 + 1,  posf6 + 0,  posf7 + 0, 
                              i, numGradVecs);
    float result10010100 =  vec10 * tmp[0] +  vec01 * tmp[1] +  vec02 * tmp[2] +  vec13 * tmp[3] +  vec04 * tmp[4] +  vec15 * tmp[5] +  vec06 * tmp[6] +  vec07 * tmp[7] + 
    0;
     tmp =
        gradVecs +
        8 * hash8( posf0 + 1,  posf1 + 0,  posf2 + 0,  posf3 + 1,  posf4 + 0,  posf5 + 1,  posf6 + 0,  posf7 + 1, 
                              i, numGradVecs);
    float result10010101 =  vec10 * tmp[0] +  vec01 * tmp[1] +  vec02 * tmp[2] +  vec13 * tmp[3] +  vec04 * tmp[4] +  vec15 * tmp[5] +  vec06 * tmp[6] +  vec17 * tmp[7] + 
    0;
     tmp =
        gradVecs +
        8 * hash8( posf0 + 1,  posf1 + 0,  posf2 + 0,  posf3 + 1,  posf4 + 0,  posf5 + 1,  posf6 + 1,  posf7 + 0, 
                              i, numGradVecs);
    float result10010110 =  vec10 * tmp[0] +  vec01 * tmp[1] +  vec02 * tmp[2] +  vec13 * tmp[3] +  vec04 * tmp[4] +  vec15 * tmp[5] +  vec16 * tmp[6] +  vec07 * tmp[7] + 
    0;
     tmp =
        gradVecs +
        8 * hash8( posf0 + 1,  posf1 + 0,  posf2 + 0,  posf3 + 1,  posf4 + 0,  posf5 + 1,  posf6 + 1,  posf7 + 1, 
                              i, numGradVecs);
    float result10010111 =  vec10 * tmp[0] +  vec01 * tmp[1] +  vec02 * tmp[2] +  vec13 * tmp[3] +  vec04 * tmp[4] +  vec15 * tmp[5] +  vec16 * tmp[6] +  vec17 * tmp[7] + 
    0;
     tmp =
        gradVecs +
        8 * hash8( posf0 + 1,  posf1 + 0,  posf2 + 0,  posf3 + 1,  posf4 + 1,  posf5 + 0,  posf6 + 0,  posf7 + 0, 
                              i, numGradVecs);
    float result10011000 =  vec10 * tmp[0] +  vec01 * tmp[1] +  vec02 * tmp[2] +  vec13 * tmp[3] +  vec14 * tmp[4] +  vec05 * tmp[5] +  vec06 * tmp[6] +  vec07 * tmp[7] + 
    0;
     tmp =
        gradVecs +
        8 * hash8( posf0 + 1,  posf1 + 0,  posf2 + 0,  posf3 + 1,  posf4 + 1,  posf5 + 0,  posf6 + 0,  posf7 + 1, 
                              i, numGradVecs);
    float result10011001 =  vec10 * tmp[0] +  vec01 * tmp[1] +  vec02 * tmp[2] +  vec13 * tmp[3] +  vec14 * tmp[4] +  vec05 * tmp[5] +  vec06 * tmp[6] +  vec17 * tmp[7] + 
    0;
     tmp =
        gradVecs +
        8 * hash8( posf0 + 1,  posf1 + 0,  posf2 + 0,  posf3 + 1,  posf4 + 1,  posf5 + 0,  posf6 + 1,  posf7 + 0, 
                              i, numGradVecs);
    float result10011010 =  vec10 * tmp[0] +  vec01 * tmp[1] +  vec02 * tmp[2] +  vec13 * tmp[3] +  vec14 * tmp[4] +  vec05 * tmp[5] +  vec16 * tmp[6] +  vec07 * tmp[7] + 
    0;
     tmp =
        gradVecs +
        8 * hash8( posf0 + 1,  posf1 + 0,  posf2 + 0,  posf3 + 1,  posf4 + 1,  posf5 + 0,  posf6 + 1,  posf7 + 1, 
                              i, numGradVecs);
    float result10011011 =  vec10 * tmp[0] +  vec01 * tmp[1] +  vec02 * tmp[2] +  vec13 * tmp[3] +  vec14 * tmp[4] +  vec05 * tmp[5] +  vec16 * tmp[6] +  vec17 * tmp[7] + 
    0;
     tmp =
        gradVecs +
        8 * hash8( posf0 + 1,  posf1 + 0,  posf2 + 0,  posf3 + 1,  posf4 + 1,  posf5 + 1,  posf6 + 0,  posf7 + 0, 
                              i, numGradVecs);
    float result10011100 =  vec10 * tmp[0] +  vec01 * tmp[1] +  vec02 * tmp[2] +  vec13 * tmp[3] +  vec14 * tmp[4] +  vec15 * tmp[5] +  vec06 * tmp[6] +  vec07 * tmp[7] + 
    0;
     tmp =
        gradVecs +
        8 * hash8( posf0 + 1,  posf1 + 0,  posf2 + 0,  posf3 + 1,  posf4 + 1,  posf5 + 1,  posf6 + 0,  posf7 + 1, 
                              i, numGradVecs);
    float result10011101 =  vec10 * tmp[0] +  vec01 * tmp[1] +  vec02 * tmp[2] +  vec13 * tmp[3] +  vec14 * tmp[4] +  vec15 * tmp[5] +  vec06 * tmp[6] +  vec17 * tmp[7] + 
    0;
     tmp =
        gradVecs +
        8 * hash8( posf0 + 1,  posf1 + 0,  posf2 + 0,  posf3 + 1,  posf4 + 1,  posf5 + 1,  posf6 + 1,  posf7 + 0, 
                              i, numGradVecs);
    float result10011110 =  vec10 * tmp[0] +  vec01 * tmp[1] +  vec02 * tmp[2] +  vec13 * tmp[3] +  vec14 * tmp[4] +  vec15 * tmp[5] +  vec16 * tmp[6] +  vec07 * tmp[7] + 
    0;
     tmp =
        gradVecs +
        8 * hash8( posf0 + 1,  posf1 + 0,  posf2 + 0,  posf3 + 1,  posf4 + 1,  posf5 + 1,  posf6 + 1,  posf7 + 1, 
                              i, numGradVecs);
    float result10011111 =  vec10 * tmp[0] +  vec01 * tmp[1] +  vec02 * tmp[2] +  vec13 * tmp[3] +  vec14 * tmp[4] +  vec15 * tmp[5] +  vec16 * tmp[6] +  vec17 * tmp[7] + 
    0;
     tmp =
        gradVecs +
        8 * hash8( posf0 + 1,  posf1 + 0,  posf2 + 1,  posf3 + 0,  posf4 + 0,  posf5 + 0,  posf6 + 0,  posf7 + 0, 
                              i, numGradVecs);
    float result10100000 =  vec10 * tmp[0] +  vec01 * tmp[1] +  vec12 * tmp[2] +  vec03 * tmp[3] +  vec04 * tmp[4] +  vec05 * tmp[5] +  vec06 * tmp[6] +  vec07 * tmp[7] + 
    0;
     tmp =
        gradVecs +
        8 * hash8( posf0 + 1,  posf1 + 0,  posf2 + 1,  posf3 + 0,  posf4 + 0,  posf5 + 0,  posf6 + 0,  posf7 + 1, 
                              i, numGradVecs);
    float result10100001 =  vec10 * tmp[0] +  vec01 * tmp[1] +  vec12 * tmp[2] +  vec03 * tmp[3] +  vec04 * tmp[4] +  vec05 * tmp[5] +  vec06 * tmp[6] +  vec17 * tmp[7] + 
    0;
     tmp =
        gradVecs +
        8 * hash8( posf0 + 1,  posf1 + 0,  posf2 + 1,  posf3 + 0,  posf4 + 0,  posf5 + 0,  posf6 + 1,  posf7 + 0, 
                              i, numGradVecs);
    float result10100010 =  vec10 * tmp[0] +  vec01 * tmp[1] +  vec12 * tmp[2] +  vec03 * tmp[3] +  vec04 * tmp[4] +  vec05 * tmp[5] +  vec16 * tmp[6] +  vec07 * tmp[7] + 
    0;
     tmp =
        gradVecs +
        8 * hash8( posf0 + 1,  posf1 + 0,  posf2 + 1,  posf3 + 0,  posf4 + 0,  posf5 + 0,  posf6 + 1,  posf7 + 1, 
                              i, numGradVecs);
    float result10100011 =  vec10 * tmp[0] +  vec01 * tmp[1] +  vec12 * tmp[2] +  vec03 * tmp[3] +  vec04 * tmp[4] +  vec05 * tmp[5] +  vec16 * tmp[6] +  vec17 * tmp[7] + 
    0;
     tmp =
        gradVecs +
        8 * hash8( posf0 + 1,  posf1 + 0,  posf2 + 1,  posf3 + 0,  posf4 + 0,  posf5 + 1,  posf6 + 0,  posf7 + 0, 
                              i, numGradVecs);
    float result10100100 =  vec10 * tmp[0] +  vec01 * tmp[1] +  vec12 * tmp[2] +  vec03 * tmp[3] +  vec04 * tmp[4] +  vec15 * tmp[5] +  vec06 * tmp[6] +  vec07 * tmp[7] + 
    0;
     tmp =
        gradVecs +
        8 * hash8( posf0 + 1,  posf1 + 0,  posf2 + 1,  posf3 + 0,  posf4 + 0,  posf5 + 1,  posf6 + 0,  posf7 + 1, 
                              i, numGradVecs);
    float result10100101 =  vec10 * tmp[0] +  vec01 * tmp[1] +  vec12 * tmp[2] +  vec03 * tmp[3] +  vec04 * tmp[4] +  vec15 * tmp[5] +  vec06 * tmp[6] +  vec17 * tmp[7] + 
    0;
     tmp =
        gradVecs +
        8 * hash8( posf0 + 1,  posf1 + 0,  posf2 + 1,  posf3 + 0,  posf4 + 0,  posf5 + 1,  posf6 + 1,  posf7 + 0, 
                              i, numGradVecs);
    float result10100110 =  vec10 * tmp[0] +  vec01 * tmp[1] +  vec12 * tmp[2] +  vec03 * tmp[3] +  vec04 * tmp[4] +  vec15 * tmp[5] +  vec16 * tmp[6] +  vec07 * tmp[7] + 
    0;
     tmp =
        gradVecs +
        8 * hash8( posf0 + 1,  posf1 + 0,  posf2 + 1,  posf3 + 0,  posf4 + 0,  posf5 + 1,  posf6 + 1,  posf7 + 1, 
                              i, numGradVecs);
    float result10100111 =  vec10 * tmp[0] +  vec01 * tmp[1] +  vec12 * tmp[2] +  vec03 * tmp[3] +  vec04 * tmp[4] +  vec15 * tmp[5] +  vec16 * tmp[6] +  vec17 * tmp[7] + 
    0;
     tmp =
        gradVecs +
        8 * hash8( posf0 + 1,  posf1 + 0,  posf2 + 1,  posf3 + 0,  posf4 + 1,  posf5 + 0,  posf6 + 0,  posf7 + 0, 
                              i, numGradVecs);
    float result10101000 =  vec10 * tmp[0] +  vec01 * tmp[1] +  vec12 * tmp[2] +  vec03 * tmp[3] +  vec14 * tmp[4] +  vec05 * tmp[5] +  vec06 * tmp[6] +  vec07 * tmp[7] + 
    0;
     tmp =
        gradVecs +
        8 * hash8( posf0 + 1,  posf1 + 0,  posf2 + 1,  posf3 + 0,  posf4 + 1,  posf5 + 0,  posf6 + 0,  posf7 + 1, 
                              i, numGradVecs);
    float result10101001 =  vec10 * tmp[0] +  vec01 * tmp[1] +  vec12 * tmp[2] +  vec03 * tmp[3] +  vec14 * tmp[4] +  vec05 * tmp[5] +  vec06 * tmp[6] +  vec17 * tmp[7] + 
    0;
     tmp =
        gradVecs +
        8 * hash8( posf0 + 1,  posf1 + 0,  posf2 + 1,  posf3 + 0,  posf4 + 1,  posf5 + 0,  posf6 + 1,  posf7 + 0, 
                              i, numGradVecs);
    float result10101010 =  vec10 * tmp[0] +  vec01 * tmp[1] +  vec12 * tmp[2] +  vec03 * tmp[3] +  vec14 * tmp[4] +  vec05 * tmp[5] +  vec16 * tmp[6] +  vec07 * tmp[7] + 
    0;
     tmp =
        gradVecs +
        8 * hash8( posf0 + 1,  posf1 + 0,  posf2 + 1,  posf3 + 0,  posf4 + 1,  posf5 + 0,  posf6 + 1,  posf7 + 1, 
                              i, numGradVecs);
    float result10101011 =  vec10 * tmp[0] +  vec01 * tmp[1] +  vec12 * tmp[2] +  vec03 * tmp[3] +  vec14 * tmp[4] +  vec05 * tmp[5] +  vec16 * tmp[6] +  vec17 * tmp[7] + 
    0;
     tmp =
        gradVecs +
        8 * hash8( posf0 + 1,  posf1 + 0,  posf2 + 1,  posf3 + 0,  posf4 + 1,  posf5 + 1,  posf6 + 0,  posf7 + 0, 
                              i, numGradVecs);
    float result10101100 =  vec10 * tmp[0] +  vec01 * tmp[1] +  vec12 * tmp[2] +  vec03 * tmp[3] +  vec14 * tmp[4] +  vec15 * tmp[5] +  vec06 * tmp[6] +  vec07 * tmp[7] + 
    0;
     tmp =
        gradVecs +
        8 * hash8( posf0 + 1,  posf1 + 0,  posf2 + 1,  posf3 + 0,  posf4 + 1,  posf5 + 1,  posf6 + 0,  posf7 + 1, 
                              i, numGradVecs);
    float result10101101 =  vec10 * tmp[0] +  vec01 * tmp[1] +  vec12 * tmp[2] +  vec03 * tmp[3] +  vec14 * tmp[4] +  vec15 * tmp[5] +  vec06 * tmp[6] +  vec17 * tmp[7] + 
    0;
     tmp =
        gradVecs +
        8 * hash8( posf0 + 1,  posf1 + 0,  posf2 + 1,  posf3 + 0,  posf4 + 1,  posf5 + 1,  posf6 + 1,  posf7 + 0, 
                              i, numGradVecs);
    float result10101110 =  vec10 * tmp[0] +  vec01 * tmp[1] +  vec12 * tmp[2] +  vec03 * tmp[3] +  vec14 * tmp[4] +  vec15 * tmp[5] +  vec16 * tmp[6] +  vec07 * tmp[7] + 
    0;
     tmp =
        gradVecs +
        8 * hash8( posf0 + 1,  posf1 + 0,  posf2 + 1,  posf3 + 0,  posf4 + 1,  posf5 + 1,  posf6 + 1,  posf7 + 1, 
                              i, numGradVecs);
    float result10101111 =  vec10 * tmp[0] +  vec01 * tmp[1] +  vec12 * tmp[2] +  vec03 * tmp[3] +  vec14 * tmp[4] +  vec15 * tmp[5] +  vec16 * tmp[6] +  vec17 * tmp[7] + 
    0;
     tmp =
        gradVecs +
        8 * hash8( posf0 + 1,  posf1 + 0,  posf2 + 1,  posf3 + 1,  posf4 + 0,  posf5 + 0,  posf6 + 0,  posf7 + 0, 
                              i, numGradVecs);
    float result10110000 =  vec10 * tmp[0] +  vec01 * tmp[1] +  vec12 * tmp[2] +  vec13 * tmp[3] +  vec04 * tmp[4] +  vec05 * tmp[5] +  vec06 * tmp[6] +  vec07 * tmp[7] + 
    0;
     tmp =
        gradVecs +
        8 * hash8( posf0 + 1,  posf1 + 0,  posf2 + 1,  posf3 + 1,  posf4 + 0,  posf5 + 0,  posf6 + 0,  posf7 + 1, 
                              i, numGradVecs);
    float result10110001 =  vec10 * tmp[0] +  vec01 * tmp[1] +  vec12 * tmp[2] +  vec13 * tmp[3] +  vec04 * tmp[4] +  vec05 * tmp[5] +  vec06 * tmp[6] +  vec17 * tmp[7] + 
    0;
     tmp =
        gradVecs +
        8 * hash8( posf0 + 1,  posf1 + 0,  posf2 + 1,  posf3 + 1,  posf4 + 0,  posf5 + 0,  posf6 + 1,  posf7 + 0, 
                              i, numGradVecs);
    float result10110010 =  vec10 * tmp[0] +  vec01 * tmp[1] +  vec12 * tmp[2] +  vec13 * tmp[3] +  vec04 * tmp[4] +  vec05 * tmp[5] +  vec16 * tmp[6] +  vec07 * tmp[7] + 
    0;
     tmp =
        gradVecs +
        8 * hash8( posf0 + 1,  posf1 + 0,  posf2 + 1,  posf3 + 1,  posf4 + 0,  posf5 + 0,  posf6 + 1,  posf7 + 1, 
                              i, numGradVecs);
    float result10110011 =  vec10 * tmp[0] +  vec01 * tmp[1] +  vec12 * tmp[2] +  vec13 * tmp[3] +  vec04 * tmp[4] +  vec05 * tmp[5] +  vec16 * tmp[6] +  vec17 * tmp[7] + 
    0;
     tmp =
        gradVecs +
        8 * hash8( posf0 + 1,  posf1 + 0,  posf2 + 1,  posf3 + 1,  posf4 + 0,  posf5 + 1,  posf6 + 0,  posf7 + 0, 
                              i, numGradVecs);
    float result10110100 =  vec10 * tmp[0] +  vec01 * tmp[1] +  vec12 * tmp[2] +  vec13 * tmp[3] +  vec04 * tmp[4] +  vec15 * tmp[5] +  vec06 * tmp[6] +  vec07 * tmp[7] + 
    0;
     tmp =
        gradVecs +
        8 * hash8( posf0 + 1,  posf1 + 0,  posf2 + 1,  posf3 + 1,  posf4 + 0,  posf5 + 1,  posf6 + 0,  posf7 + 1, 
                              i, numGradVecs);
    float result10110101 =  vec10 * tmp[0] +  vec01 * tmp[1] +  vec12 * tmp[2] +  vec13 * tmp[3] +  vec04 * tmp[4] +  vec15 * tmp[5] +  vec06 * tmp[6] +  vec17 * tmp[7] + 
    0;
     tmp =
        gradVecs +
        8 * hash8( posf0 + 1,  posf1 + 0,  posf2 + 1,  posf3 + 1,  posf4 + 0,  posf5 + 1,  posf6 + 1,  posf7 + 0, 
                              i, numGradVecs);
    float result10110110 =  vec10 * tmp[0] +  vec01 * tmp[1] +  vec12 * tmp[2] +  vec13 * tmp[3] +  vec04 * tmp[4] +  vec15 * tmp[5] +  vec16 * tmp[6] +  vec07 * tmp[7] + 
    0;
     tmp =
        gradVecs +
        8 * hash8( posf0 + 1,  posf1 + 0,  posf2 + 1,  posf3 + 1,  posf4 + 0,  posf5 + 1,  posf6 + 1,  posf7 + 1, 
                              i, numGradVecs);
    float result10110111 =  vec10 * tmp[0] +  vec01 * tmp[1] +  vec12 * tmp[2] +  vec13 * tmp[3] +  vec04 * tmp[4] +  vec15 * tmp[5] +  vec16 * tmp[6] +  vec17 * tmp[7] + 
    0;
     tmp =
        gradVecs +
        8 * hash8( posf0 + 1,  posf1 + 0,  posf2 + 1,  posf3 + 1,  posf4 + 1,  posf5 + 0,  posf6 + 0,  posf7 + 0, 
                              i, numGradVecs);
    float result10111000 =  vec10 * tmp[0] +  vec01 * tmp[1] +  vec12 * tmp[2] +  vec13 * tmp[3] +  vec14 * tmp[4] +  vec05 * tmp[5] +  vec06 * tmp[6] +  vec07 * tmp[7] + 
    0;
     tmp =
        gradVecs +
        8 * hash8( posf0 + 1,  posf1 + 0,  posf2 + 1,  posf3 + 1,  posf4 + 1,  posf5 + 0,  posf6 + 0,  posf7 + 1, 
                              i, numGradVecs);
    float result10111001 =  vec10 * tmp[0] +  vec01 * tmp[1] +  vec12 * tmp[2] +  vec13 * tmp[3] +  vec14 * tmp[4] +  vec05 * tmp[5] +  vec06 * tmp[6] +  vec17 * tmp[7] + 
    0;
     tmp =
        gradVecs +
        8 * hash8( posf0 + 1,  posf1 + 0,  posf2 + 1,  posf3 + 1,  posf4 + 1,  posf5 + 0,  posf6 + 1,  posf7 + 0, 
                              i, numGradVecs);
    float result10111010 =  vec10 * tmp[0] +  vec01 * tmp[1] +  vec12 * tmp[2] +  vec13 * tmp[3] +  vec14 * tmp[4] +  vec05 * tmp[5] +  vec16 * tmp[6] +  vec07 * tmp[7] + 
    0;
     tmp =
        gradVecs +
        8 * hash8( posf0 + 1,  posf1 + 0,  posf2 + 1,  posf3 + 1,  posf4 + 1,  posf5 + 0,  posf6 + 1,  posf7 + 1, 
                              i, numGradVecs);
    float result10111011 =  vec10 * tmp[0] +  vec01 * tmp[1] +  vec12 * tmp[2] +  vec13 * tmp[3] +  vec14 * tmp[4] +  vec05 * tmp[5] +  vec16 * tmp[6] +  vec17 * tmp[7] + 
    0;
     tmp =
        gradVecs +
        8 * hash8( posf0 + 1,  posf1 + 0,  posf2 + 1,  posf3 + 1,  posf4 + 1,  posf5 + 1,  posf6 + 0,  posf7 + 0, 
                              i, numGradVecs);
    float result10111100 =  vec10 * tmp[0] +  vec01 * tmp[1] +  vec12 * tmp[2] +  vec13 * tmp[3] +  vec14 * tmp[4] +  vec15 * tmp[5] +  vec06 * tmp[6] +  vec07 * tmp[7] + 
    0;
     tmp =
        gradVecs +
        8 * hash8( posf0 + 1,  posf1 + 0,  posf2 + 1,  posf3 + 1,  posf4 + 1,  posf5 + 1,  posf6 + 0,  posf7 + 1, 
                              i, numGradVecs);
    float result10111101 =  vec10 * tmp[0] +  vec01 * tmp[1] +  vec12 * tmp[2] +  vec13 * tmp[3] +  vec14 * tmp[4] +  vec15 * tmp[5] +  vec06 * tmp[6] +  vec17 * tmp[7] + 
    0;
     tmp =
        gradVecs +
        8 * hash8( posf0 + 1,  posf1 + 0,  posf2 + 1,  posf3 + 1,  posf4 + 1,  posf5 + 1,  posf6 + 1,  posf7 + 0, 
                              i, numGradVecs);
    float result10111110 =  vec10 * tmp[0] +  vec01 * tmp[1] +  vec12 * tmp[2] +  vec13 * tmp[3] +  vec14 * tmp[4] +  vec15 * tmp[5] +  vec16 * tmp[6] +  vec07 * tmp[7] + 
    0;
     tmp =
        gradVecs +
        8 * hash8( posf0 + 1,  posf1 + 0,  posf2 + 1,  posf3 + 1,  posf4 + 1,  posf5 + 1,  posf6 + 1,  posf7 + 1, 
                              i, numGradVecs);
    float result10111111 =  vec10 * tmp[0] +  vec01 * tmp[1] +  vec12 * tmp[2] +  vec13 * tmp[3] +  vec14 * tmp[4] +  vec15 * tmp[5] +  vec16 * tmp[6] +  vec17 * tmp[7] + 
    0;
     tmp =
        gradVecs +
        8 * hash8( posf0 + 1,  posf1 + 1,  posf2 + 0,  posf3 + 0,  posf4 + 0,  posf5 + 0,  posf6 + 0,  posf7 + 0, 
                              i, numGradVecs);
    float result11000000 =  vec10 * tmp[0] +  vec11 * tmp[1] +  vec02 * tmp[2] +  vec03 * tmp[3] +  vec04 * tmp[4] +  vec05 * tmp[5] +  vec06 * tmp[6] +  vec07 * tmp[7] + 
    0;
     tmp =
        gradVecs +
        8 * hash8( posf0 + 1,  posf1 + 1,  posf2 + 0,  posf3 + 0,  posf4 + 0,  posf5 + 0,  posf6 + 0,  posf7 + 1, 
                              i, numGradVecs);
    float result11000001 =  vec10 * tmp[0] +  vec11 * tmp[1] +  vec02 * tmp[2] +  vec03 * tmp[3] +  vec04 * tmp[4] +  vec05 * tmp[5] +  vec06 * tmp[6] +  vec17 * tmp[7] + 
    0;
     tmp =
        gradVecs +
        8 * hash8( posf0 + 1,  posf1 + 1,  posf2 + 0,  posf3 + 0,  posf4 + 0,  posf5 + 0,  posf6 + 1,  posf7 + 0, 
                              i, numGradVecs);
    float result11000010 =  vec10 * tmp[0] +  vec11 * tmp[1] +  vec02 * tmp[2] +  vec03 * tmp[3] +  vec04 * tmp[4] +  vec05 * tmp[5] +  vec16 * tmp[6] +  vec07 * tmp[7] + 
    0;
     tmp =
        gradVecs +
        8 * hash8( posf0 + 1,  posf1 + 1,  posf2 + 0,  posf3 + 0,  posf4 + 0,  posf5 + 0,  posf6 + 1,  posf7 + 1, 
                              i, numGradVecs);
    float result11000011 =  vec10 * tmp[0] +  vec11 * tmp[1] +  vec02 * tmp[2] +  vec03 * tmp[3] +  vec04 * tmp[4] +  vec05 * tmp[5] +  vec16 * tmp[6] +  vec17 * tmp[7] + 
    0;
     tmp =
        gradVecs +
        8 * hash8( posf0 + 1,  posf1 + 1,  posf2 + 0,  posf3 + 0,  posf4 + 0,  posf5 + 1,  posf6 + 0,  posf7 + 0, 
                              i, numGradVecs);
    float result11000100 =  vec10 * tmp[0] +  vec11 * tmp[1] +  vec02 * tmp[2] +  vec03 * tmp[3] +  vec04 * tmp[4] +  vec15 * tmp[5] +  vec06 * tmp[6] +  vec07 * tmp[7] + 
    0;
     tmp =
        gradVecs +
        8 * hash8( posf0 + 1,  posf1 + 1,  posf2 + 0,  posf3 + 0,  posf4 + 0,  posf5 + 1,  posf6 + 0,  posf7 + 1, 
                              i, numGradVecs);
    float result11000101 =  vec10 * tmp[0] +  vec11 * tmp[1] +  vec02 * tmp[2] +  vec03 * tmp[3] +  vec04 * tmp[4] +  vec15 * tmp[5] +  vec06 * tmp[6] +  vec17 * tmp[7] + 
    0;
     tmp =
        gradVecs +
        8 * hash8( posf0 + 1,  posf1 + 1,  posf2 + 0,  posf3 + 0,  posf4 + 0,  posf5 + 1,  posf6 + 1,  posf7 + 0, 
                              i, numGradVecs);
    float result11000110 =  vec10 * tmp[0] +  vec11 * tmp[1] +  vec02 * tmp[2] +  vec03 * tmp[3] +  vec04 * tmp[4] +  vec15 * tmp[5] +  vec16 * tmp[6] +  vec07 * tmp[7] + 
    0;
     tmp =
        gradVecs +
        8 * hash8( posf0 + 1,  posf1 + 1,  posf2 + 0,  posf3 + 0,  posf4 + 0,  posf5 + 1,  posf6 + 1,  posf7 + 1, 
                              i, numGradVecs);
    float result11000111 =  vec10 * tmp[0] +  vec11 * tmp[1] +  vec02 * tmp[2] +  vec03 * tmp[3] +  vec04 * tmp[4] +  vec15 * tmp[5] +  vec16 * tmp[6] +  vec17 * tmp[7] + 
    0;
     tmp =
        gradVecs +
        8 * hash8( posf0 + 1,  posf1 + 1,  posf2 + 0,  posf3 + 0,  posf4 + 1,  posf5 + 0,  posf6 + 0,  posf7 + 0, 
                              i, numGradVecs);
    float result11001000 =  vec10 * tmp[0] +  vec11 * tmp[1] +  vec02 * tmp[2] +  vec03 * tmp[3] +  vec14 * tmp[4] +  vec05 * tmp[5] +  vec06 * tmp[6] +  vec07 * tmp[7] + 
    0;
     tmp =
        gradVecs +
        8 * hash8( posf0 + 1,  posf1 + 1,  posf2 + 0,  posf3 + 0,  posf4 + 1,  posf5 + 0,  posf6 + 0,  posf7 + 1, 
                              i, numGradVecs);
    float result11001001 =  vec10 * tmp[0] +  vec11 * tmp[1] +  vec02 * tmp[2] +  vec03 * tmp[3] +  vec14 * tmp[4] +  vec05 * tmp[5] +  vec06 * tmp[6] +  vec17 * tmp[7] + 
    0;
     tmp =
        gradVecs +
        8 * hash8( posf0 + 1,  posf1 + 1,  posf2 + 0,  posf3 + 0,  posf4 + 1,  posf5 + 0,  posf6 + 1,  posf7 + 0, 
                              i, numGradVecs);
    float result11001010 =  vec10 * tmp[0] +  vec11 * tmp[1] +  vec02 * tmp[2] +  vec03 * tmp[3] +  vec14 * tmp[4] +  vec05 * tmp[5] +  vec16 * tmp[6] +  vec07 * tmp[7] + 
    0;
     tmp =
        gradVecs +
        8 * hash8( posf0 + 1,  posf1 + 1,  posf2 + 0,  posf3 + 0,  posf4 + 1,  posf5 + 0,  posf6 + 1,  posf7 + 1, 
                              i, numGradVecs);
    float result11001011 =  vec10 * tmp[0] +  vec11 * tmp[1] +  vec02 * tmp[2] +  vec03 * tmp[3] +  vec14 * tmp[4] +  vec05 * tmp[5] +  vec16 * tmp[6] +  vec17 * tmp[7] + 
    0;
     tmp =
        gradVecs +
        8 * hash8( posf0 + 1,  posf1 + 1,  posf2 + 0,  posf3 + 0,  posf4 + 1,  posf5 + 1,  posf6 + 0,  posf7 + 0, 
                              i, numGradVecs);
    float result11001100 =  vec10 * tmp[0] +  vec11 * tmp[1] +  vec02 * tmp[2] +  vec03 * tmp[3] +  vec14 * tmp[4] +  vec15 * tmp[5] +  vec06 * tmp[6] +  vec07 * tmp[7] + 
    0;
     tmp =
        gradVecs +
        8 * hash8( posf0 + 1,  posf1 + 1,  posf2 + 0,  posf3 + 0,  posf4 + 1,  posf5 + 1,  posf6 + 0,  posf7 + 1, 
                              i, numGradVecs);
    float result11001101 =  vec10 * tmp[0] +  vec11 * tmp[1] +  vec02 * tmp[2] +  vec03 * tmp[3] +  vec14 * tmp[4] +  vec15 * tmp[5] +  vec06 * tmp[6] +  vec17 * tmp[7] + 
    0;
     tmp =
        gradVecs +
        8 * hash8( posf0 + 1,  posf1 + 1,  posf2 + 0,  posf3 + 0,  posf4 + 1,  posf5 + 1,  posf6 + 1,  posf7 + 0, 
                              i, numGradVecs);
    float result11001110 =  vec10 * tmp[0] +  vec11 * tmp[1] +  vec02 * tmp[2] +  vec03 * tmp[3] +  vec14 * tmp[4] +  vec15 * tmp[5] +  vec16 * tmp[6] +  vec07 * tmp[7] + 
    0;
     tmp =
        gradVecs +
        8 * hash8( posf0 + 1,  posf1 + 1,  posf2 + 0,  posf3 + 0,  posf4 + 1,  posf5 + 1,  posf6 + 1,  posf7 + 1, 
                              i, numGradVecs);
    float result11001111 =  vec10 * tmp[0] +  vec11 * tmp[1] +  vec02 * tmp[2] +  vec03 * tmp[3] +  vec14 * tmp[4] +  vec15 * tmp[5] +  vec16 * tmp[6] +  vec17 * tmp[7] + 
    0;
     tmp =
        gradVecs +
        8 * hash8( posf0 + 1,  posf1 + 1,  posf2 + 0,  posf3 + 1,  posf4 + 0,  posf5 + 0,  posf6 + 0,  posf7 + 0, 
                              i, numGradVecs);
    float result11010000 =  vec10 * tmp[0] +  vec11 * tmp[1] +  vec02 * tmp[2] +  vec13 * tmp[3] +  vec04 * tmp[4] +  vec05 * tmp[5] +  vec06 * tmp[6] +  vec07 * tmp[7] + 
    0;
     tmp =
        gradVecs +
        8 * hash8( posf0 + 1,  posf1 + 1,  posf2 + 0,  posf3 + 1,  posf4 + 0,  posf5 + 0,  posf6 + 0,  posf7 + 1, 
                              i, numGradVecs);
    float result11010001 =  vec10 * tmp[0] +  vec11 * tmp[1] +  vec02 * tmp[2] +  vec13 * tmp[3] +  vec04 * tmp[4] +  vec05 * tmp[5] +  vec06 * tmp[6] +  vec17 * tmp[7] + 
    0;
     tmp =
        gradVecs +
        8 * hash8( posf0 + 1,  posf1 + 1,  posf2 + 0,  posf3 + 1,  posf4 + 0,  posf5 + 0,  posf6 + 1,  posf7 + 0, 
                              i, numGradVecs);
    float result11010010 =  vec10 * tmp[0] +  vec11 * tmp[1] +  vec02 * tmp[2] +  vec13 * tmp[3] +  vec04 * tmp[4] +  vec05 * tmp[5] +  vec16 * tmp[6] +  vec07 * tmp[7] + 
    0;
     tmp =
        gradVecs +
        8 * hash8( posf0 + 1,  posf1 + 1,  posf2 + 0,  posf3 + 1,  posf4 + 0,  posf5 + 0,  posf6 + 1,  posf7 + 1, 
                              i, numGradVecs);
    float result11010011 =  vec10 * tmp[0] +  vec11 * tmp[1] +  vec02 * tmp[2] +  vec13 * tmp[3] +  vec04 * tmp[4] +  vec05 * tmp[5] +  vec16 * tmp[6] +  vec17 * tmp[7] + 
    0;
     tmp =
        gradVecs +
        8 * hash8( posf0 + 1,  posf1 + 1,  posf2 + 0,  posf3 + 1,  posf4 + 0,  posf5 + 1,  posf6 + 0,  posf7 + 0, 
                              i, numGradVecs);
    float result11010100 =  vec10 * tmp[0] +  vec11 * tmp[1] +  vec02 * tmp[2] +  vec13 * tmp[3] +  vec04 * tmp[4] +  vec15 * tmp[5] +  vec06 * tmp[6] +  vec07 * tmp[7] + 
    0;
     tmp =
        gradVecs +
        8 * hash8( posf0 + 1,  posf1 + 1,  posf2 + 0,  posf3 + 1,  posf4 + 0,  posf5 + 1,  posf6 + 0,  posf7 + 1, 
                              i, numGradVecs);
    float result11010101 =  vec10 * tmp[0] +  vec11 * tmp[1] +  vec02 * tmp[2] +  vec13 * tmp[3] +  vec04 * tmp[4] +  vec15 * tmp[5] +  vec06 * tmp[6] +  vec17 * tmp[7] + 
    0;
     tmp =
        gradVecs +
        8 * hash8( posf0 + 1,  posf1 + 1,  posf2 + 0,  posf3 + 1,  posf4 + 0,  posf5 + 1,  posf6 + 1,  posf7 + 0, 
                              i, numGradVecs);
    float result11010110 =  vec10 * tmp[0] +  vec11 * tmp[1] +  vec02 * tmp[2] +  vec13 * tmp[3] +  vec04 * tmp[4] +  vec15 * tmp[5] +  vec16 * tmp[6] +  vec07 * tmp[7] + 
    0;
     tmp =
        gradVecs +
        8 * hash8( posf0 + 1,  posf1 + 1,  posf2 + 0,  posf3 + 1,  posf4 + 0,  posf5 + 1,  posf6 + 1,  posf7 + 1, 
                              i, numGradVecs);
    float result11010111 =  vec10 * tmp[0] +  vec11 * tmp[1] +  vec02 * tmp[2] +  vec13 * tmp[3] +  vec04 * tmp[4] +  vec15 * tmp[5] +  vec16 * tmp[6] +  vec17 * tmp[7] + 
    0;
     tmp =
        gradVecs +
        8 * hash8( posf0 + 1,  posf1 + 1,  posf2 + 0,  posf3 + 1,  posf4 + 1,  posf5 + 0,  posf6 + 0,  posf7 + 0, 
                              i, numGradVecs);
    float result11011000 =  vec10 * tmp[0] +  vec11 * tmp[1] +  vec02 * tmp[2] +  vec13 * tmp[3] +  vec14 * tmp[4] +  vec05 * tmp[5] +  vec06 * tmp[6] +  vec07 * tmp[7] + 
    0;
     tmp =
        gradVecs +
        8 * hash8( posf0 + 1,  posf1 + 1,  posf2 + 0,  posf3 + 1,  posf4 + 1,  posf5 + 0,  posf6 + 0,  posf7 + 1, 
                              i, numGradVecs);
    float result11011001 =  vec10 * tmp[0] +  vec11 * tmp[1] +  vec02 * tmp[2] +  vec13 * tmp[3] +  vec14 * tmp[4] +  vec05 * tmp[5] +  vec06 * tmp[6] +  vec17 * tmp[7] + 
    0;
     tmp =
        gradVecs +
        8 * hash8( posf0 + 1,  posf1 + 1,  posf2 + 0,  posf3 + 1,  posf4 + 1,  posf5 + 0,  posf6 + 1,  posf7 + 0, 
                              i, numGradVecs);
    float result11011010 =  vec10 * tmp[0] +  vec11 * tmp[1] +  vec02 * tmp[2] +  vec13 * tmp[3] +  vec14 * tmp[4] +  vec05 * tmp[5] +  vec16 * tmp[6] +  vec07 * tmp[7] + 
    0;
     tmp =
        gradVecs +
        8 * hash8( posf0 + 1,  posf1 + 1,  posf2 + 0,  posf3 + 1,  posf4 + 1,  posf5 + 0,  posf6 + 1,  posf7 + 1, 
                              i, numGradVecs);
    float result11011011 =  vec10 * tmp[0] +  vec11 * tmp[1] +  vec02 * tmp[2] +  vec13 * tmp[3] +  vec14 * tmp[4] +  vec05 * tmp[5] +  vec16 * tmp[6] +  vec17 * tmp[7] + 
    0;
     tmp =
        gradVecs +
        8 * hash8( posf0 + 1,  posf1 + 1,  posf2 + 0,  posf3 + 1,  posf4 + 1,  posf5 + 1,  posf6 + 0,  posf7 + 0, 
                              i, numGradVecs);
    float result11011100 =  vec10 * tmp[0] +  vec11 * tmp[1] +  vec02 * tmp[2] +  vec13 * tmp[3] +  vec14 * tmp[4] +  vec15 * tmp[5] +  vec06 * tmp[6] +  vec07 * tmp[7] + 
    0;
     tmp =
        gradVecs +
        8 * hash8( posf0 + 1,  posf1 + 1,  posf2 + 0,  posf3 + 1,  posf4 + 1,  posf5 + 1,  posf6 + 0,  posf7 + 1, 
                              i, numGradVecs);
    float result11011101 =  vec10 * tmp[0] +  vec11 * tmp[1] +  vec02 * tmp[2] +  vec13 * tmp[3] +  vec14 * tmp[4] +  vec15 * tmp[5] +  vec06 * tmp[6] +  vec17 * tmp[7] + 
    0;
     tmp =
        gradVecs +
        8 * hash8( posf0 + 1,  posf1 + 1,  posf2 + 0,  posf3 + 1,  posf4 + 1,  posf5 + 1,  posf6 + 1,  posf7 + 0, 
                              i, numGradVecs);
    float result11011110 =  vec10 * tmp[0] +  vec11 * tmp[1] +  vec02 * tmp[2] +  vec13 * tmp[3] +  vec14 * tmp[4] +  vec15 * tmp[5] +  vec16 * tmp[6] +  vec07 * tmp[7] + 
    0;
     tmp =
        gradVecs +
        8 * hash8( posf0 + 1,  posf1 + 1,  posf2 + 0,  posf3 + 1,  posf4 + 1,  posf5 + 1,  posf6 + 1,  posf7 + 1, 
                              i, numGradVecs);
    float result11011111 =  vec10 * tmp[0] +  vec11 * tmp[1] +  vec02 * tmp[2] +  vec13 * tmp[3] +  vec14 * tmp[4] +  vec15 * tmp[5] +  vec16 * tmp[6] +  vec17 * tmp[7] + 
    0;
     tmp =
        gradVecs +
        8 * hash8( posf0 + 1,  posf1 + 1,  posf2 + 1,  posf3 + 0,  posf4 + 0,  posf5 + 0,  posf6 + 0,  posf7 + 0, 
                              i, numGradVecs);
    float result11100000 =  vec10 * tmp[0] +  vec11 * tmp[1] +  vec12 * tmp[2] +  vec03 * tmp[3] +  vec04 * tmp[4] +  vec05 * tmp[5] +  vec06 * tmp[6] +  vec07 * tmp[7] + 
    0;
     tmp =
        gradVecs +
        8 * hash8( posf0 + 1,  posf1 + 1,  posf2 + 1,  posf3 + 0,  posf4 + 0,  posf5 + 0,  posf6 + 0,  posf7 + 1, 
                              i, numGradVecs);
    float result11100001 =  vec10 * tmp[0] +  vec11 * tmp[1] +  vec12 * tmp[2] +  vec03 * tmp[3] +  vec04 * tmp[4] +  vec05 * tmp[5] +  vec06 * tmp[6] +  vec17 * tmp[7] + 
    0;
     tmp =
        gradVecs +
        8 * hash8( posf0 + 1,  posf1 + 1,  posf2 + 1,  posf3 + 0,  posf4 + 0,  posf5 + 0,  posf6 + 1,  posf7 + 0, 
                              i, numGradVecs);
    float result11100010 =  vec10 * tmp[0] +  vec11 * tmp[1] +  vec12 * tmp[2] +  vec03 * tmp[3] +  vec04 * tmp[4] +  vec05 * tmp[5] +  vec16 * tmp[6] +  vec07 * tmp[7] + 
    0;
     tmp =
        gradVecs +
        8 * hash8( posf0 + 1,  posf1 + 1,  posf2 + 1,  posf3 + 0,  posf4 + 0,  posf5 + 0,  posf6 + 1,  posf7 + 1, 
                              i, numGradVecs);
    float result11100011 =  vec10 * tmp[0] +  vec11 * tmp[1] +  vec12 * tmp[2] +  vec03 * tmp[3] +  vec04 * tmp[4] +  vec05 * tmp[5] +  vec16 * tmp[6] +  vec17 * tmp[7] + 
    0;
     tmp =
        gradVecs +
        8 * hash8( posf0 + 1,  posf1 + 1,  posf2 + 1,  posf3 + 0,  posf4 + 0,  posf5 + 1,  posf6 + 0,  posf7 + 0, 
                              i, numGradVecs);
    float result11100100 =  vec10 * tmp[0] +  vec11 * tmp[1] +  vec12 * tmp[2] +  vec03 * tmp[3] +  vec04 * tmp[4] +  vec15 * tmp[5] +  vec06 * tmp[6] +  vec07 * tmp[7] + 
    0;
     tmp =
        gradVecs +
        8 * hash8( posf0 + 1,  posf1 + 1,  posf2 + 1,  posf3 + 0,  posf4 + 0,  posf5 + 1,  posf6 + 0,  posf7 + 1, 
                              i, numGradVecs);
    float result11100101 =  vec10 * tmp[0] +  vec11 * tmp[1] +  vec12 * tmp[2] +  vec03 * tmp[3] +  vec04 * tmp[4] +  vec15 * tmp[5] +  vec06 * tmp[6] +  vec17 * tmp[7] + 
    0;
     tmp =
        gradVecs +
        8 * hash8( posf0 + 1,  posf1 + 1,  posf2 + 1,  posf3 + 0,  posf4 + 0,  posf5 + 1,  posf6 + 1,  posf7 + 0, 
                              i, numGradVecs);
    float result11100110 =  vec10 * tmp[0] +  vec11 * tmp[1] +  vec12 * tmp[2] +  vec03 * tmp[3] +  vec04 * tmp[4] +  vec15 * tmp[5] +  vec16 * tmp[6] +  vec07 * tmp[7] + 
    0;
     tmp =
        gradVecs +
        8 * hash8( posf0 + 1,  posf1 + 1,  posf2 + 1,  posf3 + 0,  posf4 + 0,  posf5 + 1,  posf6 + 1,  posf7 + 1, 
                              i, numGradVecs);
    float result11100111 =  vec10 * tmp[0] +  vec11 * tmp[1] +  vec12 * tmp[2] +  vec03 * tmp[3] +  vec04 * tmp[4] +  vec15 * tmp[5] +  vec16 * tmp[6] +  vec17 * tmp[7] + 
    0;
     tmp =
        gradVecs +
        8 * hash8( posf0 + 1,  posf1 + 1,  posf2 + 1,  posf3 + 0,  posf4 + 1,  posf5 + 0,  posf6 + 0,  posf7 + 0, 
                              i, numGradVecs);
    float result11101000 =  vec10 * tmp[0] +  vec11 * tmp[1] +  vec12 * tmp[2] +  vec03 * tmp[3] +  vec14 * tmp[4] +  vec05 * tmp[5] +  vec06 * tmp[6] +  vec07 * tmp[7] + 
    0;
     tmp =
        gradVecs +
        8 * hash8( posf0 + 1,  posf1 + 1,  posf2 + 1,  posf3 + 0,  posf4 + 1,  posf5 + 0,  posf6 + 0,  posf7 + 1, 
                              i, numGradVecs);
    float result11101001 =  vec10 * tmp[0] +  vec11 * tmp[1] +  vec12 * tmp[2] +  vec03 * tmp[3] +  vec14 * tmp[4] +  vec05 * tmp[5] +  vec06 * tmp[6] +  vec17 * tmp[7] + 
    0;
     tmp =
        gradVecs +
        8 * hash8( posf0 + 1,  posf1 + 1,  posf2 + 1,  posf3 + 0,  posf4 + 1,  posf5 + 0,  posf6 + 1,  posf7 + 0, 
                              i, numGradVecs);
    float result11101010 =  vec10 * tmp[0] +  vec11 * tmp[1] +  vec12 * tmp[2] +  vec03 * tmp[3] +  vec14 * tmp[4] +  vec05 * tmp[5] +  vec16 * tmp[6] +  vec07 * tmp[7] + 
    0;
     tmp =
        gradVecs +
        8 * hash8( posf0 + 1,  posf1 + 1,  posf2 + 1,  posf3 + 0,  posf4 + 1,  posf5 + 0,  posf6 + 1,  posf7 + 1, 
                              i, numGradVecs);
    float result11101011 =  vec10 * tmp[0] +  vec11 * tmp[1] +  vec12 * tmp[2] +  vec03 * tmp[3] +  vec14 * tmp[4] +  vec05 * tmp[5] +  vec16 * tmp[6] +  vec17 * tmp[7] + 
    0;
     tmp =
        gradVecs +
        8 * hash8( posf0 + 1,  posf1 + 1,  posf2 + 1,  posf3 + 0,  posf4 + 1,  posf5 + 1,  posf6 + 0,  posf7 + 0, 
                              i, numGradVecs);
    float result11101100 =  vec10 * tmp[0] +  vec11 * tmp[1] +  vec12 * tmp[2] +  vec03 * tmp[3] +  vec14 * tmp[4] +  vec15 * tmp[5] +  vec06 * tmp[6] +  vec07 * tmp[7] + 
    0;
     tmp =
        gradVecs +
        8 * hash8( posf0 + 1,  posf1 + 1,  posf2 + 1,  posf3 + 0,  posf4 + 1,  posf5 + 1,  posf6 + 0,  posf7 + 1, 
                              i, numGradVecs);
    float result11101101 =  vec10 * tmp[0] +  vec11 * tmp[1] +  vec12 * tmp[2] +  vec03 * tmp[3] +  vec14 * tmp[4] +  vec15 * tmp[5] +  vec06 * tmp[6] +  vec17 * tmp[7] + 
    0;
     tmp =
        gradVecs +
        8 * hash8( posf0 + 1,  posf1 + 1,  posf2 + 1,  posf3 + 0,  posf4 + 1,  posf5 + 1,  posf6 + 1,  posf7 + 0, 
                              i, numGradVecs);
    float result11101110 =  vec10 * tmp[0] +  vec11 * tmp[1] +  vec12 * tmp[2] +  vec03 * tmp[3] +  vec14 * tmp[4] +  vec15 * tmp[5] +  vec16 * tmp[6] +  vec07 * tmp[7] + 
    0;
     tmp =
        gradVecs +
        8 * hash8( posf0 + 1,  posf1 + 1,  posf2 + 1,  posf3 + 0,  posf4 + 1,  posf5 + 1,  posf6 + 1,  posf7 + 1, 
                              i, numGradVecs);
    float result11101111 =  vec10 * tmp[0] +  vec11 * tmp[1] +  vec12 * tmp[2] +  vec03 * tmp[3] +  vec14 * tmp[4] +  vec15 * tmp[5] +  vec16 * tmp[6] +  vec17 * tmp[7] + 
    0;
     tmp =
        gradVecs +
        8 * hash8( posf0 + 1,  posf1 + 1,  posf2 + 1,  posf3 + 1,  posf4 + 0,  posf5 + 0,  posf6 + 0,  posf7 + 0, 
                              i, numGradVecs);
    float result11110000 =  vec10 * tmp[0] +  vec11 * tmp[1] +  vec12 * tmp[2] +  vec13 * tmp[3] +  vec04 * tmp[4] +  vec05 * tmp[5] +  vec06 * tmp[6] +  vec07 * tmp[7] + 
    0;
     tmp =
        gradVecs +
        8 * hash8( posf0 + 1,  posf1 + 1,  posf2 + 1,  posf3 + 1,  posf4 + 0,  posf5 + 0,  posf6 + 0,  posf7 + 1, 
                              i, numGradVecs);
    float result11110001 =  vec10 * tmp[0] +  vec11 * tmp[1] +  vec12 * tmp[2] +  vec13 * tmp[3] +  vec04 * tmp[4] +  vec05 * tmp[5] +  vec06 * tmp[6] +  vec17 * tmp[7] + 
    0;
     tmp =
        gradVecs +
        8 * hash8( posf0 + 1,  posf1 + 1,  posf2 + 1,  posf3 + 1,  posf4 + 0,  posf5 + 0,  posf6 + 1,  posf7 + 0, 
                              i, numGradVecs);
    float result11110010 =  vec10 * tmp[0] +  vec11 * tmp[1] +  vec12 * tmp[2] +  vec13 * tmp[3] +  vec04 * tmp[4] +  vec05 * tmp[5] +  vec16 * tmp[6] +  vec07 * tmp[7] + 
    0;
     tmp =
        gradVecs +
        8 * hash8( posf0 + 1,  posf1 + 1,  posf2 + 1,  posf3 + 1,  posf4 + 0,  posf5 + 0,  posf6 + 1,  posf7 + 1, 
                              i, numGradVecs);
    float result11110011 =  vec10 * tmp[0] +  vec11 * tmp[1] +  vec12 * tmp[2] +  vec13 * tmp[3] +  vec04 * tmp[4] +  vec05 * tmp[5] +  vec16 * tmp[6] +  vec17 * tmp[7] + 
    0;
     tmp =
        gradVecs +
        8 * hash8( posf0 + 1,  posf1 + 1,  posf2 + 1,  posf3 + 1,  posf4 + 0,  posf5 + 1,  posf6 + 0,  posf7 + 0, 
                              i, numGradVecs);
    float result11110100 =  vec10 * tmp[0] +  vec11 * tmp[1] +  vec12 * tmp[2] +  vec13 * tmp[3] +  vec04 * tmp[4] +  vec15 * tmp[5] +  vec06 * tmp[6] +  vec07 * tmp[7] + 
    0;
     tmp =
        gradVecs +
        8 * hash8( posf0 + 1,  posf1 + 1,  posf2 + 1,  posf3 + 1,  posf4 + 0,  posf5 + 1,  posf6 + 0,  posf7 + 1, 
                              i, numGradVecs);
    float result11110101 =  vec10 * tmp[0] +  vec11 * tmp[1] +  vec12 * tmp[2] +  vec13 * tmp[3] +  vec04 * tmp[4] +  vec15 * tmp[5] +  vec06 * tmp[6] +  vec17 * tmp[7] + 
    0;
     tmp =
        gradVecs +
        8 * hash8( posf0 + 1,  posf1 + 1,  posf2 + 1,  posf3 + 1,  posf4 + 0,  posf5 + 1,  posf6 + 1,  posf7 + 0, 
                              i, numGradVecs);
    float result11110110 =  vec10 * tmp[0] +  vec11 * tmp[1] +  vec12 * tmp[2] +  vec13 * tmp[3] +  vec04 * tmp[4] +  vec15 * tmp[5] +  vec16 * tmp[6] +  vec07 * tmp[7] + 
    0;
     tmp =
        gradVecs +
        8 * hash8( posf0 + 1,  posf1 + 1,  posf2 + 1,  posf3 + 1,  posf4 + 0,  posf5 + 1,  posf6 + 1,  posf7 + 1, 
                              i, numGradVecs);
    float result11110111 =  vec10 * tmp[0] +  vec11 * tmp[1] +  vec12 * tmp[2] +  vec13 * tmp[3] +  vec04 * tmp[4] +  vec15 * tmp[5] +  vec16 * tmp[6] +  vec17 * tmp[7] + 
    0;
     tmp =
        gradVecs +
        8 * hash8( posf0 + 1,  posf1 + 1,  posf2 + 1,  posf3 + 1,  posf4 + 1,  posf5 + 0,  posf6 + 0,  posf7 + 0, 
                              i, numGradVecs);
    float result11111000 =  vec10 * tmp[0] +  vec11 * tmp[1] +  vec12 * tmp[2] +  vec13 * tmp[3] +  vec14 * tmp[4] +  vec05 * tmp[5] +  vec06 * tmp[6] +  vec07 * tmp[7] + 
    0;
     tmp =
        gradVecs +
        8 * hash8( posf0 + 1,  posf1 + 1,  posf2 + 1,  posf3 + 1,  posf4 + 1,  posf5 + 0,  posf6 + 0,  posf7 + 1, 
                              i, numGradVecs);
    float result11111001 =  vec10 * tmp[0] +  vec11 * tmp[1] +  vec12 * tmp[2] +  vec13 * tmp[3] +  vec14 * tmp[4] +  vec05 * tmp[5] +  vec06 * tmp[6] +  vec17 * tmp[7] + 
    0;
     tmp =
        gradVecs +
        8 * hash8( posf0 + 1,  posf1 + 1,  posf2 + 1,  posf3 + 1,  posf4 + 1,  posf5 + 0,  posf6 + 1,  posf7 + 0, 
                              i, numGradVecs);
    float result11111010 =  vec10 * tmp[0] +  vec11 * tmp[1] +  vec12 * tmp[2] +  vec13 * tmp[3] +  vec14 * tmp[4] +  vec05 * tmp[5] +  vec16 * tmp[6] +  vec07 * tmp[7] + 
    0;
     tmp =
        gradVecs +
        8 * hash8( posf0 + 1,  posf1 + 1,  posf2 + 1,  posf3 + 1,  posf4 + 1,  posf5 + 0,  posf6 + 1,  posf7 + 1, 
                              i, numGradVecs);
    float result11111011 =  vec10 * tmp[0] +  vec11 * tmp[1] +  vec12 * tmp[2] +  vec13 * tmp[3] +  vec14 * tmp[4] +  vec05 * tmp[5] +  vec16 * tmp[6] +  vec17 * tmp[7] + 
    0;
     tmp =
        gradVecs +
        8 * hash8( posf0 + 1,  posf1 + 1,  posf2 + 1,  posf3 + 1,  posf4 + 1,  posf5 + 1,  posf6 + 0,  posf7 + 0, 
                              i, numGradVecs);
    float result11111100 =  vec10 * tmp[0] +  vec11 * tmp[1] +  vec12 * tmp[2] +  vec13 * tmp[3] +  vec14 * tmp[4] +  vec15 * tmp[5] +  vec06 * tmp[6] +  vec07 * tmp[7] + 
    0;
     tmp =
        gradVecs +
        8 * hash8( posf0 + 1,  posf1 + 1,  posf2 + 1,  posf3 + 1,  posf4 + 1,  posf5 + 1,  posf6 + 0,  posf7 + 1, 
                              i, numGradVecs);
    float result11111101 =  vec10 * tmp[0] +  vec11 * tmp[1] +  vec12 * tmp[2] +  vec13 * tmp[3] +  vec14 * tmp[4] +  vec15 * tmp[5] +  vec06 * tmp[6] +  vec17 * tmp[7] + 
    0;
     tmp =
        gradVecs +
        8 * hash8( posf0 + 1,  posf1 + 1,  posf2 + 1,  posf3 + 1,  posf4 + 1,  posf5 + 1,  posf6 + 1,  posf7 + 0, 
                              i, numGradVecs);
    float result11111110 =  vec10 * tmp[0] +  vec11 * tmp[1] +  vec12 * tmp[2] +  vec13 * tmp[3] +  vec14 * tmp[4] +  vec15 * tmp[5] +  vec16 * tmp[6] +  vec07 * tmp[7] + 
    0;
     tmp =
        gradVecs +
        8 * hash8( posf0 + 1,  posf1 + 1,  posf2 + 1,  posf3 + 1,  posf4 + 1,  posf5 + 1,  posf6 + 1,  posf7 + 1, 
                              i, numGradVecs);
    float result11111111 =  vec10 * tmp[0] +  vec11 * tmp[1] +  vec12 * tmp[2] +  vec13 * tmp[3] +  vec14 * tmp[4] +  vec15 * tmp[5] +  vec16 * tmp[6] +  vec17 * tmp[7] + 
    0;
    
      float result0000000 =
        result00000000 + lerp0 * (result10000000 - result00000000);
     float result0000001 =
        result00000001 + lerp0 * (result10000001 - result00000001);
     float result0000010 =
        result00000010 + lerp0 * (result10000010 - result00000010);
     float result0000011 =
        result00000011 + lerp0 * (result10000011 - result00000011);
     float result0000100 =
        result00000100 + lerp0 * (result10000100 - result00000100);
     float result0000101 =
        result00000101 + lerp0 * (result10000101 - result00000101);
     float result0000110 =
        result00000110 + lerp0 * (result10000110 - result00000110);
     float result0000111 =
        result00000111 + lerp0 * (result10000111 - result00000111);
     float result0001000 =
        result00001000 + lerp0 * (result10001000 - result00001000);
     float result0001001 =
        result00001001 + lerp0 * (result10001001 - result00001001);
     float result0001010 =
        result00001010 + lerp0 * (result10001010 - result00001010);
     float result0001011 =
        result00001011 + lerp0 * (result10001011 - result00001011);
     float result0001100 =
        result00001100 + lerp0 * (result10001100 - result00001100);
     float result0001101 =
        result00001101 + lerp0 * (result10001101 - result00001101);
     float result0001110 =
        result00001110 + lerp0 * (result10001110 - result00001110);
     float result0001111 =
        result00001111 + lerp0 * (result10001111 - result00001111);
     float result0010000 =
        result00010000 + lerp0 * (result10010000 - result00010000);
     float result0010001 =
        result00010001 + lerp0 * (result10010001 - result00010001);
     float result0010010 =
        result00010010 + lerp0 * (result10010010 - result00010010);
     float result0010011 =
        result00010011 + lerp0 * (result10010011 - result00010011);
     float result0010100 =
        result00010100 + lerp0 * (result10010100 - result00010100);
     float result0010101 =
        result00010101 + lerp0 * (result10010101 - result00010101);
     float result0010110 =
        result00010110 + lerp0 * (result10010110 - result00010110);
     float result0010111 =
        result00010111 + lerp0 * (result10010111 - result00010111);
     float result0011000 =
        result00011000 + lerp0 * (result10011000 - result00011000);
     float result0011001 =
        result00011001 + lerp0 * (result10011001 - result00011001);
     float result0011010 =
        result00011010 + lerp0 * (result10011010 - result00011010);
     float result0011011 =
        result00011011 + lerp0 * (result10011011 - result00011011);
     float result0011100 =
        result00011100 + lerp0 * (result10011100 - result00011100);
     float result0011101 =
        result00011101 + lerp0 * (result10011101 - result00011101);
     float result0011110 =
        result00011110 + lerp0 * (result10011110 - result00011110);
     float result0011111 =
        result00011111 + lerp0 * (result10011111 - result00011111);
     float result0100000 =
        result00100000 + lerp0 * (result10100000 - result00100000);
     float result0100001 =
        result00100001 + lerp0 * (result10100001 - result00100001);
     float result0100010 =
        result00100010 + lerp0 * (result10100010 - result00100010);
     float result0100011 =
        result00100011 + lerp0 * (result10100011 - result00100011);
     float result0100100 =
        result00100100 + lerp0 * (result10100100 - result00100100);
     float result0100101 =
        result00100101 + lerp0 * (result10100101 - result00100101);
     float result0100110 =
        result00100110 + lerp0 * (result10100110 - result00100110);
     float result0100111 =
        result00100111 + lerp0 * (result10100111 - result00100111);
     float result0101000 =
        result00101000 + lerp0 * (result10101000 - result00101000);
     float result0101001 =
        result00101001 + lerp0 * (result10101001 - result00101001);
     float result0101010 =
        result00101010 + lerp0 * (result10101010 - result00101010);
     float result0101011 =
        result00101011 + lerp0 * (result10101011 - result00101011);
     float result0101100 =
        result00101100 + lerp0 * (result10101100 - result00101100);
     float result0101101 =
        result00101101 + lerp0 * (result10101101 - result00101101);
     float result0101110 =
        result00101110 + lerp0 * (result10101110 - result00101110);
     float result0101111 =
        result00101111 + lerp0 * (result10101111 - result00101111);
     float result0110000 =
        result00110000 + lerp0 * (result10110000 - result00110000);
     float result0110001 =
        result00110001 + lerp0 * (result10110001 - result00110001);
     float result0110010 =
        result00110010 + lerp0 * (result10110010 - result00110010);
     float result0110011 =
        result00110011 + lerp0 * (result10110011 - result00110011);
     float result0110100 =
        result00110100 + lerp0 * (result10110100 - result00110100);
     float result0110101 =
        result00110101 + lerp0 * (result10110101 - result00110101);
     float result0110110 =
        result00110110 + lerp0 * (result10110110 - result00110110);
     float result0110111 =
        result00110111 + lerp0 * (result10110111 - result00110111);
     float result0111000 =
        result00111000 + lerp0 * (result10111000 - result00111000);
     float result0111001 =
        result00111001 + lerp0 * (result10111001 - result00111001);
     float result0111010 =
        result00111010 + lerp0 * (result10111010 - result00111010);
     float result0111011 =
        result00111011 + lerp0 * (result10111011 - result00111011);
     float result0111100 =
        result00111100 + lerp0 * (result10111100 - result00111100);
     float result0111101 =
        result00111101 + lerp0 * (result10111101 - result00111101);
     float result0111110 =
        result00111110 + lerp0 * (result10111110 - result00111110);
     float result0111111 =
        result00111111 + lerp0 * (result10111111 - result00111111);
     float result1000000 =
        result01000000 + lerp0 * (result11000000 - result01000000);
     float result1000001 =
        result01000001 + lerp0 * (result11000001 - result01000001);
     float result1000010 =
        result01000010 + lerp0 * (result11000010 - result01000010);
     float result1000011 =
        result01000011 + lerp0 * (result11000011 - result01000011);
     float result1000100 =
        result01000100 + lerp0 * (result11000100 - result01000100);
     float result1000101 =
        result01000101 + lerp0 * (result11000101 - result01000101);
     float result1000110 =
        result01000110 + lerp0 * (result11000110 - result01000110);
     float result1000111 =
        result01000111 + lerp0 * (result11000111 - result01000111);
     float result1001000 =
        result01001000 + lerp0 * (result11001000 - result01001000);
     float result1001001 =
        result01001001 + lerp0 * (result11001001 - result01001001);
     float result1001010 =
        result01001010 + lerp0 * (result11001010 - result01001010);
     float result1001011 =
        result01001011 + lerp0 * (result11001011 - result01001011);
     float result1001100 =
        result01001100 + lerp0 * (result11001100 - result01001100);
     float result1001101 =
        result01001101 + lerp0 * (result11001101 - result01001101);
     float result1001110 =
        result01001110 + lerp0 * (result11001110 - result01001110);
     float result1001111 =
        result01001111 + lerp0 * (result11001111 - result01001111);
     float result1010000 =
        result01010000 + lerp0 * (result11010000 - result01010000);
     float result1010001 =
        result01010001 + lerp0 * (result11010001 - result01010001);
     float result1010010 =
        result01010010 + lerp0 * (result11010010 - result01010010);
     float result1010011 =
        result01010011 + lerp0 * (result11010011 - result01010011);
     float result1010100 =
        result01010100 + lerp0 * (result11010100 - result01010100);
     float result1010101 =
        result01010101 + lerp0 * (result11010101 - result01010101);
     float result1010110 =
        result01010110 + lerp0 * (result11010110 - result01010110);
     float result1010111 =
        result01010111 + lerp0 * (result11010111 - result01010111);
     float result1011000 =
        result01011000 + lerp0 * (result11011000 - result01011000);
     float result1011001 =
        result01011001 + lerp0 * (result11011001 - result01011001);
     float result1011010 =
        result01011010 + lerp0 * (result11011010 - result01011010);
     float result1011011 =
        result01011011 + lerp0 * (result11011011 - result01011011);
     float result1011100 =
        result01011100 + lerp0 * (result11011100 - result01011100);
     float result1011101 =
        result01011101 + lerp0 * (result11011101 - result01011101);
     float result1011110 =
        result01011110 + lerp0 * (result11011110 - result01011110);
     float result1011111 =
        result01011111 + lerp0 * (result11011111 - result01011111);
     float result1100000 =
        result01100000 + lerp0 * (result11100000 - result01100000);
     float result1100001 =
        result01100001 + lerp0 * (result11100001 - result01100001);
     float result1100010 =
        result01100010 + lerp0 * (result11100010 - result01100010);
     float result1100011 =
        result01100011 + lerp0 * (result11100011 - result01100011);
     float result1100100 =
        result01100100 + lerp0 * (result11100100 - result01100100);
     float result1100101 =
        result01100101 + lerp0 * (result11100101 - result01100101);
     float result1100110 =
        result01100110 + lerp0 * (result11100110 - result01100110);
     float result1100111 =
        result01100111 + lerp0 * (result11100111 - result01100111);
     float result1101000 =
        result01101000 + lerp0 * (result11101000 - result01101000);
     float result1101001 =
        result01101001 + lerp0 * (result11101001 - result01101001);
     float result1101010 =
        result01101010 + lerp0 * (result11101010 - result01101010);
     float result1101011 =
        result01101011 + lerp0 * (result11101011 - result01101011);
     float result1101100 =
        result01101100 + lerp0 * (result11101100 - result01101100);
     float result1101101 =
        result01101101 + lerp0 * (result11101101 - result01101101);
     float result1101110 =
        result01101110 + lerp0 * (result11101110 - result01101110);
     float result1101111 =
        result01101111 + lerp0 * (result11101111 - result01101111);
     float result1110000 =
        result01110000 + lerp0 * (result11110000 - result01110000);
     float result1110001 =
        result01110001 + lerp0 * (result11110001 - result01110001);
     float result1110010 =
        result01110010 + lerp0 * (result11110010 - result01110010);
     float result1110011 =
        result01110011 + lerp0 * (result11110011 - result01110011);
     float result1110100 =
        result01110100 + lerp0 * (result11110100 - result01110100);
     float result1110101 =
        result01110101 + lerp0 * (result11110101 - result01110101);
     float result1110110 =
        result01110110 + lerp0 * (result11110110 - result01110110);
     float result1110111 =
        result01110111 + lerp0 * (result11110111 - result01110111);
     float result1111000 =
        result01111000 + lerp0 * (result11111000 - result01111000);
     float result1111001 =
        result01111001 + lerp0 * (result11111001 - result01111001);
     float result1111010 =
        result01111010 + lerp0 * (result11111010 - result01111010);
     float result1111011 =
        result01111011 + lerp0 * (result11111011 - result01111011);
     float result1111100 =
        result01111100 + lerp0 * (result11111100 - result01111100);
     float result1111101 =
        result01111101 + lerp0 * (result11111101 - result01111101);
     float result1111110 =
        result01111110 + lerp0 * (result11111110 - result01111110);
     float result1111111 =
        result01111111 + lerp0 * (result11111111 - result01111111);
    
      float result000000 =
        result0000000 + lerp1 * (result1000000 - result0000000);
     float result000001 =
        result0000001 + lerp1 * (result1000001 - result0000001);
     float result000010 =
        result0000010 + lerp1 * (result1000010 - result0000010);
     float result000011 =
        result0000011 + lerp1 * (result1000011 - result0000011);
     float result000100 =
        result0000100 + lerp1 * (result1000100 - result0000100);
     float result000101 =
        result0000101 + lerp1 * (result1000101 - result0000101);
     float result000110 =
        result0000110 + lerp1 * (result1000110 - result0000110);
     float result000111 =
        result0000111 + lerp1 * (result1000111 - result0000111);
     float result001000 =
        result0001000 + lerp1 * (result1001000 - result0001000);
     float result001001 =
        result0001001 + lerp1 * (result1001001 - result0001001);
     float result001010 =
        result0001010 + lerp1 * (result1001010 - result0001010);
     float result001011 =
        result0001011 + lerp1 * (result1001011 - result0001011);
     float result001100 =
        result0001100 + lerp1 * (result1001100 - result0001100);
     float result001101 =
        result0001101 + lerp1 * (result1001101 - result0001101);
     float result001110 =
        result0001110 + lerp1 * (result1001110 - result0001110);
     float result001111 =
        result0001111 + lerp1 * (result1001111 - result0001111);
     float result010000 =
        result0010000 + lerp1 * (result1010000 - result0010000);
     float result010001 =
        result0010001 + lerp1 * (result1010001 - result0010001);
     float result010010 =
        result0010010 + lerp1 * (result1010010 - result0010010);
     float result010011 =
        result0010011 + lerp1 * (result1010011 - result0010011);
     float result010100 =
        result0010100 + lerp1 * (result1010100 - result0010100);
     float result010101 =
        result0010101 + lerp1 * (result1010101 - result0010101);
     float result010110 =
        result0010110 + lerp1 * (result1010110 - result0010110);
     float result010111 =
        result0010111 + lerp1 * (result1010111 - result0010111);
     float result011000 =
        result0011000 + lerp1 * (result1011000 - result0011000);
     float result011001 =
        result0011001 + lerp1 * (result1011001 - result0011001);
     float result011010 =
        result0011010 + lerp1 * (result1011010 - result0011010);
     float result011011 =
        result0011011 + lerp1 * (result1011011 - result0011011);
     float result011100 =
        result0011100 + lerp1 * (result1011100 - result0011100);
     float result011101 =
        result0011101 + lerp1 * (result1011101 - result0011101);
     float result011110 =
        result0011110 + lerp1 * (result1011110 - result0011110);
     float result011111 =
        result0011111 + lerp1 * (result1011111 - result0011111);
     float result100000 =
        result0100000 + lerp1 * (result1100000 - result0100000);
     float result100001 =
        result0100001 + lerp1 * (result1100001 - result0100001);
     float result100010 =
        result0100010 + lerp1 * (result1100010 - result0100010);
     float result100011 =
        result0100011 + lerp1 * (result1100011 - result0100011);
     float result100100 =
        result0100100 + lerp1 * (result1100100 - result0100100);
     float result100101 =
        result0100101 + lerp1 * (result1100101 - result0100101);
     float result100110 =
        result0100110 + lerp1 * (result1100110 - result0100110);
     float result100111 =
        result0100111 + lerp1 * (result1100111 - result0100111);
     float result101000 =
        result0101000 + lerp1 * (result1101000 - result0101000);
     float result101001 =
        result0101001 + lerp1 * (result1101001 - result0101001);
     float result101010 =
        result0101010 + lerp1 * (result1101010 - result0101010);
     float result101011 =
        result0101011 + lerp1 * (result1101011 - result0101011);
     float result101100 =
        result0101100 + lerp1 * (result1101100 - result0101100);
     float result101101 =
        result0101101 + lerp1 * (result1101101 - result0101101);
     float result101110 =
        result0101110 + lerp1 * (result1101110 - result0101110);
     float result101111 =
        result0101111 + lerp1 * (result1101111 - result0101111);
     float result110000 =
        result0110000 + lerp1 * (result1110000 - result0110000);
     float result110001 =
        result0110001 + lerp1 * (result1110001 - result0110001);
     float result110010 =
        result0110010 + lerp1 * (result1110010 - result0110010);
     float result110011 =
        result0110011 + lerp1 * (result1110011 - result0110011);
     float result110100 =
        result0110100 + lerp1 * (result1110100 - result0110100);
     float result110101 =
        result0110101 + lerp1 * (result1110101 - result0110101);
     float result110110 =
        result0110110 + lerp1 * (result1110110 - result0110110);
     float result110111 =
        result0110111 + lerp1 * (result1110111 - result0110111);
     float result111000 =
        result0111000 + lerp1 * (result1111000 - result0111000);
     float result111001 =
        result0111001 + lerp1 * (result1111001 - result0111001);
     float result111010 =
        result0111010 + lerp1 * (result1111010 - result0111010);
     float result111011 =
        result0111011 + lerp1 * (result1111011 - result0111011);
     float result111100 =
        result0111100 + lerp1 * (result1111100 - result0111100);
     float result111101 =
        result0111101 + lerp1 * (result1111101 - result0111101);
     float result111110 =
        result0111110 + lerp1 * (result1111110 - result0111110);
     float result111111 =
        result0111111 + lerp1 * (result1111111 - result0111111);
    
      float result00000 =
        result000000 + lerp2 * (result100000 - result000000);
     float result00001 =
        result000001 + lerp2 * (result100001 - result000001);
     float result00010 =
        result000010 + lerp2 * (result100010 - result000010);
     float result00011 =
        result000011 + lerp2 * (result100011 - result000011);
     float result00100 =
        result000100 + lerp2 * (result100100 - result000100);
     float result00101 =
        result000101 + lerp2 * (result100101 - result000101);
     float result00110 =
        result000110 + lerp2 * (result100110 - result000110);
     float result00111 =
        result000111 + lerp2 * (result100111 - result000111);
     float result01000 =
        result001000 + lerp2 * (result101000 - result001000);
     float result01001 =
        result001001 + lerp2 * (result101001 - result001001);
     float result01010 =
        result001010 + lerp2 * (result101010 - result001010);
     float result01011 =
        result001011 + lerp2 * (result101011 - result001011);
     float result01100 =
        result001100 + lerp2 * (result101100 - result001100);
     float result01101 =
        result001101 + lerp2 * (result101101 - result001101);
     float result01110 =
        result001110 + lerp2 * (result101110 - result001110);
     float result01111 =
        result001111 + lerp2 * (result101111 - result001111);
     float result10000 =
        result010000 + lerp2 * (result110000 - result010000);
     float result10001 =
        result010001 + lerp2 * (result110001 - result010001);
     float result10010 =
        result010010 + lerp2 * (result110010 - result010010);
     float result10011 =
        result010011 + lerp2 * (result110011 - result010011);
     float result10100 =
        result010100 + lerp2 * (result110100 - result010100);
     float result10101 =
        result010101 + lerp2 * (result110101 - result010101);
     float result10110 =
        result010110 + lerp2 * (result110110 - result010110);
     float result10111 =
        result010111 + lerp2 * (result110111 - result010111);
     float result11000 =
        result011000 + lerp2 * (result111000 - result011000);
     float result11001 =
        result011001 + lerp2 * (result111001 - result011001);
     float result11010 =
        result011010 + lerp2 * (result111010 - result011010);
     float result11011 =
        result011011 + lerp2 * (result111011 - result011011);
     float result11100 =
        result011100 + lerp2 * (result111100 - result011100);
     float result11101 =
        result011101 + lerp2 * (result111101 - result011101);
     float result11110 =
        result011110 + lerp2 * (result111110 - result011110);
     float result11111 =
        result011111 + lerp2 * (result111111 - result011111);
    
      float result0000 =
        result00000 + lerp3 * (result10000 - result00000);
     float result0001 =
        result00001 + lerp3 * (result10001 - result00001);
     float result0010 =
        result00010 + lerp3 * (result10010 - result00010);
     float result0011 =
        result00011 + lerp3 * (result10011 - result00011);
     float result0100 =
        result00100 + lerp3 * (result10100 - result00100);
     float result0101 =
        result00101 + lerp3 * (result10101 - result00101);
     float result0110 =
        result00110 + lerp3 * (result10110 - result00110);
     float result0111 =
        result00111 + lerp3 * (result10111 - result00111);
     float result1000 =
        result01000 + lerp3 * (result11000 - result01000);
     float result1001 =
        result01001 + lerp3 * (result11001 - result01001);
     float result1010 =
        result01010 + lerp3 * (result11010 - result01010);
     float result1011 =
        result01011 + lerp3 * (result11011 - result01011);
     float result1100 =
        result01100 + lerp3 * (result11100 - result01100);
     float result1101 =
        result01101 + lerp3 * (result11101 - result01101);
     float result1110 =
        result01110 + lerp3 * (result11110 - result01110);
     float result1111 =
        result01111 + lerp3 * (result11111 - result01111);
    
      float result000 =
        result0000 + lerp4 * (result1000 - result0000);
     float result001 =
        result0001 + lerp4 * (result1001 - result0001);
     float result010 =
        result0010 + lerp4 * (result1010 - result0010);
     float result011 =
        result0011 + lerp4 * (result1011 - result0011);
     float result100 =
        result0100 + lerp4 * (result1100 - result0100);
     float result101 =
        result0101 + lerp4 * (result1101 - result0101);
     float result110 =
        result0110 + lerp4 * (result1110 - result0110);
     float result111 =
        result0111 + lerp4 * (result1111 - result0111);
    
      float result00 =
        result000 + lerp5 * (result100 - result000);
     float result01 =
        result001 + lerp5 * (result101 - result001);
     float result10 =
        result010 + lerp5 * (result110 - result010);
     float result11 =
        result011 + lerp5 * (result111 - result011);
    
      float result0 =
        result00 + lerp6 * (result10 - result00);
     float result1 =
        result01 + lerp6 * (result11 - result01);
    
      float result =
        result0 + lerp7 * (result1 - result0);
    
    
    total += result * amplitude;
    amplitude *= persistence;
     pos0 *= 2;
     pos1 *= 2;
     pos2 *= 2;
     pos3 *= 2;
     pos4 *= 2;
     pos5 *= 2;
     pos6 *= 2;
     pos7 *= 2;
    
  }
  return total;
}

)OPENCLPP"
