#include <stdio.h>
#include <fcntl.h>
#include <linux/types.h>
#include <sys/ioctl.h>
#include <stdlib.h>
#include <unistd.h>

#include "MCCard.h"

void usage() {
  fprintf(stderr,"Usage: forcefifo n\n");
  exit(2);
}

int main(int argc, char **argv) {
  if (argc!=2)
    usage();

  int fd = open("/dev/MCCard", O_RDONLY);
  if (fd<0) {
    perror("Cannot open MCCard");
    exit(2);
  }

  int n = atoi(argv[1]);
  if (ioctl(fd, MCCARD_IOCFORCEFIFO,  &n)) {
    perror("Can't ioctl FORCEFIFO");
    exit(2);
  }

  close(fd);
  return 0;
}
