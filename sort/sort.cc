#include <algorithm>
#include <fcntl.h>
#include <inttypes.h>
#include <iterator>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <unistd.h>

#include <benchmark/benchmark.h>

#include <avx512-32bit-qsort.hpp>

#define REGISTER_BENCHMARK(NAME, FN) static void NAME(benchmark::State &state) { \
  auto s = state.range(0); \
  int nums_fd = open("generated/nums", O_RDONLY); \
  uint32_t *nums = (uint32_t *)mmap(NULL, 1024 * 1024 * 64 * sizeof(uint32_t), \
                          PROT_READ | PROT_WRITE, MAP_PRIVATE, nums_fd, 0); \
  for (auto _ : state) { \
    FN(nums, s); \
  } \
} \
BENCHMARK(NAME)->RangeMultiplier(2)->Range(8, 8<<23);

void sort_classic(uint32_t *nums, size_t size) {
  std::sort(nums, nums + size);
}

void sort_avx512(uint32_t *nums, size_t size) {
  avx512_qsort<uint32_t>(nums, size);
}

// int main(int argc, char **argv) {
//   uint32_t *nums = NULL;
//   int nums_fd = open("nums", O_RDONLY);
//   if (nums_fd < 0) {
//     printf("Could not open nums file.");
//     exit(1);
//   }
// 
//   nums = (uint32_t *)mmap(NULL, 1024 * 1024 * sizeof(uint32_t),
//                           PROT_READ | PROT_WRITE, MAP_PRIVATE, nums_fd, 0);
//   std::sort(nums, nums + 10);
//   for (int i = 0; i < 10; ++i) {
//     printf("%u\n", nums[i]);
//   }
//   return 0;
// }

REGISTER_BENCHMARK(Sort, sort_classic);
REGISTER_BENCHMARK(AVX512Sort, sort_avx512);

BENCHMARK_MAIN();
