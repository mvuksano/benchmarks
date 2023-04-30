#include <unistd.h>

float sum(float *vals, size_t size) {
  float result = 0;
  for (int i = 0; i < size; i++) {
    result += vals[i];
  }
  return result;
}


