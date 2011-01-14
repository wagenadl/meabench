/* utils/spikeorder.C: part of meabench, an MEA recording and analysis tool
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

// spikeorder.C

#include <stdio.h>
#include <stdlib.h>
#include <set>
#include <common/Types.H>
#include <base/Error.H>
#include <base/Sprintf.H>

const int QUEUELENGTH = 1000;

void usage() {
  fprintf(stderr,"Usage: spikeorder [filename]\n\n"
	  "Reads a spike file from a file or stdin and writes it to stdout reordering\n"
	  "spikes in proper time order.\n");
  exit(1);
}

class SI_TimeChannel {
public:
  bool operator()(Spikeinfo const &a, Spikeinfo const &b) {
    if (a.time<b.time)
      return true;
    else if (a.time>b.time)
      return false;
    else
      return a.channel<b.channel;
  }
};

int main(int argc, char **argv) {
  FILE *in = stdin;
  if (argc<1 || argc>2 || (argc==2 && argv[1][0]=='-'))
    usage();

  if (argc==2) //  && argv[1][0]!='0')
    in = fopen(argv[1],"rb");

  int fno=0;

  try {
    if (!in)
      throw SysErr("spikeorder","Cannot open input");
    set<Spikeinfo,SI_TimeChannel> queue;
    Spikeinfo si;
    
    // read stuff to fill queue
    for (int n=0; n<QUEUELENGTH; n++) {
      if (fread(&si,SpikeinfoDISKSIZE,1,in)==1)
	queue.insert(queue.end(),si);
      else
	break;
    }
    if (ferror(in))
      throw SysErr("spikeorder","Cannot read input");
    
    // read and write until eof
    while (1) {
      while (fread(&si,SpikeinfoDISKSIZE,1,in)==1) {
	queue.insert(queue.end(),si);
	fwrite(&*queue.begin(),SpikeinfoDISKSIZE,1,stdout);
	queue.erase(queue.begin());
      }
      if (ferror(in))
	throw SysErr("spikeorder","Cannot read input");
      if (in!=stdin) {
	fclose(in);
	fno++;
	in = fopen(Sprintf("%s-%i",argv[1],fno).c_str(),"rb");
	if (in) 
	  fprintf(stderr,"Continuing with file %s-%i\n",argv[1],fno);
	else
	  break;
      } else {
        break;
      }
    }
    
    // write last bits
    for (set<Spikeinfo,SI_TimeChannel>::iterator i=queue.begin();
	 i!=queue.end(); ++i)
      fwrite(&*i,SpikeinfoDISKSIZE,1,stdout);
    
  } catch (Error const &e) {
    e.report();
    exit(2);
  }
  
  return 0;
}
