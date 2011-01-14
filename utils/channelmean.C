/* utils/channelmean.C: part of meabench, an MEA recording and analysis tool
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

// channelmean.C

#include <stdio.h>
#include <getopt.h>
#include <stdlib.h>
#include <common/Types.H>
#include <common/Config.H>

void usage() {
  fprintf(stderr,"Usage: channelmean -p period_ms -d delay_ms -l length_ms\n");
  exit(1);
}

int main(int argc, char **argv) {
  int period_sams=0;
  int delay_sams=0;
  int length_sams=0;
  while (1) {
    char c = getopt(argc, argv, "p:d:l:");
    if (c==-1)
      break;
    switch (c) {
    case 'p': period_sams = int(FREQKHZ * atof(optarg)); break;
    case 'd': delay_sams = int(FREQKHZ * atof(optarg)); break;
    case 'l': length_sams = int(FREQKHZ * atof(optarg)); break;
    default: usage();
    }
  }
  if (optind<argc)
    usage();
  if (period_sams==0 || length_sams==0)
    usage();

  Sample *data = new Sample[period_sams];
  
  while (!feof(stdin)) {
    int n=fread(data,sizeof(Sample),period_sams,stdin);
    if (n<0) {
      perror("channelmean");
      exit(2);
    } else if (n==0) {
      break;
    } else if (n<delay_sams+length_sams) {
      fprintf(stderr,"Partial last window ignored\n");
      break;
    }
    for (int hw=0; hw<NCHANS; hw++) {
      int sum=0;
      for (int t=delay_sams; t<delay_sams+length_sams; t++)
	sum+=data[t][hw];
      printf("%g ",1.*sum/length_sams);
    }
    printf("\n");
  }
  return 0;
}
