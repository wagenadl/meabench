/* tools/SpikeXCorrel.C: part of meabench, an MEA recording and analysis tool
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

// SpikeXCorrel.C

#include "SpikeXCorrel.H"
#include "MultiTimeQueue.H"
#include "SpikeQueue.H"

SpikeXCorrel::SpikeXCorrel(int nbins0, int dt0):
  nbins(nbins0), dt(dt0) {
  maxt=nbins*dt;
  for (int c0=0; c0<TOTALCHANS; c0++) {
    totals[c0]=0;
    for (int c1=0; c1<TOTALCHANS; c1++) {
      data[c0][c1] = new int[nbins];
      for (int n=0; n<nbins; n++)
	data[c0][c1][n]=0;
    }
  }
}

SpikeXCorrel:: ~SpikeXCorrel() {
  for (int c0=0; c0<TOTALCHANS; c0++)
    for (int c1=0; c1<TOTALCHANS; c1++)
      delete [] data[c0][c1];
}

#define DROPIVAL 20

void SpikeXCorrel::build(FILE *fh)  {
  MultiTimeQueue mtq;
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
      for (TimeQueue::iterator i=mtq.begin(c0); i!=mtq.end(c0); ++i) {
	timeref_t then = (*i);
	if (then>now)
	  break;
	add(c0,si.channel,now-then);
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

double SpikeXCorrel::cov(int c0, int c1, int bin) const {
  double nxy=data[c0][c1][bin];
  double nx=totals[c0];
  double ny=totals[c1];
  double intt = totaltime();
  return nxy/(dt*intt) - (nx*ny)/(intt*intt);
}

/* Here's the mathematics (in tex):

   \def\dtau{{\delta\tau}}

   Let $t_{xn}$ (for $n=1..N_x$) be the arrival times on channel $x$.

   Let $\dtau$ be the bin size.

   Define: $x(t) = {1\over\dtau}
                   \int_0^\dtaudt' \delta(t_{xn} - t - t')$.

   By definition:
   \[
   cov_\tau(x,y) = {\int dt x(t) y(t+\tau) \over \int dt} 
                    - {\int dt x(t) \over \int dt} {\int dt y(t) \over \int dt}
   \]
   count(x,y,bin) contains:
   \[
   N_{xy}^\tau = \sum_{n,m} \int_0^\dtau \dt'
     \delta(t_{xn}-t{ym} - \tau-t'},
   \]
   where $\tau = bin * \dtau$.

   Note that:
   \[
   \int dt x(t)
   = \int dt \int_0^\dtau dt' \sum_n \delta(t_{xn} - t-t') / \dtau \\
   = \int_0^\dtau \sum_n {1\over\dtau} = N_x.
   \]
   This is contained in count(x).

   Now calculate:
   \[ (\dtau)^2 \int dt x(t) x(t+\tau)
      = \int dt \int_0^\dtau dt' \int_0^\dtau dt'' \sum_{n,m}
        \delta(t_{xn} - t - t') \delta(t_{yn}+\tau - t - t'') \\
      = \int_0^\dtau dt' \int_0^\dtau dt'' \sum_{n,m}
        \delta(t_{yn} - (t_{xn} - t') - t'') \\
      = \int_0^\dtau dt'' \sum_{n,m} \int_0^\dtau dt'
        \delta(t_{xn} - t_{yn} - t' + t'' - \tau) \\
      = \int_0^\dtau dt'' N_{xy}^{\tau-t''} \sim (\dtau) N_{xy}^\tau.
   \]

   Thus:
   \[ cov_\tau(x,y) = {N_{xy}^\tau \ over \dtau \int dt}
      - {N_x N_y \over (\int dt)^2},
   \]
   which is the value returned by cov(c0,c1,bin).
*/

double SpikeXCorrel::subtractor(int c0, int c1) const {
  return double(count(c0))*count(c1)*dt/totaltime();
}

double SpikeXCorrel::normalizer(int c0, int c1) const {
  return subtractor(c0, c1);
}
