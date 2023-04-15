#include <benchmark/benchmark.h>

#define REGISTER_BENCHMARK(NAME, FN) static void NAME(benchmark::State &state) { \
  auto s = state.range(0); \
  float __attribute__((aligned(64))) vals[s]; \
  GenerateNumbers(vals, s); \
  for (auto _ : state) { \
    auto res = FN(vals, s); \
    benchmark::DoNotOptimize(res); \
  } \
} \
BENCHMARK(NAME)->RangeMultiplier(2)->Range(8, 8<<17);

float avx512_sum(float *vals, size_t size) {
  int __attribute__((aligned(64)))
  permute1[] = {1, 0, 3, 2, 5, 4, 7, 6, 9, 8, 11, 10, 13, 12, 15, 14};
  int __attribute__((aligned(64)))
  permute2[] = {2, 3, 0, 1, 6, 7, 4, 5, 11, 10, 8, 9, 14, 15, 12, 13};
  int __attribute__((aligned(64)))
  permute3[] = {4, 5, 6, 7, 0, 1, 2, 3, 12, 13, 14, 15, 8, 9, 10, 11};
  int __attribute__((aligned(64)))
  permute4[] = {8, 9, 10, 11, 12, 13, 14, 15, 0, 1, 2, 3, 4, 5, 6, 7};
  float result = 0;
  float total = 0;
  float __attribute__((aligned(64))) temp[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  asm(R"(vmovdqa32   (%0), %%zmm0)"
      : /* no outputs */
      : "r"(&temp)
      : "zmm0", "ymm0", "xmm0");

  for (int i = 0; i < size / 16; i++) {
    asm(R"(
            vmovdqa32   (%0), %%zmm1
            vaddps %%zmm0, %%zmm1, %%zmm0)"
        : /* no outputs */
        : "r"(&vals[i * 16])
        : "zmm0", "ymm0", "xmm0");
  }
  asm(R"(vmovd	%%xmm1, %0)"
      : "=m"(temp) 
      : /* no inputs */ );
  for (int i = 0; i < 16; i++) {
	  total += temp[i];
  }
  return total;
}

float avx_sum(float *vals, size_t size) {
  int __attribute__((aligned(32))) permute[] = {0, 4, 1, 5, 2, 6, 3, 7};
  float __attribute__((aligned(32))) temp[] = {0, 0, 0, 0, 0, 0, 0, 0};
  float total = 0;
  float result = 0;
  asm(R"(vmovaps (%0), %%ymm0)"
	: /* outputs */
	: "r"(&temp) 
	: "xmm0", "ymm0");
  for (int i = 0; i < size / 8; i++) {
    asm(R"(
            vmovaps  (%0), %%ymm1
            vhaddps %%ymm0, %%ymm1, %%ymm0)"
        : /* no outputs */ 
        : "r"(&vals[i * 8])
        : "ymm0", "xmm0");
  }
  asm(R"(vmovaps %%ymm1, %0)"
	: "=m"(temp) 
	: /* no inputs */
	: /* clobbers */);

  for (int i = 0; i < 8; i++) {
	total += temp[i];
  }
  return total;
}

float sum(float *vals, size_t size) {
  float result = 0;
  for (int i = 0; i < size; i++) {
    result += vals[i];
  }
  return result;
}

static void GenerateNumbers(float *nums, size_t size) {
  for (size_t i = 0; i < size; i++) {
    nums[i] = float(i + 1);
  }
}

REGISTER_BENCHMARK(AVXSum, avx_sum);
REGISTER_BENCHMARK(AVX512Sum, avx512_sum);
REGISTER_BENCHMARK(SUM, sum);

BENCHMARK_MAIN();
