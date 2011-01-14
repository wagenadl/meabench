// hardrelease.C

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <stdlib.h>
#include <stdio.h>

#include "MCCard.h"

int main(int argc, char **argv) {
  int fd = open("/dev/MCCard",O_RDONLY);
  if (fd<0) {
    perror("hardrelease: can't open device driver");
    exit(1);
  }
  ioctl(fd,MCCARD_IOCHARD_RELEASE,0);
  return 0;
}
