#include <errno.h>
#include <fcntl.h>
#include <immintrin.h>
#include <inttypes.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#include <sys/stat.h>

#define SIZE_1G (1024 * 1024 * 1024)
#define SIZE_1M (1024 * 1024)
#define SIZE_1K (1024)

void generateFile(uint32_t *data, size_t size, char *fileName) {
  int fd = open(fileName, O_CREAT | O_RDWR);
  if (fd < 0) {
    printf("Could not open file \"%s\".\n", fileName);
    exit(1);
  }
  size_t bytes_to_write = size;
  while (bytes_to_write) {
    auto bytes = write(fd, data + (size - bytes_to_write), bytes_to_write);
    bytes_to_write -= bytes;
    if (bytes_to_write == 0) {
      break;
    }
    if (bytes != size) {
      printf("Didn't write all bytes.\n");
      printf("Wrote %ld bytes but expected to write %lu.\n", bytes, size);
    }
  }
  close(fd);
}

bool populateData(uint32_t *data, size_t size) {
  uint32_t val;
  for (size_t i = 0; i < size / sizeof(val); i++) {
#ifdef USE_HW_RAND
    auto res = _rdrand32_step(&val);
#else
    auto res = random();
#endif
    data[i] = val;
  }
  return true;
}

int main(int argc, char **argv) {
  if (mkdir("files", 0755) == -1) {
    if (errno == EEXIST) {
      printf("files directory already exists.\n");
    } else {
      printf("Could not create files directory.\n");
      exit(1);
    }
  }
  uint32_t *data = (uint32_t *)malloc(SIZE_1G);
  if (data == nullptr) {
    printf("Could not allocate large enough array for data.\n");
    exit(1);
  }
  populateData(data, SIZE_1G);

  for (size_t i = 1; i <= SIZE_1G; i = i << 1) {

    char filename[16] = {'\0'};
    if (i / SIZE_1G > 0) {
      sprintf(filename, "files/%luG", i / SIZE_1G);
    } else if (i / SIZE_1M > 0) {
      sprintf(filename, "files/%luM", i / SIZE_1M);
    } else if (i / SIZE_1K > 0) {
      sprintf(filename, "files/%luK", i / SIZE_1K);
    } else {
      sprintf(filename, "files/%lub", i);
    }
    printf("Generating file %s\n", filename);
    generateFile(data, i, filename);
  }

  return 0;
}
