/* utils/trialdump.C: part of meabench, an MEA recording and analysis tool
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

// trialdump.C

#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>

#include <common/Types.H>
#include <common/Config.H>

void usage() {
  fprintf(stderr,"Usage: trialdump -An -h -O [spikefile]\n");
  exit(1);
}

int main(int argc, char **argv) {
  bool reorder = false;
  bool header = false;
  int An = 60;
  FILE *src = stdin;
  char const *fn = 0;
  while (1) {
    char c = getopt(argc, argv,"OhA:");
    if (c==-1)
      break;
    switch (c) {
    case 'O': reorder = true; break;
    case 'h': header = true; break;
    case 'A': An = 60 + *optarg-'1';
      if (An<60 | An>63) usage();
      break;
    default: usage();
    }
  }
  if (argc-optind) 
    fn = argv[optind++];
  if (argc-optind)
    usage();
  
  if (reorder) {
    char cmd[1000];
    sprintf(cmd,"spikeorder %s",fn?fn:"");
    src = popen(cmd,"r");
  } else if (fn) {
    src = fopen(fn,"rb");
  }

  if (!src) {
    perror("trialdump");
    exit(2);
  }

  fprintf(header?stdout:stderr,
	  "# Time[s] ch[hw] height[digi] width[sams] trial rel-time [s]\n");
  int trial = 0;
  timeref_t trialstart = 0;
  while (!feof(src)) {
    Spikeinfo si;
    int n=fread(&si,SpikeinfoDISKSIZE,1,src);
    if (n<0)
      perror("spikedump");
    else if (n==0)
      break; // eof
    if (si.channel == An) {
      trial++;
      trialstart = si.time;
    }
    printf("%10.5f %2i %6i %5i %5i %10.5f\n",si.time/(FREQKHZ*1000.),si.channel,si.height,si.width,trial,(si.time-trialstart)/(FREQKHZ*1000.));
  }
  if (reorder)
    pclose(src);
  else if (fn)
    fclose(src);
  
  return 0;
}
