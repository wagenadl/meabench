/* tools/SXCLog.C: part of meabench, an MEA recording and analysis tool
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

// SXCLog.C

#include "SXCLog.H"
#include "MultiTimeQueue.H"
#include "SpikeQueue.H"

SXCLog::SXCLog(int nbins0, int dt0, int linbins0, float incr0,
	       BinIdSet const &bis):
  nbins(nbins0), dt(dt0), tbase(dt0*linbins0),
  linbins(linbins0), logbase(log(1+incr0)) {
  maxt=int(tbase*exp(logbase*(nbins-linbins)));
  for (int c0=0; c0<TOTALCHANS; c0++) {
    totals[c0]=0;
    for (int c1=0; c1<TOTALCHANS; c1++) {
      data[c0][c1] = new int[nbins];
      for (int n=0; n<nbins; n++)
	data[c0][c1][n]=0;
    }
  }
  for (BinIdSet::const_iterator i=bis.begin(); i!=bis.end(); ++i)
    bim_[*i]=list<PairId>();
}

void SXCLog::report() {
  fprintf(stderr,"nbins=%i\n",nbins);
  fprintf(stderr,"dt=%i\n",dt);
  fprintf(stderr,"tbase=%i\n",tbase);
  fprintf(stderr,"linbins=%i\n",linbins);
  fprintf(stderr,"maxt=%i\n",maxt);
  fprintf(stderr,"logbase=%g\n",logbase);
}

SXCLog::~SXCLog() {
  for (int c0=0; c0<TOTALCHANS; c0++)
    for (int c1=0; c1<TOTALCHANS; c1++)
      delete [] data[c0][c1];
}

#define DROPIVAL 20

void SXCLog::build(FILE *fh) throw(Error) {
  MultiTimeIdQueue mtq;
  Spikeinfo si;
  int waiti=DROPIVAL;
  t_begin=t_end=0;
  bool nobegin=true;
  timeref_t now=0;
  while (fread(&si,sizeof(si),1,fh)==1) {
    now = si.time;
    if (nobegin) {
      t_begin=now;
      nobegin=false;
    }
    if (now>maxt && --waiti==0) {
      mtq.drop_until(now-maxt);
      waiti=DROPIVAL;
    }
    totals[si.channel]++;
    for (int c0=0; c0<TOTALCHANS; c0++) {
      for (TimeIdQueue::iterator i=mtq.begin(c0); i!=mtq.end(c0); ++i) {
	timeref_t then = (*i).first;
	if (then>now)
	  break;
	BinId bid=add(c0,si.channel,now-then);
	BinIdMap::iterator m = bim_.find(bid);
	if (m!=bim_.end())
	  (*m).second.push_back(PairId((*i).second, mtq.now(), now-then));
      }
    }
    mtq.enqueue(si);
  }
  t_end = now;
  if (feof(fh))
    return;
  if (ferror(fh))
    throw SysErr("SXCBuilder","Cannot read file");
}
