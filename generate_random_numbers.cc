#include <fcntl.h>
#include <immintrin.h>
#include <inttypes.h>
#include <stdio.h>
#include <unistd.h>

int main(int argc, char **argv) {
  int nums_fd = open("nums", O_CREAT | O_RDWR | O_TRUNC);
  if (nums_fd < 0) {
    printf("Could not open file \"nums\".\n");
    exit(1);
  }

  for (int i = 0; i < 1024 * 1024; i++) {
    uint64_t val;
    auto res = _rdrand64_step(&val);
    write(nums_fd, &val, sizeof(val));
  }
  close(nums_fd);
  return 0;
}
