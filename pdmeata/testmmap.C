/* neurosock/pdmeata/testmmap.C: part of meabench, an MEA recording and analysis tool
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

// testmmap.C

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
  system("lsmod | grep pdmeata|head -1");
  int fd = open("/dev/pdmeata",O_RDONLY);
  system("lsmod | grep pdmeata|head -1");
  if (fd<0) {
    perror("testmmap: cannot open");
    exit(2);
  }

  struct PDMeataConfig config;
  if (ioctl(fd,PDMEATA_IOCTL_GETCONFIG,&config)) {
    perror("testmmap: cannot get config");
    exit(2);
  }
  fprintf(stderr,"nDMASegments: %i\n",config.nDMASegments);
  fprintf(stderr,"bytes/seg: %i\n",config.segmentSize_bytes);
  fprintf(stderr,"scans/seg: %i\n",config.segmentSize_scans);
  fprintf(stderr,"nChannels: %i\n",config.nChannels);
  
  Sample *dmamem=0;
  dmamem = (Sample*)mmap(0, config.nDMASegments*config.segmentSize_bytes,
			 PROT_READ, MAP_SHARED | MAP_FILE, fd, 0);
  if (dmamem == MAP_FAILED) {
    perror("testmmap: cannot mmap\n");
    exit(2);
  }
  system("lsmod | grep pdmeata|head -1");

  fprintf(stderr,"dmamem=%p\n",dmamem);
  
  for (int k=0; k<config.nDMASegments; k+=13) {
    for (int l=0; l<config.segmentSize_scans; l+=220) {
      fprintf(stderr,"Seg %2i Sam %5i : ",k,l);
      for (int n=0; n<4; n++) 
	fprintf(stderr,"%i ",dmamem[k*config.segmentSize_scans+l].data[n]);
      fprintf(stderr,"\n");
    }
  }
  system("lsmod | grep pdmeata|head -1");
  if (munmap(dmamem,config.nDMASegments*config.segmentSize_bytes))
    perror("testmmap: cannot munmap");
  system("lsmod | grep pdmeata|head -1");
  if (close(fd))
    perror("testmmap: cannot close");
  system("lsmod | grep pdmeata|head -1");
  return 0;
}
