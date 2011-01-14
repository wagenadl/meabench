/* utils/getvar1c.C: part of meabench, an MEA recording and analysis tool
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

// getvar1c.C

#include <stdio.h>
#include <stdlib.h>

#include <common/Config.H>

void usage() {
  fprintf(stderr,"Usage: getvar1c window_ms < infile > outfile\n"
	  "Calculates the variance in each window of the input file. Input must be\n"
	  "obtained using onechannel.\n");
  exit(1);
}

void printvar(short *x, int n) {
  int sumx=0;
  for (int i=0; i<n; i++)
    sumx+=x[i];
  double ave=double(sumx)/n;
  double var=0;
  for (int i=0; i<n; i++) {
    double dx=x[i]-ave;
    var+=dx*dx;
  }
  var/=(n-1);
  printf("%g\n",var);
}  

int main(int argc, char **argv) {
  if (argc!=2)
    usage();
  int window_sams = FREQKHZ * atoi(argv[1]);

  short buffer[window_sams];

  while (!feof(stdin)) {
    int n=fread(buffer,2,window_sams,stdin);
    if (n==window_sams)
      printvar(buffer,window_sams);
    else if (n<0) {
      perror("getvar1c"); exit(2);
    } else if (n>0)
      fprintf(stderr,"getvar1c: Warning: incomplete last window ignored\n");
  }
  return 0;
}
