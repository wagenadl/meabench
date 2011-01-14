/* raster/QAutoRaster.C: part of meabench, an MEA recording and analysis tool
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

// QAutoRaster.C

/* Changelog
 * 1/23/01 [DW] Changed to new relative timestamping scheme.
 */

#include <stdio.h>
#include <qsocketnotifier.h>
#include <qtimer.h>

#include <common/CommonPath.H>
#include "QAutoRaster.H"

QAutoRaster::QAutoRaster(QWidget *parent, char const *name, WFlags f):
  MultiRasterBase(parent,name,f) {
  nclusters=0;
  latesti=-1;
  MultiRasterBase::qrs.samsperspike = 1;
  tolerance = 80; // default value
}

QAutoRaster::~QAutoRaster() {
}

void QAutoRaster::polish() {
  MultiRasterBase::polish();
  layout = new Layout(this);
}

void QAutoRaster::refresh(QRDispInfo const *q) {
  if (q) 
    for (int i=0; i<nclusters; i++)
      qrs[i].shallowcopy(*q);
  for (int i=0; i<nclusters; i++)
    raster[i]->refresh(&qrs[i]);
}

void QAutoRaster::mayscroll(int i) {
  if (nclusters)
    if (nexttrial[i] > qrs[i].firsttrial +
	raster[i]->contentsRect().height()/qrs[i].pixpertrial)
      qrs[i].firsttrial ++;
}

void QAutoRaster::restart() {
  MultiRasterBase::restart();
  while (nclusters)
    delete raster[--nclusters];
  latesti=-1;
}

void QAutoRaster::pollresponse() {
  if (frozen)
    return;
  if (spikesf) {
    timeref_t next = spikesf->aux()->lastcontextified;
    if (ssf_latest >= next)
      return; // [+] This makes [*] safe
    while (ssf_latest < next) {
      Spikeinfo const &si((*spikesf)[ssf_latest++]);
      if (trigch==NCHANS+3 && si.time >= auto_trig_time) {
	timeref_t time = si.time;
	MultiRasterBase::nexttrial++;
	if (latesti>=0 && notfresh)
	  onerefresh();
	latesti = find(2000); // UGLY HACK! THIS IS 0V on A1
	if (latesti>=0) {
	  notfresh=true;
	  qrs[latesti].trials.add(TrialInfo(nexttrial[latesti]++,auto_trig_time,0));
	  latest_trig_time = auto_trig_time;
	  mayscroll(latesti);
	}
	if (time - auto_trig_time > FREQKHZ*10*1000)
	  auto_trig_time=time;
	else if (auto_trig_time<time) 
	  auto_trig_time +=
	    MultiRasterBase::qrs.presams + MultiRasterBase::qrs.postsams;
      } else if (si.channel==trigch) {
	MultiRasterBase::nexttrial++;
	if (latesti>=0 && notfresh)
	  onerefresh();
	latesti = find(si);
	if (latesti>=0) {
	  notfresh=true;
	  qrs[latesti].trials.add(TrialInfo(nexttrial[latesti]++,si.time,0));
	  latest_trig_time = si.time;
	  mayscroll(latesti);
	}
      } else if (si.channel<NCHANS && latesti>=0) {
	raster[latesti]->addspike(si);
      }
    }
    if (latest_spike_time < latest_trig_time + MultiRasterBase::qrs.postsams) {
      // onerefresh();
      reportTriggers(Sprintf("%i",MultiRasterBase::nexttrial).c_str());
    }
    latest_spike_time = (*spikesf)[ssf_latest-1].time; // [*] safe because of [+].
    reportTime(Sprintf("%.3f s",latest_spike_time/(1000.*FREQKHZ)).c_str());
  }
}

void QAutoRaster::onerefresh() {
  if (latesti>=0)
    raster[latesti]->refresh();
  notfresh=false;
}
  

QRaster *QAutoRaster::inject(int j, float where) {
  sdbx("inject j=%i n=%i where=%g this=%p",j,nclusters,where,this);
  cluster_mean[j]=where;
  cluster_weight[j]=1;
  int idx=0;
  for (int i=0; i<nclusters; i++)
    if (cluster_mean[i]>where)
      idx++;
  sdbx("  idx=%i",idx);
  raster[j] = new QRaster(this);
  raster[j]->setMulti(true);
  sdbx("  raster[j]=%p",raster[j]);
  qrs[j].shallowcopy(MultiRasterBase::qrs);
  sdbx("  shallow ok");
  raster[j]->setSource(&qrs[j]);
  layout->insertWidget(idx,raster[j]);
  cluster_label[j] = int(where+.5);
  string s=Sprintf("%i",mean2cr(where)); // ,cluster_label[j]);
  string t = "Cluster near "; t+=s;
  dbx(t);
  raster[j]->setNameAndId(t.c_str(),s.c_str());
  qrs[j].firsttrial=0;
  nexttrial[j]=0;
  dbx("setname ok");
  raster[j]->show();
  dbx("show ok");
  return raster[j];
}
