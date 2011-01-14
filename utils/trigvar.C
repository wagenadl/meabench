/* utils/trigvar.C: part of meabench, an MEA recording and analysis tool
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

// trigvar.C

#include <stdio.h>
#include <common/EasyClient.H>
#include <common/ChannelNrs.H>
#include <math.h>

void usage() {
  throw Usage("trigvar","window-ms [raw stream]\nReads from stdin if no arg given. Max window is 1000 ms");
}

class SumALot {
public:
  static const raw_t BASEOFFSET = 2048;
public:
  SumALot(int sams0) {
    sams=sams0;
    for (int c=0; c<NCHANS; c++) {
      sx[c] = new double[sams];
      sxx[c] = new double[sams];
      for (int dt=0; dt<sams; dt++)
	sx[c][dt]=sxx[c][dt]=0;
    }
    cnt = new int[sams];
    for (int dt=0; dt<sams; dt++)
      cnt[dt]=0;
  }
  ~SumALot() {
    for (int c=0; c<NCHANS; c++) {
      delete [] sx[c];
      delete [] sxx[c];
    }
    delete [] cnt;
  }
  void add(int dt, Sample const &s) {
    for (int c=0; c<NCHANS; c++) {
      double v=s[c] - BASEOFFSET;
      sx[c][dt]+=v;
      sxx[c][dt]+=v*v;
    }
    cnt[dt]++;
  }
  void dump() {
    for (int dt=0; dt<sams; dt++) {
      printf("%5.2f ",double(dt)/FREQKHZ);
      for (int c=0; c<NCHANS; c++) {
	double mean = sx[c][dt] / cnt[dt];
	double var = sxx[c][dt] / cnt[dt] - mean*mean;
	printf("%6.1f %6.1f ",mean*341./2048,sqrt(var)*341./2048);
      }
      printf("\n");
    }
  }
private:
  int sams;
  int *cnt;
  double *sx[NCHANS];
  double *sxx[NCHANS];
};

int main(int argc, char **argv) {
  StreamRaw *sr=0;
  int hicnt=0;
  if (argc>3 || argc<2)
    usage();
  int sams=FREQKHZ*atoi(argv[1]);
  SumALot cal(sams);
  Sample *buffer = new Sample[sams];
  try {
    if (argc==3)
      sr = new StreamRaw(argv[2],false);
    else
      sr = new StreamRaw(stdin);
    while (sr->read(buffer,sams)) {
      for (int i=0; i<sams; i++)
	cal.add(i,buffer[i]);
      fprintf(stderr,"Processed %i windows\n",++hicnt);
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

