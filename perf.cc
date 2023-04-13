#include <benchmark/benchmark.h>
float avx512_sum(float *vals, size_t size) {
    int __attribute__ ((aligned (64))) permute1[] = {1, 0, 3, 2, 5, 4, 7, 6, 9, 8, 11, 10, 13, 12, 15, 14};
    int __attribute__ ((aligned (64))) permute2[] = {2, 3, 0, 1, 6, 7, 4, 5, 11, 10, 8, 9, 14, 15, 12, 13};
    int __attribute__ ((aligned (64))) permute3[] = {4, 5, 6, 7, 0, 1, 2, 3, 12, 13, 14, 15, 8, 9, 10, 11};
    int __attribute__ ((aligned (64))) permute4[] = {8, 9, 10, 11, 12, 13, 14, 15, 0, 1, 2, 3, 4, 5, 6, 7};
    float result = 0;
    float total = 0;
    for(int i = 0; i < size / 16; i++) {
        asm (R"(
            vmovaps	(%2), %%zmm0
            vmovaps	(%1), %%zmm1
            vmovaps	(%1), %%zmm2
            vpermps	%%zmm1, %%zmm0, %%zmm2
            vaddps	%%zmm1, %%zmm2, %%zmm1
            vmovaps	(%3), %%zmm0
            vpermps	%%zmm1, %%zmm0, %%zmm2
            vaddps	%%zmm1, %%zmm2, %%zmm1
            vmovaps	(%4), %%zmm0
            vpermps	%%zmm1, %%zmm0, %%zmm2
            vaddps	%%zmm1, %%zmm2, %%zmm1
            vmovaps	(%5), %%zmm0
            vpermps	%%zmm1, %%zmm0, %%zmm2
            vaddps	%%zmm1, %%zmm2, %%zmm1
            vmovaps	%%zmm1, (%1)
            vmovd	%%xmm1, %0)"
        : "=r" (result)
        : "r" (&vals[i * 16]), "r" (&permute1), "r" (&permute2), "r" (&permute3), "r" (&permute4)
        : "zmm0", "zmm1", "zmm2", "ymm0", "ymm1", "ymm2", "xmm0" , "xmm1", "xmm2");
        total += result;
    }
    return total;
}

float avx_sum(float *vals, size_t size) {
    int __attribute__ ((aligned (32))) permute[] = {0, 4, 1, 5, 2, 6, 3, 7};
    float total = 0;
    float result = 0;
    for (int i = 0; i < size / 8; i++) {
    asm (R"(
            vmovaps	(%1), %%ymm1
            vmovdqa	(%2), %%ymm0
            vhaddps %%ymm1, %%ymm1, %%ymm1
            vhaddps %%ymm1, %%ymm1, %%ymm1
            vpermps %%ymm1, %%ymm0, %%ymm1
            vhaddps %%ymm1, %%ymm1, %%ymm1
            vmovd %%xmm1, %0)"
        : "=r" (result)
        : "r" (&vals[i * 8]), "r" (&permute)
        : "ymm0", "xmm0" , "ymm1", "xmm1");
    	total += result;
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
	for(size_t i = 0; i < size; i++) {
		nums[i] = float(i + 1);
	}
}

static void AvxSum256(benchmark::State& state) {
  float __attribute__ ((aligned (32))) vals[256];
  GenerateNumbers(vals, 256);
  // Code inside this loop is measured repeatedly
  for (auto _ : state) {
    auto res = avx_sum(vals, 256);
    benchmark::DoNotOptimize(res);
  }
}
// Register the function as a benchmark
BENCHMARK(AvxSum256);

static void AvxSum1024(benchmark::State& state) {
  float __attribute__ ((aligned (32))) vals[1024];
  GenerateNumbers(vals, 1024);
  // Code inside this loop is measured repeatedly
  for (auto _ : state) {
    auto res = avx_sum(vals, 1024);
    benchmark::DoNotOptimize(res);
  }
}
// Register the function as a benchmark
BENCHMARK(AvxSum1024);

static void Avx512Sum256(benchmark::State& state) {
  float __attribute__ ((aligned (64))) vals[256];
  GenerateNumbers(vals, 256);
  // Code inside this loop is measured repeatedly
  for (auto _ : state) {
    auto res = avx512_sum(vals, 256);
    benchmark::DoNotOptimize(res);
  }
}
// Register the function as a benchmark
BENCHMARK(Avx512Sum256);

static void Avx512Sum1024(benchmark::State& state) {
  float __attribute__ ((aligned (64))) vals[1024];
  GenerateNumbers(vals, 1024);
  // Code inside this loop is measured repeatedly
  for (auto _ : state) {
    auto res = avx_sum(vals, 1024);
    benchmark::DoNotOptimize(res);
  }
}
// Register the function as a benchmark
BENCHMARK(Avx512Sum1024);

static void Sum256(benchmark::State& state) {
  float __attribute__ ((aligned (32))) vals[256];
  GenerateNumbers(vals, 256);
  // Code inside this loop is measured repeatedly
  for (auto _ : state) {
    auto res = sum(vals, 256);
    benchmark::DoNotOptimize(res);
  }
}
// Register the function as a benchmark
BENCHMARK(Sum256);

static void Sum1024(benchmark::State& state) {
  float __attribute__ ((aligned (32))) vals[1024];
  GenerateNumbers(vals, 1024);
  // Code inside this loop is measured repeatedly
  for (auto _ : state) {
    auto res = sum(vals, 1024);
    benchmark::DoNotOptimize(res);
  }
}
// Register the function as a benchmark
BENCHMARK(Sum1024);

BENCHMARK_MAIN();
