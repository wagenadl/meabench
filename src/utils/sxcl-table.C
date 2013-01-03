/* utils/sxcl-table.C: part of meabench, an MEA recording and analysis tool
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

// sxcl-table.C

#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <tools/SXCLog.H>

void usage() {
  fprintf(stderr,
	  "Usage: sxcl-table -n nbins -t dt_ms -l linbins -f frac -s sel\n"
"\n"
"Reads spikes from stdin as sxclog does, but uses the sxcl-filter output file\n"
"specified with -s flag to collect spikenumbers that make up all\n"
"the statistically significant pairs.\n"
"Output is: blocks of lines, separated by empty lines. First line of each block\n"
"is a verbatim line from sxcl-filter. Subsequent lines are spikenumber pairs\n"
"followed by exact intra-pair-interval in *samples*.\n"
"\n"
"You may want to pipe the sxcl-filter output through sxcl-dedupe to cluster\n"
"together adjacent significant bins before running sxcl-table.\n"
"");  exit(1);
}

class Stats {
public:
  Stats(): cnt(0), expc(0), 
	   c(-1), d(-1), b(-1), t0(0), t1(0) { }
  void add(int cnt0, int exp0=0) { cnt+=cnt0; expc+=exp0; }
  void notify(int ca, int da, int ba, double t0a, double t1a) {
    if (b<0) {
      c=ca; d=da; b=ba; t0=t0a;
    }
    t1=t1a;
  }
  double over() const {
    return (cnt-expc)/sqrt(expc+.0001);
  }
public:
  int cnt;
  double expc;
  int c,d,b;
  double t0,t1;
};

int main(int argc, char **argv) {
  int nbins = 50;
  int dt = 25;
  int linbins = 10;
  float frac = .1;
  char const *sel=0;
  while (1) {
    char c = getopt(argc, argv, "n:t:l:f:s:");
    if (c==-1)
      break;
    switch (c) {
    case 'n': nbins = atoi(optarg); break;
    case 't': dt = int(FREQKHZ * atof(optarg)); break;
    case 'l': linbins = atoi(optarg); break;
    case 'f': frac = atof(optarg); break;
    case 's': sel = optarg; break;
    default: usage();
    }
  }
  if (argc-optind)
    usage();
  if (!sel)
    usage();

  fprintf(stderr,
	  "Settings are:\n  bins: %i\n  dt: %g\n  linbins: %i\n  frac: %g\n",
	  nbins, double(dt)/FREQKHZ, linbins, frac);

  //  map<int, string> class2tag;
  map<BinId, int> binid2class;
  map<int, BinIdSet> class2binids;
  map<int, Stats> class2stats;
  BinIdSet binids;
  FILE *selfh = fopen(sel,"r");
  if (!selfh) {
    perror("sxcl-table: opening sel file");
    exit(2);
  }
  while (1) {
    char buf[1000];
    if (fgets(buf,1000,selfh) != buf)
      break;
    int n,c,d,b,cnt;
    double t0,t1,expc,over;
    if (sscanf(buf,"%i %i %i %i %lg %lg %i %lg %lg",&n,&c,&d,&b,
	       &t0, &t1, &cnt, &expc,&over) < 9) {
      fprintf(stderr,"sxcl-table: Badly formed input line: %s\n",buf);
      exit(1);
    }
    BinId bid(c,d,b);
    binid2class[bid]=n;
    //class2tag[n]=buf;
    class2stats[n].notify(c,d,b,t0,t1);
    class2stats[n].add(cnt,int(expc));
    class2binids[n].insert(bid);
    binids.insert(bid);
  }
  fclose(selfh);
  fprintf(stderr,"Read %i binids\n", int(binids.size()));

  SXCLog xcor(nbins,dt,linbins,frac,binids);

  try {
    xcor.build(stdin);
  } catch (Error const &e) {
    e.report();
    exit(2);
  }

  for (map<int,BinIdSet>::const_iterator c=class2binids.begin();
       c!=class2binids.end(); ++c) {
    int classnr = (*c).first;
    BinIdSet const &bis = (*c).second;
    //    string tag = class2tag[classnr];
    //    printf("%s",tag.c_str());
    Stats const &st = class2stats[classnr];
    printf("%i %i %i %i ", classnr, st.c, st.d, st.b);
    printf("%g %g %i %g %g\n", st.t0, st.t1, st.cnt, st.expc, st.over());
    for (BinIdSet::const_iterator s=bis.begin(); s!=bis.end(); ++s) {
      BinId const &id = *s;
      BinIdMap::const_iterator m=xcor.bim().find(id);
      if (m==xcor.bim().end()) {
	fprintf(stderr,"sxcl-table: no entries for %i: %i.%i.%i!?\n",
		classnr,id.c0,id.c1,id.bin);
	exit(1);
      }
      list<PairId> const &lst=(*m).second;
      for (list<PairId>::const_iterator j=lst.begin(); j!=lst.end(); ++j)
	printf("%i %i %i\n",(*j).from,(*j).to,(*j).dt);
    }
    printf("\n");
  }

  return 0;
}
