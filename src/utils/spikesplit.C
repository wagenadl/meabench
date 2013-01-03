/* --------------------------------------------------------- compilation ---
g++ -o spikesplit -I/home/wagenaar/meabench spikesplit.C
*/

#include <common/Types.H>

#include <string>
using namespace std;
#include <stdio.h>
#include <stdlib.h>

void usage() {
  fprintf(stderr,"spikesplit -/INFILE [OUTFILEBASE]\n");
  exit(1);
}

void error(char const *s) {
  perror(s);
  exit(2);
}

int main(int argc, char **argv) {
  if (argc<2 || argc>3)
    usage();
  string ifn = argv[1];
  string ofnbase = (argc>2) ? argv[2] : argv[1];
  string::size_type dot = ofnbase.rfind(".");
  ofnbase = (dot==string::npos) ? ofnbase : ofnbase.substr(0,dot);
  ofnbase += ".%i.%s";

  fprintf(stderr,"in: %s - out: %s\n",ifn.c_str(),ofnbase.c_str());
  
  FILE *in = stdin;
  if (argc>=3 && ifn!="-")
    in=fopen(ifn.c_str(),"rb");
  if (!in) error("Can't open input");
  FILE *outtime[60];
  FILE *outshape[60];
  char buffer[1000];
  for (int hw=0; hw<60; hw++) {
    sprintf(buffer,ofnbase.c_str(),hw,"times");
    outtime[hw] = fopen(buffer,"wb");
    if (!outtime[hw]) error("Can't open output 'times'");
    sprintf(buffer,ofnbase.c_str(),hw,"shapes");
    outshape[hw] = fopen(buffer,"wb");
    if (!outshape[hw]) error("Can't open output 'shapes'");
  }

  Spikeinfo spk[1];
  while (!feof(in)) {
    int n=fread(spk,SpikeinfoDISKSIZE,1,in);
    if (n<0)
      error("Error reading spikeinfo");
    else if (n==0)
      break;
    for (int i=0; i<n; i++) {
      int hw=spk[i].channel;
      if (hw<0 || hw>59) {
	fprintf(stderr,"At %.5f: hw=%i\n",spk[i].time / 25000.0,hw);
	continue;
      }
      fprintf(outtime[hw],"%.5f\n",spk[i].time / 25000.0);
      for (int c=0; c<74; c++)
      	fprintf(outshape[hw],"%i%c",spk[i].context[c],c==73?'\n':' ');
    }
  }
  return 0;
}
