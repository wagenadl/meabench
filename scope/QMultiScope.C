/* scope/QMultiScope.C: part of meabench, an MEA recording and analysis tool
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

// QMultiScope.C

/* Changelog
 * 1/23/01 [DW] Changed to new relative timestamping scheme.
 */

#include <stdio.h>

#include "QMultiScope.H"

QMultiScope::QMultiScope(QWidget *parent, char const *name, WFlags f):
  QMultiGraph(parent, name, f) {
}

QMultiScope::~QMultiScope() {
}

QSSource QMultiScope::source() const {
  QSSource qss1=qss;
  qss1.length /= FREQKHZ;
  qss1.pretrig /= FREQKHZ;
  return qss1;
}

void QMultiScope::setSource(QSSource const &qss0) {
  qss = qss0;
  qss.length  *= FREQKHZ; // convert from ms to samples
  qss.pretrig *= FREQKHZ; // convert from ms to samples
  for (int hw=0; hw<TOTALCHANS; hw++) {
    qss.channel = hw;
    QScope *s = dynamic_cast<QScope *>(graphptrs[hw]);
    if (s)
      s->setSource(qss);
  }
  dumpNext();
}

void QMultiScope::setSpikeSource(SpikeSFCli *spikesf0) {
  spikesf = spikesf0;
  dumpNext();
}

void QMultiScope::setLength(int length) {
  dbx("Set length");
  qss.length = length*FREQKHZ;
  forall(&QScope::setLength,qss.length);
  update();
  dumpNext();
}

void QMultiScope::setPreTrig(int pretrig) {
  dbx("Set pre trig");
  qss.pretrig = pretrig*FREQKHZ;
  forall(&QScope::setPreTrig,qss.pretrig);
  update();
  dumpNext();
}

void QMultiScope::setExtent(int ext) {
  forall(&QScope::setExtent,ext);
  update();
  dumpNext();
}

void QMultiScope::setGuideSpacing(int spc) {
  forall(&QScope::setGuideSpacing,spc);
  update();
  dumpNext();
}

void QMultiScope::setSpeedy(int s) {
  sdbx("Set speedy %i",s);
  forall(&QScope::setSpeedy,s);
  update();
  dumpNext();
}

void QMultiScope::setCenter() {
  dbx("Set center");
  forall(&QScope::setCenter);
  update();
  dumpNext();
}

void QMultiScope::refresh(timeref_t t) {
  qss.endtime = t;
  forall(&QScope::refresh,t);
  addSpikes();
  dumpme();
}

void QMultiScope::addSpikes() {
  if (!spikesf || !spikesf->running())
    return;
  timeref_t spkidx = spikesf->latest();
  unsigned int spkbufsiz = (unsigned int)(.75*spikesf->bufsize());
  timeref_t spkstart = (spkidx>spkbufsiz) ? spkidx-spkbufsiz : 0;
  if (spkstart < spikesf->first())
    spkstart = spikesf->first();
  timeref_t winend = qss.endtime - qss.sf->first();
  timeref_t winstart = winend - qss.length;
  sdbx("addSpikes end=%Li start=%Li",winend,winstart);
  int nspi = 0;
  while (spkidx>spkstart) {
    Spikeinfo const &si = (*spikesf)[--spkidx];
//dbx//	   if (si.time < winstart && si.channel==47) {
//dbx//	     sdbx("Spike %i too old: %i < %i",int(spkidx),int(si.time),int(winstart));      break;
//dbx//	   }
    if (si.time < winend) {
      if (si.time < winstart) {
	if (si.time < winstart - (100*FREQKHZ))
	  break; // way before window? Let's stop right here: valuable recent spikes won't be found
	else
	  continue; // don't try to plot anything that's too old.
      }
      QScope *s = dynamic_cast<QScope *>(graphptrs[si.channel]);
      if (s) {
	s->addSpike(si);
	nspi++;
//dbx//	       if (si.channel==47)
//dbx//		 sdbx("Spike %i shown: %i",int(spkidx),int(si.time));
      } else {
//dbx//	       if (si.channel==47)
//dbx//		 sdbx("Spike %i not shown: ch=%i",int(spkidx),int(si.channel));
      }
    } else {
//dbx//	     if (si.channel==47)
//dbx//	       sdbx("Spike %i too recent: %i >= %i",int(spkidx),int(si.time),int(winend));
    }
  }
  if (nspi)
    sdbx("QMS: addspikes: %i",nspi);
}

QGraph *QMultiScope::constructGraph(int ch) {
  QScope *qs = new QScope(this,0);
  QSSource qss2(qss); qss2.channel = ch; qs->setSource(qss2);
  return qs;
}

void QMultiScope::restart() {
  qss.endtime = first();
  dumpNext();
};
