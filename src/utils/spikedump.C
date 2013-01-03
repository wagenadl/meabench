/* utils/spikedump.C: part of meabench, an MEA recording and analysis tool
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

// spikedump.C

#include <stdio.h>
#include <stdlib.h>

#include <common/Types.H>
#include <common/Config.H>

void usage() {
  fprintf(stderr,"Usage: spikedump [spikefile]\n");
  exit(1);
}

int main(int argc, char **argv) {
  FILE *src = stdin;
  if (argc>2)
    usage();
  else if (argc>1)
    src = fopen(argv[1],"rb");
  if (!src) {
    perror("spikedump");
    exit(1);
  }

  setvbuf(stdout,0,_IONBF,0);

  
  printf("# Time[s] ch[hw] height[digi] width[sams] thresh[digi]\n");
  while (!feof(src)) {
    Spikeinfo si;
    int n=fread(&si,SpikeinfoDISKSIZE,1,src);
    if (n<0)
      perror("spikedump");
    else if (n==0)
      break; // eof
    printf("%10.5f %2i %6i %5i %6i\n",si.time/(FREQKHZ*1000.),si.channel,si.height,si.width,si.threshold);
  }
  fclose(src);
  return 0;
}
