/* neurosock/pdmeata/pdmreset.c: part of meabench, an MEA recording and analysis tool
** Copyright (C) 2000-2002  Daniel Wagenaar (wagenaar@caltech.edu)
**
** This program is free software; you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation; either version 2 of the License, or
** (at your option) any later version.
**
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with this program; if not, write to the Free Software
** Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

// pdmreset.c - simple proggie to reset the pdmeata driver

#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <errno.h>
#include "pdmeata.h"

int main() {
  int fd = open("/dev/pdmeata2", O_RDONLY);
  if (fd<0) {
    perror("pdmreset: cannot open");
    exit(2);
  }
  if (ioctl(fd,PDMEATA_IOCTL_HARDRESET,0) < 0) {
    perror("pdmreset: cannot reset");
    exit(2);
  }
  system("lsmod");

  return 0;
}

    
