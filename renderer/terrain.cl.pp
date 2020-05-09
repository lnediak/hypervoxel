#ifndef TERRAIN_NON_REP_
#define TERRAIN_NON_REP_

int fastFloor(float x) {
  int i = x;
  return i - (x < i);
}

#endif // TERRAIN_NON_REP_

uint hashNUMDIMS(REPEAT IND NUMDIMS int posIND, END;
                 uint octave, uint numGradVecs) {
  // TODO: WRITE THIS
  return (pos0 * pos0 + pos1 + pos2 * pos2 * pos2 + octave) % numGradVecs;
}

float getNoiseNUMDIMS(REPEAT IND NUMDIMS float scaleIND, float posIND, END;
                      __global float *gradVecs, uint numGradVecs,
                      int numOctaves, float persistence) {
  float total = 0;
  float amplitude = 1;
  REPEAT IND NUMDIMS posIND = (posIND + 0.5) / scaleIND;
  END;
  for (uint i = numOctaves; i--;) {
    REPEAT IND NUMDIMS int posfIND = fastFloor(posIND);
    float vec0IND = posIND - posfIND;
    float vec1IND = vec0IND - 1;
    float lerpIND = vec0IND * vec0IND * (3 - 2 * vec0IND);
    END;
    __global float *tmp;
    REPEAT2 BITS VAR NUMDIMS tmp =
        gradVecs +
        NUMDIMS * hashNUMDIMS(REPEAT IND NUMDIMS posfIND + VARIND, END;
                              i, numGradVecs);
    float resultBITS = REPEAT IND NUMDIMS vecVARINDIND * tmp[IND] + END;
    0;
    END;
    REPEATB IND INV NUMDIMS REPEAT2 BITS VAR INV float resultBITS =
        result0BITS + lerpIND * (result1BITS - result0BITS);
    END;
    END;
    total += result * amplitude;
    amplitude *= persistence;
    REPEAT IND NUMDIMS posIND *= 2;
    END;
  }
  return total;
}
