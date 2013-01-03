/* utils/killspchannel.C: part of meabench, an MEA recording and analysis tool
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

// killspchannel.H

#include <common/EasyClient.H>
#include <common/ChannelNrs.H>

#include <math.h>

void usage() {
  throw Usage("killspchannel","CR-channel [spike stream]\nReads from stdin if no arg given");
}
  
int main(int argc, char **argv) {
  Spikeinfo inbuf[1024];
  Spikeinfo outbuf[1024];
  StreamSpike *sr=0;
  int lowcnt=0, hicnt=0;
  try {
    if (argc<2 || argc>3)
      usage();
    if (argc==3)
      sr = new StreamSpike(argv[2],false);
    else
      sr = new StreamSpike(stdin);

    int hw = cr2hw(argv[1][0]-'1', argv[1][1]-'1');
    
    while (sr->read(inbuf,1024)) {
      int o=0;
      for (int i=0; i<1024; i++) {
	if (inbuf[i].channel != hw)
	  outbuf[o++] = inbuf[i];
      }
      for (int k=0; k<o; k++)
	fwrite(outbuf+k,SpikeinfoDISKSIZE,1,stdout);
      lowcnt+=1024;
      if (lowcnt>25000) {
	lowcnt-=25000;
	fprintf(stderr,"Processed %i kspikes\n",25 * ++hicnt);
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
  delete sr;
  return 0;
}

