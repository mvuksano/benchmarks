#include <fcntl.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char **argv) {
  int nums_fd = open("nums", O_CREAT | O_RDWR | O_TRUNC, 0644);
  if (nums_fd < 0) {
    printf("Could not open file \"nums\".\n");
    exit(1);
  }

  int rand_fd = open("/dev/random", O_RDONLY);
  if (rand_fd < 0) {
	  printf("Could not open /dev/random.");
	  exit(1);
  }

  char buf[64];

  for (int i = 0; i < 1024 * 1024; i++) {
    if (read(rand_fd, buf, 64) != 64) {
	    // If we didn't read 64 bytes then discard this block
	    // and try again.
	    i--;
	    continue;
    };
    write(nums_fd, buf, 64);
  }
  close(nums_fd);
  close(rand_fd);
  return 0;
}
