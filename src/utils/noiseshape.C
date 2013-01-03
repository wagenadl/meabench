/* utils/noiseshape.C: part of meabench, an MEA recording and analysis tool
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

// noiseshape.C

#include <common/EasyClient.H>
#include <common/ChannelNrs.H>

#include <math.h>

void usage() {
  throw Usage("noiseshape","[raw stream]\nReads from stdin if no arg given");
}

class SumALot {
public:
  static const int N=4; // max power to collect
public:
  SumALot() {
    for (int c=0; c<NCHANS; c++)
      for (int n=0; n<N; n++)
	sum[c][n]=0;
    cnt=0;
  }
  void add(Sample const &s) {
    for (int c=0; c<NCHANS; c++) {
      double v=s[c];
      double a=v;
      for (int n=0; n<N; n++) {
	sum[c][n]+=a;
	a*=v;
      }
    }
    cnt++;
  }
  void dump() {
    for (int c=0; c<NCHANS; c++) {
      int col, row; hw2cr(c,col,row);
      for (int n=0; n<N; n++)
	sum[c][n]/=cnt;
      double mean = sum[c][0];
      double var = sum[c][1]-pow(sum[c][0],2);
      double skew = sum[c][2]-3*sum[c][1]*mean+2*pow(mean,3);
      double kurt = sum[c][3]-4*sum[c][2]*mean+6*sum[c][1]*pow(mean,2)-3*pow(mean,4);
      printf("%2i %8.2f %10.3f %10.5f %10.5f %i%i\n",c,mean,var,
	     skew/pow(var,1.5),
	     kurt/pow(var,2),col+1,row+1);
    }
  }
private:
  double sum[NCHANS][N];
  int cnt;
};

  
int main(int argc, char **argv) {
  SumALot cal;
  Sample buffer[1024];
  StreamRaw *sr=0;
  int lowcnt=0, hicnt=0;
  try {
    if (argc>2)
      usage();
    if (argc==2)
      sr = new StreamRaw(argv[1],false);
    else
      sr = new StreamRaw(stdin);
    while (sr->read(buffer,1024)) {
      for (int i=0; i<1024; i++)
	cal.add(buffer[i]);
      lowcnt+=1024;
      if (lowcnt>25000) {
	lowcnt-=25000;
	fprintf(stderr,"Processed %i s\n",++hicnt);
      }
    }
  } catch (Expectable const &e) {
    // Probably EOF
    e.report();
  } catch (Error const &e) {
    e.report();
    return 1;
  } catch (...) {
    fprintf(stderr,"Unknown exception\n");
    return 2;
  }
  cal.dump();
  delete sr;
  return 0;
}

