#include "FrqStSound.H"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <base/Sprintf.H>

void usage() {
  fprintf(stderr,"Usage: spk2wav spikefile [wavfile]\n");
}

int main(int argc, char **argv) {

  if (argc<2 || argc>3)
    usage();

  char const *infn = argv[1];
  char outfn[1000];
  if (argc==3) {
    strcpy(outfn,argv[2]);
  } else {
    strcpy(outfn,infn);
    char *dot = strrchr(outfn,'.');
    if (dot)
      strcpy(dot,".wav");
    else
      strcpy(outfn+strlen(outfn),".wav");
  }

  FILE *in = fopen(infn,"rb");
  if (!in) {
    perror("spk2wav: Cannot open input file");
    exit(1);
  }
  FILE *out = popen(Sprintf("sox -t raw -r 44100 -s -w -c 2 - -t wav %s",outfn).c_str(),"w");
  if (!out) {
    perror("spk2wav: Cannot open sox");
    exit(1);
  }

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
      //      fprintf(stderr,"writing: 65536 - %i\n",m);
      if (m<65536)
	fwrite(obuf,sizeof(FrqStSound::sam),65536-m,out);
    } while (m==0);
  }

  //  fprintf(stderr,"Out of loop\n");

  pclose(out);
  fclose(in);
  return 0;
}
