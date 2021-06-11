typedef struct Line {

  float8 a, b;
  float3 a3, b3;
  uchar d1, d2;
} Line;

float sigV(float in) { return in >= 0 ? 1.000001 : -0.000001; }

void followLine(const Line *line, __global FacesCacher *f,
                __global TerrainGenerator *g, float dist1, float dist2) {
  if (line->a3.s2 > dist2 || line->b3.s2 < dist1) {
    return;
  }
  float8 a = line->a;
  float8 b = line->b;
  float3 a3 = line->a3;
  float3 b3 = line->b3;

  float8 df = b - a;
  float3 df3 = b3 - a3;
  if (dist1 - a3.s2 >= 1e-4) {
    float offset = (dist1 - a3.s2) / df3.s2;
    a += df * offset;
    a3 += df3 * offset;
  }
  if (b3.s2 - dist2 >= 1e-4) {
    float offset = (dist1 - b3.s2) / df3.s2;
    b -= df * offset;
    b3 -= df3 * offset;
  }

  uchar d1 = line->d1;
  uchar d2 = line->d2;

  int8 ci = fastGetFloor5(a);
  float8 invdf = 1 / df;
  float8 sigdf1 = (float8)(sigV(invdf.s0), sigV(invdf.s1), sigV(invdf.s2),
                           sigV(invdf.s3), sigV(invdf.s4), 0, 0, 0);

  if (getMin5(invdf) >= 1e6) {
    return;
  }
  float dist = getMin5((int52float5(ci) - a + sigdf1) * invdf);
  float8 pos = a + df * dist;
  float3 pos3 = a3 + df3 * dist;
  while (true) {
    double ndist = getMin5((int52float5(ci) - pos + sigdf1) * invdf);
    dist += ndist + 1e-6;
    if (dist >= 1) {
      float3 tmp = a3 + df3 * dist;
      addEdge(f, g, ci, d1, d2, pos3, tmp);
      break;
    }
    if (ndist >= 1e-6) {
      float3 tmp = a3 + df3 * dist;
      if (!addEdge(f, g, ci, d1, d2, pos3, tmp)) {
        break;
      }
      pos3 = tmp;
    }
    pos = a + df * dist;
    ci = fastGetFloor5(pos);
  }
}

