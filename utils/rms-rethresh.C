// rms-rethresh.C

#include <stdio.h>
#include <stdlib.h>
#include <base/Description.H>
#include <record/Descriptions.H>
#include <rms/Defs.H>
#include <algorithm>
#include <math.h>

const int MAXWIDTH = 20;
const float PERCENTILE = 0.25;

void usage() {
  fprintf(stderr,"Usage: rms-rethresh thresh width filebase > out.spike\n");
  exit(1);
}

void error(char const *e) {
  perror(e);
  exit(1);
}

float kth_element(float const *src, int N, int k, int stride) {
  static float buf[MAXWIDTH*2+1];
  for (int n=0; n<N; n++)
    buf[n] = src[n*stride];
  std::nth_element(buf,buf+k,buf+N);
  return buf[k];
}

void domedian(RMSValue *dst, RMSValue const *src, int len,
	      float perc, int width) {
  if (sizeof(RMSValue) != TOTALCHANS*sizeof(float))
    throw Error("domedian","Internal error: RMSValue must be pure floats");
    
  for (int l=0; l<width; l++) {
    int l0=0;
    int n=width+l+1;
    int k=int(ceil(perc*n));
    for (int c=0; c<NCHANS; c++)
      dst[l][c] = kth_element(&src[l0].data[c],n,k,TOTALCHANS);
  }
  int n=width*2+1;
  int k=int(ceil(perc*n));
  for (int l=width; l<len-width; l++) {
    int l0=l-width;
    for (int c=0; c<NCHANS; c++)
      dst[l][c] = kth_element(&src[l0].data[c],n,k,TOTALCHANS);
  }
  for (int l=len-width; l<len; l++) {
    int l0=l-width;
    int n = width + (len-l);
    int k = int(ceil(perc*n));
    for (int c=0; c<NCHANS; c++)
      dst[l][c] = kth_element(&src[l0].data[c],n,k,TOTALCHANS);
  }
}

int main(int argc, char **argv) {
  //  switchdbx(1);
  if (argc!=4)
    usage();

  float thresh = atof(argv[1]);
  int width = atoi(argv[1]);
  char const *filebase = argv[3];

  if (width>MAXWIDTH)
    fprintf(stderr,"Warning: Width limited to %i\n",
	    width=MAXWIDTH);
  if (width<=0)
    usage();

  char spikein[1000]; sprintf(spikein,"%s.spike",filebase);
  char rmsinfn[1000]; sprintf(rmsinfn,"%s.rms",filebase);
  char descin[1000]; sprintf(descin,"%s.rms.desc",filebase);

  FILE *spike = fopen(spikein,"rb");
  if (!spike)
    error("rms-rethresh: Cannot open .spike file");
  
  int period_sams=0;
  try {
    ReadDescription d(descin);
    int freqhz = int(atof(d[dRAWSAMFREQ].c_str())*1000);
    if (freqhz==0) {
      freqhz = FREQKHZ*1000;
      //      fprintf(stderr,"Warning: Sample frequency guessed: %i Hz\n",freqhz);
    }
    sdbx("rmsperiod: %s",d[dRMSPERIOD].c_str());
    period_sams = atoi(d[dRMSPERIOD].c_str());
    if (period_sams==0) {
      period_sams = int(freqhz);
      fprintf(stderr,"Warning: Period guessed: %i samples [%.3f s]\n",period_sams,period_sams/(freqhz+0.0));
    }
  } catch (Error &e) {
    e.report();
    exit(1);
  }
  fprintf(stderr,"Period sams: %i\n",period_sams);

  FILE *rmsin = fopen(rmsinfn,"rb");
  if (!rmsin)
    error("rms-rethresh: .rms file not found");
  fseek(rmsin,0,SEEK_END);
  int len = ftell(rmsin) / sizeof(RMSValue);
  fseek(rmsin,0,SEEK_SET);

  sdbx("Reading rms file");
  RMSValue *rms = new RMSValue[len];
  int l=fread(rms,sizeof(RMSValue),len,rmsin);
  if (l!=len) 
    error("rms-rethresh: can't read .rms file");
  fclose(rmsin);
  
  RMSValue *med = new RMSValue[len];
  sdbx("Length: %i\n",len);

  sdbx("Calculating %i-th percentile",int(PERCENTILE*100));
  domedian(med,rms,len,PERCENTILE,width);
//  FILE *o1=fopen("/tmp/o1.rms","wb");
//  fwrite(med,sizeof(RMSValue),len,o1);
//  fclose(o1);
  sdbx("Calculating %i-th percentile",75);
  domedian(rms,med,len,.75,width);
//  FILE *o2=fopen("/tmp/o2.rms","wb");
//  fwrite(rms,sizeof(RMSValue),len,o2);
//  fclose(o2);

  delete [] med;

  sdbx("Working through spike file set");
  
  Spikeinfo si[1];
  int N=-1;
  int in=0, out=0;
  int lastrep=0;
  int seqno=0;
  while (1) {
    N=fread(si,SpikeinfoDISKSIZE,1,spike);
    while (N==0) {
      fclose(spike);
      seqno++;
      char fn[1000]; sprintf(fn,"%s-%i",spikein,seqno);
      fprintf(stderr,"Trying to open next spike file in set: %s...",fn);
      spike=fopen(fn,"rb");
      if (!spike)
	break;
      fprintf(stderr,"OK\n");
      N=fread(si,SpikeinfoDISKSIZE,1,spike);
    }
    if (N<0) {
      perror("rms-rethresh: read error from spike file");
      exit(1);
    } else if (N==0) {
      fprintf(stderr,"EOF\n");
      break;
    }
    for (int n=0; n<N; n++) {
      Spikeinfo &s=si[n];
      int t = s.time / period_sams;
      if (t>=len) {
	// This shouldn't happen
	fprintf(stderr,"Warning: Time %i out of range (%i)\n",t,len);
	t=len-1;
      }
//      int base=0;
//      int s1=0;
//      for (int k=0; k<10; k++) {
//	base+=s.context[k];
//	s1++;
//      }
//      for (int k=50; k<74; k++) {
//	base+=s.context[k];
//	s1++;
//      }
//      base /= s1;
//      sdbx("ch: %i",s.channel);
//      sdbx("hei: %i",s.height);
//      sdbx("thr: %g",thresh);
//      sdbx("t: %i",t);
//      sdbx("rms: %g",rms[t][s.channel]);
//      sdbx(" ");
      s.threshold = raw_t(thresh*rms[t][s.channel]);
      if (s.channel>=NCHANS || abs(s.height) >= s.threshold) {
	fwrite(&s,SpikeinfoDISKSIZE,1,stdout);
	out++;
      }
    }
    in+=N;
    if (in-lastrep>=1000) {
      fprintf(stderr,"Spikes in: %8i. Out: %8i\n",in,out);
      lastrep=in;
    }
  }
  if (spike)
    fclose(spike);
  return 0;
}
