/* neurosock/pdmeata/testindma.C: part of meabench, an MEA recording and analysis tool
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

// testindma.C

#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include "pdmeata.h"

struct Sample {
  short data[64];
};

int main() {
  int fd = open("/dev/pdmeata1",O_RDONLY);
  if (fd<0) {
    perror("testindma: cannot open");
    exit(2);
  }

  long long yes=0;
  long long total=0;
  long long lastprint=0;
  if (ioctl(fd,PDMEATA_IOCTL_INDMA)<0) {
    perror("testindma: INDMA not supported");
    exit(1);
  }
  while (1) {
    total++;
    if (ioctl(fd,PDMEATA_IOCTL_INDMA))
      yes++;
    if (total-lastprint>=1000*1000) {
      lastprint=total;
      fprintf(stderr,"InDMA: %Li out of %Li. [%g]\n",yes,total,(yes+0.0)/total);
    }
  }
  
  if (close(fd)) 
    perror("testindma: cannot close");
  return 0;
}
