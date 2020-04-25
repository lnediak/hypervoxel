#ifndef __OPENCL_VERSION__

#include <cstdint>
typedef uint32_t uint;

#endif // __OPENCL_VERSION__

struct MT19937 {

  uint state[624];
  int index;
};

void mt19937init(MT19937 *mt, uint seed) {
  uint *s = mt->state;
  mt->index = 624;
  *s++ = seed;
  for (int j = 1; j < 624; j++) {
    *s++ = seed = 1812433253 * (seed ^ (seed >> 30)) + j;
  }
}

uint mt19937twist(MT19937 *mt) {
  int i = 0;
  for (; i < 227; i++) {
    uint x = (mt->state[i] & 0x80000000U) | (mt->state[i + 1] & 0x7FFFFFFFU);
    mt->state[i] = mt->state[i + 397] ^ (x >> 1) ^ ((x & 1) * 0x9908b0dfU);
  }
  for (; i < 623; i++) {
    uint x = (mt->state[i] & 0x80000000U) | (mt->state[i + 1] & 0x7FFFFFFFU);
    mt->state[i] = mt->state[i - 227] ^ (x >> 1) ^ ((x & 1) * 0x9908b0dfU);
  }
  uint x = (mt->state[i] & 0x80000000U) | (mt->state[0] & 0x7FFFFFFFU);
  mt->state[i] = mt->state[396] ^ (x >> 1) ^ ((x & 1) * 0x9908b0dfU);
  mt->index = 0;
}

uint mt19937next(MT19937 *mt) {
  if (mt->index >= 624) {
    mt19937twist(mt);
  }
  uint x = mt->state[mt->index++];
  x ^= x >> 11;
  x ^= (x << 7) & 0x9D2C5680;
  x ^= (x << 15) & 0xEFC60000;
  return x ^ (x >> 18);
}

