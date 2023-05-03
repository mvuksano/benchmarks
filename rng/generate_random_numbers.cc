#include <errno.h>
#include <fcntl.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <gflags/gflags.h>

#include <string>

DEFINE_string(out_dir, "generated", "Directory where to store file with generated numbers");

int main(int argc, char **argv) {
  gflags::ParseCommandLineFlags(&argc, &argv, true);
  std::string filePath = FLAGS_out_dir + "/nums";

  if (mkdir(FLAGS_out_dir.c_str(), 0755) == -1) {
    if (errno == EEXIST) {
      printf("'%s' directory already exists.\n", FLAGS_out_dir.c_str());
    } else {
      printf("Could not create '%s' directory.\n", FLAGS_out_dir.c_str());
      exit(1);
    }
  }

  int nums_fd = open(filePath.c_str(), O_CREAT | O_RDWR | O_TRUNC, 0644);
  if (nums_fd < 0) {
    printf("Could not open file \"nums\".\n");
    exit(1);
  }

  int rand_fd = open("/dev/random", O_RDONLY);
  if (rand_fd < 0) {
	  printf("Could not open /dev/random.");
	  exit(1);
  }

  size_t buf_size = 64 * sizeof(uint32_t);
  char buf[buf_size];

  for (int i = 0; i < 1024 * 1024; i++) {
    if (read(rand_fd, buf, buf_size) != buf_size) {
	    // If we didn't read 64 bytes then discard this block
	    // and try again.
	    i--;
	    continue;
    };
    write(nums_fd, buf, buf_size);
  }
  close(nums_fd);
  close(rand_fd);
  return 0;
}
