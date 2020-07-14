#include "FrqStSound.H"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <base/Sprintf.H>

void usage() {
  fprintf(stderr,"Usage: spk2raw < spikefile | sox -t raw -r 44100 -s -w -c 2 - -t wav > wavfile\n");
}

int main(int argc, char **) {

  if (argc>1)
    usage();

  FILE *in = stdin;
  FILE *out = stdout;

  FrqStSound snd;
  
  Spikeinfo ibuf[1024];

  FrqStSound::sam obuf[65536];
  
  while (!feof(in)) {
    int n = fread(ibuf,sizeof(Spikeinfo),1024,in);
    if (n<0) {
      perror("spk2wav: Reading error");
      exit(2);
    } else if (n==0) {
      break;
    }

    fprintf(stderr,"n=%i ch[0]=%i tm[0]=%.6f\n",n,ibuf[0].channel,ibuf[0].time/25000.0);
    
    snd.feedspikes(ibuf,n);

    int m;
    do {
      m = snd.fillstereo(obuf, 65536);
      if (m<65536)
	fwrite(obuf,sizeof(FrqStSound::sam),65536-m,out);
    } while (m==0);
  }

  return 0;
}
