/* utils/noisehisto.C: part of meabench, an MEA recording and analysis tool
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

// noisehisto.C

#include <common/EasyClient.H>

void usage() {
  throw Usage("noisehisto","[raw stream]\nReads from stdin if no arg given");
}

class CountALot {
public:
  static const int V=4096;
public:
  CountALot() {
    for (int c=0; c<NCHANS; c++)
      for (int v=0; v<V; v++)
	cnt[c][v]=0;
    n=0;
  }
  void add(Sample const &s) {
    for (int c=0; c<NCHANS; c++) {
      cnt[c][s[c]]++;
    }
    n++;
  }
  void dump() {
    int min,max;
    // foll. is a stupid way to find min and max values
    int total[V];
    for (int v=0; v<V; v++) {
      int n=0;
      for (int c=0; c<NCHANS; c++)
	n+=cnt[c][v];
      total[v]=n;
    }
    min=0;
    while (min<V)
      if (total[min]>100)
	break;
      else
	min++;
    max=V-1;
    while (max>=0)
      if (total[max]>100)
	break;
      else
	max--;
    // junk ends here
    
    for (int v=min; v<=max; v++) {
      printf("%4i ",v);
      for (int c=0; c<NCHANS; c++)
	printf("%.6f ",double(cnt[c][v])/n);
      printf("\n");
    }
  }
private:
  int cnt[NCHANS][V];
  int n;
};

  
int main(int argc, char **argv) {

  CountALot cal;
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

