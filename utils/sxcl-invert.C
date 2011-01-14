/* utils/sxcl-invert.C: part of meabench, an MEA recording and analysis tool
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

// sxcl-invert.C

#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <tools/SXCLog.H>

void usage() {
  fprintf(stderr,
	  "Usage: sxcl-invert -r roleout -t table < spikein > listout\n"
"\n"
"Processes the output of sxcl-table and `inverts' the table, outputting\n"
"a set of roles for every spike in the table. It reads spikes from stdin,\n"
"so height and width are reported for every spike. Context is currently\n"
"not copied.\n"
"\n"
"Output is lines: time_s channel_hw height_digi width_sam tightrole rolemask n\n"
"\n"
"where: TIGHTROLE is the role number of the shortest-ISI pair of which this\n"
"                 spike is a member,\n"
"       ROLEMASK is a bitfield containing all roles that this spike fills,\n"
"       N is the spike number (counting from 0)\n"
"\n"
"Role numbers are mapped to reality in the ROLEOUT file, the lines in\n"
"which contain: channel_hw rolenumber otherchannel bin i_am_first\n"
"\n"
"where: ISI is the typical isi for pairs in this role, not the isi of any\n"
"           one particular pair,\n"
"       I_AM_FIRST is 1 if channel fired before otherchannel, 0 otherwise.\n"
"\n"
"You may want to pipe the sxcl-filter output through sxcl-dedupe to cluster\n"
"together adjacent significant bins before running sxcl-table.\n"
"\n"
"Sxcl-invert uses only the first three columns of sxcl-table block headings\n"
"for its operations. The other columns are copied verbatim to the end of the\n"
"lines in the role output file.\n"
"");
  exit(1);
}

class RoleId: public BinId {
public:
  RoleId(short c=0, short d=0, short b=0, bool cfirst0=true):
    BinId(c,d,b), cfirst(cfirst0) { }
public:
  bool cfirst;
};

bool operator<(RoleId const &a, RoleId const &b) {
  // RoleIds are major sorted by bin number, so tight pairs are indexed first
  if (a.bin<b.bin)
    return true;
  else if (a.bin>b.bin)
    return false;
  if (a.c0<b.c0)
    return true;
  else if (a.c0>b.c0)
    return false;
  if (a.c1<b.c1)
    return true;
  else if (a.c1>b.c1)
    return false;
  return a.cfirst<b.cfirst;
}

class IPISortedRole: public RoleId {
public:
  IPISortedRole(short c=0, short d=0, short b=0, bool cfirst0=true, int dt0=0):
    RoleId(c,d,b,cfirst0), dt(dt0) { }
  IPISortedRole(RoleId const &rid, int dt0=0): RoleId(rid), dt(dt0) { }
public:
  int dt;
};

bool operator<(IPISortedRole const &a, IPISortedRole const &b) {
  if (a.dt<b.dt)
    return true;
  else if (a.dt>b.dt)
    return false;
  return (RoleId const &)(a) < (RoleId const &)(b);
}

class RoleNumber {
public:
  RoleNumber(int ch0=0, int idx0=0): ch(ch0), idx(idx0) { }
public:
  short ch;
  short idx;
};

class RoleIdMap: public map<RoleId, int> {
public:
  RoleIdMap() { num=0; }
  int operator()(RoleId const &a) {
    iterator i=find(a);
    if (i!=end())
      return (*i).second;
    (*this)[a]=num;
    global2local[num] = RoleNumber(a.c0, next(a.c0));
    return num++;
  }
  int next(short channel) {
    int n=chmap[channel];
    chmap[channel]=n+1;
    return n;
  }
public:
  map<int, RoleNumber> global2local;
  // rolemap maps a global role number to a per-channel role number
private:
  int num;
  map<short, short> chmap;
};

class SpikeRoles: public map<int, set<IPISortedRole> > {
  // attach roles to spikes numbers
};

int main(int argc, char **argv) {
  char const *roleout=0;
  char const *table=0;
  while (1) {
    char c = getopt(argc, argv, "r:t:");
    if (c==-1)
      break;
    switch (c) {
    case 'r': roleout=optarg; break;
    case 't': table=optarg; break;
    default: usage();
    }
  }
  if (argc-optind)
    usage();
  if (!table)
    usage();

  FILE *tablefh = fopen(table,"r");
  if (!tablefh) {
    perror("sxcl-invert: cannot open table");
    exit(2);
  }
  FILE *roleoutfh  = roleout?fopen(roleout,"w") : 0;
  if (!roleoutfh && roleout) {
    perror("sxcl-invert: cannot open role output file");
    exit(2);
  }

  RoleIdMap rolemap;
  SpikeRoles spikeroles;

  // Read all known roles & spikes fulfilling them
  fprintf(stderr,"Processing role table...\n");
  while (1) {
    char header[1000];
    *header=0;
    while (*header<' ')
      if (fgets(header,1000,tablefh) != header)
	break;
    if (*header<' ')
      break;
    // got a header line
    int n, c, d, b;
    char rest[1000];
    if (sscanf(header,"%i %i %i %i %[^@]",&n, &c,&d,&b,rest) < 3) {
      fprintf(stderr,"sxcl-invert: wrong format header line: `%s'\n",header);
      exit(3);
    }
    RoleId first_id(c,d,b,true);
    RoleId second_id(d,c,b,false);
    int rolenumber_first = rolemap(first_id); // make sure a number is alloc'ed
    int rolenumber_second = rolemap(second_id);
    if (roleoutfh) {
      fprintf(roleoutfh,"%i %i %i %i %i %s",
	      c,rolemap.global2local[rolenumber_first].idx,d,b,true,rest);
      fprintf(roleoutfh,"%i %i %i %i %i %s",
	      d,rolemap.global2local[rolenumber_second].idx,c,b,false,rest);
    }

    // read fillers of this role
    while (1) {
      char line[1000];
      if (fgets(line,1000,tablefh) != line)
	break;
      if (*line<32)
	break;
      int s0, s1, dt;
      if (sscanf(line,"%i %i %i",&s0,&s1,&dt) != 3) {
	fprintf(stderr,"sxcl-invert: wrong format spike pair line: `%s'\n",
		line);
	exit(3);
      }
      //      fprintf(stderr,"Read pair: %i %i [role %i/%i]\n",s0,s1,rolenumber_first, rolenumber_second);
      spikeroles[s0].insert(IPISortedRole(first_id,dt));
      spikeroles[s1].insert(IPISortedRole(second_id,dt));
    }
  }
  if (!feof(tablefh)) {
    perror("sxcl-invert: error reading table file");
    exit(4);
  }
  if (roleoutfh && ferror(roleoutfh)) {
    perror("sxcl-invert: error writing role out file");
    exit(4);
  }

  // eat through spike file and attach labels
  fprintf(stderr,"Processing spike file...\n");
  int n=0;
  while (1) {
    Spikeinfo si;
    if (fread(&si,SpikeinfoDISKSIZE,1,stdin)!=1)
      break;
    SpikeRoles::iterator i = spikeroles.find(n);
    printf("%g %i %i %i",si.time/(1000.0*FREQKHZ), si.channel,
	   si.height, si.width);
    if (i!=spikeroles.end()) {
      // a labeled spike (possibly multi labeled)
      set<IPISortedRole> const &roles = (*i).second;
      if (roles.begin() == roles.end()) {
	// this shouldn't happen
	fprintf(stderr,"What!?? An empty role set?\n");
	exit(10);
      }
      IPISortedRole primary=*roles.begin();
      printf(" %i",rolemap.global2local[rolemap[primary]].idx);
      unsigned long long mask=0;
      unsigned long long one=1;
      //      int cnt=0;
      //      string boe;
      for (set<IPISortedRole>::const_iterator r=roles.begin();
	   r!=roles.end(); ++r) {
	int idx = rolemap.global2local[rolemap[*r]].idx;
	if (idx<64)
	  mask |= one<<idx;
	//	cnt++;
	//	char buf[100]; sprintf(buf,"%i",idx); boe+=" "; boe+=buf;
      }
      //      if (cnt>1 && si.channel==58)
      //	fprintf(stderr,"Multi role [%i]: %g %i %lli %s\n", cnt,
      //		si.time/(1000.0*FREQKHZ),si.channel,mask,boe.c_str());
      printf(" %llu",mask);
    } else {
      // unlabeled
      printf(" -1 0");
    }
    printf(" %i \n",n);
    n++;
  }
  
  fclose(tablefh);
  if (roleoutfh)
    fclose(roleoutfh);
  return 0;
}
