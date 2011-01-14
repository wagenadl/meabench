/* rejoin.C : part of meabench, an MEA recording and analysis tool
** Copyright (C) 2000-2003  Daniel Wagenaar (wagenaar@caltech.edu)
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

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include <stdio.h>
#include <stdlib.h>

#include <common/Types.H>
#include <common/Config.H>
#include <base/Variance.H>

void usage() {
  fprintf(stderr,"Usage: rejoin filename > output\n");
  exit(1);
}

#define BUFSIZE 16384

int main(int argc, char **argv) {
  if (argc!=2)
    usage();

  char const *namebase = argv[1];
  int fileno=0;
  char buffer[BUFSIZE];
  FILE *infh=fopen(namebase,"rb");
  if (!infh) {
    perror("Cannot open first file");
    exit(2);
  }
  int k=0;
  int l=1024*1024;
  fprintf(stderr,"(%s: File %i)\n",namebase,fileno);
  while (infh) {
    int n=fread(buffer,1,BUFSIZE,infh);
    if (n<0) {
      perror("Read error");
      exit(3);
    } else if (n==0) {
      fclose(infh);
      infh=0;
      fileno++;
      char tmp[1000];
      sprintf(tmp,"%s-%i",namebase,fileno);
      infh = fopen(tmp,"rb");
      if (infh==0) 
	fprintf(stderr,"(There is no %s: File %i)\n",namebase,fileno);
      else
	fprintf(stderr,"(%s: File %i)\n",namebase,fileno);
      k=0; l=1024*1024;
    } else {
      fwrite(buffer,1,n,stdout);
      k+=n;
      if (k>=l) {
	fprintf(stderr,"(%8.1f MB)\r",k/1024.0/1024.0);
	l+=1024*1024;
      }
    }
  }
  return 0;
}

      
