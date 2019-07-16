DEFINE RENDER(NUMDIMS)
  void normalize(VECTORIZE(NUMDIMS)(float *vecVEC, ), int) {
    float norm = 0;
    VECTORIZE(NUMDIMS)(norm += (*vecVEC) * (*vecVEC);)
    norm = sqrt(norm);
    VECTORIZE(NUMDIMS)(*vecVEC /= norm;)
  }

  bool rayBBoxNUMDIMS(VECTORIZE(NUMDIMS)(float posVEC, float dirVEC,
                                         float origVEC, float lenVEC,), int) {
    // TODO: IMPLEMENT
  }

  void traverseBVHNUMDIMS(unsigned char *bvh,
                          VECTORIZE(NUMDIMS)(float posVEC, float dirVEC,)
                          unsigned char *img, float *dist) {
    
  }

  __kernel void renderNUMDIMS(unsigned char *bvh, float *vectors,
      unsigned char *img, float *dist) {
    size_t row = get_global_id(0);
    size_t col = get_global_id(1);
    size_t height = get_global_size(0);
    size_t width = get_global_size(1);
    float xoff = (col + 0.0) / width - 0.5;
    float yoff = (row + 0.0) / height - 0.5;
    VECTORIZE(NUMDIMS)(float posVEC = vectors[VEC];
        float dirVEC = vectors[NUMDIMS + VEC] +
        xoff * vectors[2 * NUMDIMS + VEC] +
        yoff * vectors[3 * NUMDIMS + VEC];)
    normalize(VECTORIZE(NUMDIMS)(&dirVEC, ), 0);
    size_t poff = row * width + col;
    img += poff * 4;
    dist += poff;
  }
ENDEFINE
