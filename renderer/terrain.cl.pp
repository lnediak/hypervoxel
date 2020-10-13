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

float getNoiseImplNUMDIMS(REPEAT IND NUMDIMS float posIND, END;
                          __global float *gradVecs, uint numGradVecs,
                          int numOctaves, float persistence) {
  float total = 0;
  float amplitude = 1;
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

struct TerrainDataNUMDIMS {
  REPEAT IND NUMDIMS int originIND;
  END;
  REPEAT2 BITS VAR NUMDIMS float valueBITS;
  END;
};

typedef struct TerrainDataNUMDIMS TerrainDataNUMDIMS;

void initTerrainDataNUMDIMS(TerrainDataNUMDIMS *this) {
  REPEAT IND NUMDIMS this->originIND = 1073741823;
  END;
  REPEAT2 BITS VAR NUMDIMS this->valueBITS = 0.f;
  END;
}

float getNoiseNUMDIMS(REPEAT IND NUMDIMS float scaleIND, float posIND, END;
                      float lerpSlen, __global float *gradVecs,
                      uint numGradVecs, int numOctaves, float persistence,
                      TerrainDataNUMDIMS *this) {
  REPEAT IND NUMDIMS posIND = (posIND + 0.5) / scaleIND;
  float lerpLocIND = posIND / lerpSlen;
  int lerpLocfIND = fastFloor(lerpLocIND);
  END;

  // updates location of data
  REPEATB IND INV NUMDIMS switch (lerpLocfIND - this->originIND) {
  case 0:
    break;
  case 1:
    this->originIND++;
    REPEAT2 BIT1S VAR1 IND REPEAT2 BIT2S VAR2 INV this->valueBIT1S0BIT2S =
        this->valueBIT1S1BIT2S;
    this->valueBIT1S1BIT2S = getNoiseImplNUMDIMS(
        REPEAT IN2D IND lerpSlen * (this->originIN2D + VAR1IN2D), END;
        lerpSlen * (this->originIND + 1),
        REPEAT IN2D INV lerpSlen *
            (this->originSUB IN2D SUB 4294967295 IND END; END; + VAR2IN2D),
        END;
        gradVecs, numGradVecs, numOctaves, persistence);
    END;
    END;
    break;
  case -1:
    this->originIND--;
    REPEAT2 BIT1S VAR1 IND REPEAT2 BIT2S VAR2 INV this->valueBIT1S1BIT2S =
        this->valueBIT1S0BIT2S;
    this->valueBIT1S0BIT2S = getNoiseImplNUMDIMS(
        REPEAT IN2D IND lerpSlen * (this->originIN2D + VAR1IN2D), END;
        lerpSlen * this->originIND,
        REPEAT IN2D INV lerpSlen *
            (this->originSUB IN2D SUB 4294967295 IND END; END; + VAR2IN2D),
        END;
        gradVecs, numGradVecs, numOctaves, persistence);
    END;
    END;
    break;
  default:
    REPEAT IND NUMDIMS this->originIND = lerpLocfIND;
    END;
    REPEAT2 BITS VAR NUMDIMS this->valueBITS = getNoiseImplNUMDIMS(
        REPEAT IND NUMDIMS lerpSlen * (this->originIND + VARIND), END;
        gradVecs, numGradVecs, numOctaves, persistence);
    END;
  }
  END;

  // performs interpolation
  REPEAT IND NUMDIMS float vec0IND = lerpLocIND - lerpLocfIND;
  float lerpIND = vec0IND * vec0IND * (3 - 2 * vec0IND);
  END;
  REPEAT2 BITS VAR NUMDIMS float resultBITS = this->valueBITS;
  END;
  REPEATB IND INV NUMDIMS REPEAT2 BITS VAR INV float resultBITS =
      result0BITS + lerpIND * (result1BITS - result0BITS);
  END;
  END;
  return result;
}

