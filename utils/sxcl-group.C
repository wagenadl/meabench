/* utils/sxcl-group.C: part of meabench, an MEA recording and analysis tool
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

// sxcl-group.C

#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <tools/SXCLog.H>

#include <string>
#include <list>

void usage() {
  fprintf(stderr,
	  "Usage: sxcl-group -t thresh < filtin > filtout\n"
"\n"
"Reads output from sxcl-table, and combines groups of adjacent significant\n"
"bins together, by labeling all bins in a group with the classnr of the\n"
"first one.\n"
"If -t THRESH is used, groups that have no bins above significance THRESH\n"
"are silently discarded.\n"
"\n"
"Output is in the same format as for sxcl-filter, except as noted above.\n"
"");
  exit(1);
}

int main(int argc, char **argv) {
  double thresh=0;
  while (1) {
    char c=getopt(argc,argv,"t:");
    if (c==-1)
      break;
    switch (c) {
    case 't': thresh=atof(optarg); break;
    default: usage();
    }
  }
  if (argc-optind)
    usage();

  int classnr;
  int c0=-1, c1=-1, b=-1;
  double t0, t1, cnt, expc, over;
  string group="";
  bool goodgroup=false;
  while (1) {
    char buf[1000];
    if (fgets(buf,1000,stdin)!=buf)
      break;
    int CLASSNR;
    int C0, C1, B; double T0, T1, CNT, EXPC, OVER;
    if (sscanf(buf,"%i %i %i %i %lg %lg %lg %lg %lg", &CLASSNR,
	       &C0,&C1,&B, &T0,&T1, &CNT,&EXPC,&OVER) < 8)
      break;
    bool stayingroup = (C0==c0 && C1==c1 && B==b+1);
    if (stayingroup)
      CLASSNR=classnr; 
    sprintf(buf,"%i %i %i %i %g %g %g %g %g\n",
	    CLASSNR, C0, C1, B, T0, T1, CNT, EXPC, OVER);
    group += buf;
    if (OVER>=thresh)
      goodgroup=true;
    classnr=CLASSNR;
    c0=C0;
    c1=C1;
    b=B;
    t0=T0;
    t1=T1;
    cnt=CNT;
    expc=EXPC;
    over=OVER;
    if (!stayingroup) {
      if (goodgroup)
	printf("%s",group.c_str());
      group="";
      goodgroup=false;
    }
  }
  if (goodgroup)
    printf("%s",group.c_str());
  return 0;
}
