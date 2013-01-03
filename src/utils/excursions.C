/* excursions.C - Detect excursions through preset thresholds */

/* Usage: excursions THR_L0 THR_H0 THR_L1 ... THR_H63

   Reads a raw file from stdin and writes a binary file to stdout documenting
   excursions through the given thresholds.
   Example: cat foo.raw foo.raw-* | excursions `cat thresholds.txt` > foo.exc
   Output is a binary file of fields of 24 bytes each:
     +0  [int64] timestamp
     +8  [int64] channel
     +16 [int64] value
   Of course it would have been more efficient to pack this more tightly, but
   I wanted it to be easy to read it into matlab.
*/

#include <stdio.h>
#include <stdlib.h>

const size_t NCHANS = 64;
const size_t BUFLEN = 128;

void usage() {
  fprintf(stderr,"Usage: excursions THR_L0 THR_H0 THR_L1 ... THR_H63\n");
  exit(1);
}

void output(int k, long long ipeak, short peak) {
  long long record[3];
  record[0] = ipeak;
  record[1] = k;
  record[2] = peak;
  fwrite(record, 24, 1, stdout);
}

int main(int argc, char **argv) {
  if (argc!=129)
    usage();

  short lowthr[NCHANS];
  short highthr[NCHANS];
  int state[NCHANS]; // +1 for above, -1 for below, 0 for between threshs
  short peak[NCHANS]; // only defined if above or below threshold
  long long ipeak[NCHANS];
  
  for (int k=0; k<NCHANS; k++) {
    lowthr[k]=atoi(argv[2*k+1]);
    highthr[k]=atoi(argv[2*k+2]);
    state[k] = 0;
  }

  short *buffer[NCHANS*BUFLEN];
  size_t now;
  long long time = 0;
  do {
    now = fread(buffer, 2*NCHANS, BUFLEN, stdin);
    if (now<0) {
      perror("File error");
    } else if (now==0) {
      break;
    }
    short *ptr = buffer;
    for (int t=0; t<now; t++) {
      for (int k=0; k<NCHANS; k++) {
	if (*ptr>=highthr[k]) {
	  if (state[k]<0)
	    output(k, ipeak[k], peak[k]);
	  if (state[k]<=0 || *ptr>=peak[k]) {
	    state[k]=1;
	    peak[k]=*ptr;
	    ipeak[k] = time;
	  }
	} else if (*tr<lowthr[k]) {
	  if (state[k]>0)
	    output(k, ipeak[k], peak[k]);
	  if (state[k]>=0 || *ptr<=peak[k]) {
	    state[k]=1;
	    peak[k]=*ptr;
	    ipeak[k] = time;
	  }
	}
	ptr ++;
      }
      time ++;
    }
  }
  return 0;
}
