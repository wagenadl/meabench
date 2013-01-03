/* utils/idstim.C: part of meabench, an MEA recording and analysis tool
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


#include <stdio.h>
#include <stdlib.h>
#include <common/Types.H>
#include <common/Config.H>
#include <common/ChannelNrs.H>

const int MAXRANGE = 10;

void usage() {
  fprintf(stderr,"Usage: idstim An [prerange [postrange]]] < in.spike > out.text\n"
	  "Defaults: An        = A1\n"
	  "          prerange  = 13:16\n"
	  "          postrange = 39:43\n"
	  "Usage: idstim An trange1 [trange2 ...] < in.spike > out.text\n"
	  "          trange is specified in ms, must have decimal point\n"
	  "            e.g. '-0.4:0.0' to indicate that a 400 us ID pulse\n"
	  "            directly preceded the trigger. Three samples are\n"
	  "            automatically skipped at either end of the range.\n"
	  );
  exit(1);
}

bool range(int a) {
  return a<0 || a>PRECTXT+POSTCTXT;
}

int intcmp(void const *a, void const *b) {
  return (int const *)a-(int const *)b;
}

int main(int argc, char **argv) {
  int hw = CH_ANALOG1;
  int pre[MAXRANGE] = {13, 39};
  int post[MAXRANGE] = {16,43};
  int len[MAXRANGE];
  int nrange=2;

  if (argc<2 || argc>MAXRANGE+2)
    usage();
  try {
    hw = String2hw(argv[1]);
  } catch(Error const &e) {
    e.report();
    usage();
  }
  if (argc>=3) {
    for (int k=2; k<argc; k++) {
      float a,b;
      int n;
      n=sscanf(argv[k],"%g:%g",&a,&b);
      if (a==int(a) && b==int(b)) {
	pre[k-2] = int(a)-1;
	post[k-2] = int(b)-1;
      } else {
	pre[k-2] = int(a/0.040 + 25.5) + 3;
	post[k-2] = int(b/0.040 + 25.5) - 3;
      }
    }
    nrange = argc-2;
  }
  for (int n=0; n<nrange; n++) {
    if (pre[n]<0 || post[n]<pre[n] || post[n]>=74) {
      fprintf(stderr,"idstim: Illegal range given\n");
      usage();
    }
  }

  for (int n=0; n<nrange; n++)
    len[n] = 1+post[n]-pre[n];

  fprintf(stderr,"Channel: %s. Reporting [matlab indices]: ",hw2string(hw).c_str());
  for (int n=0; n<nrange; n++)
    fprintf(stderr,"%i:%i ",pre[n]+1,post[n]+1);
  fprintf(stderr,"\n");
  
  Spikeinfo buffer[1];
  while (!feof(stdin)) {
    int n = fread(buffer,SpikeinfoDISKSIZE,1,stdin);
    if (n<0) {
      perror("idstim: error reading from stdin");
      exit(1);
    } else if (n==0) {
      break;
    }
    for (int m=0; m<n; m++) {
      if (buffer[m].channel == hw) {
	printf("%10.5f",buffer[m].time/(1000.0*FREQKHZ));
	for (int n=0; n<nrange; n++) {
	  int ss[len[n]];
	  for (int i=pre[n]; i<=post[n]; i++)
	    ss[i-pre[n]]=buffer[m].context[i];
	  qsort(ss,len[n],sizeof(int),&intcmp);
	  printf(" %i",ss[len[n]/2]);
	}
	printf("\n");
      }
    }
  }
  return 0;
}
