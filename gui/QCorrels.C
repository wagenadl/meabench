/* gui/QCorrels.C: part of meabench, an MEA recording and analysis tool
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

// QCorrels.C

#include "QCorrels.H"

QCorrels::QCorrels(QWidget *parent, char const *name, WFlags f):
  QMultiGraph(parent, name, f) {
  gettimeofday(&lastdecay,0);
  tau_ms = DEFLTDECAY_MS;
  timer_id = 0;
  acv=0; xcv=0; xcm=0;
  nbins=12; maxtime=1000;
  xcm = new CorrelMatrix(TOTALCHANS);
  xcm->setBins(nbins); xcm->setRange(maxtime);
  setAutoCorrel();

  for (vector<QGraph *>::iterator i = graphptrs.begin();
       i!=graphptrs.end(); ++i) {
    QHisto *gr = (QHisto*)(*i);
    connect(gr,SIGNAL(chosen(QString)),this,SLOT(chosenChannel(QString)));
  }
}

QCorrels::~QCorrels() {
  if (acv)
    delete acv;
  if (xcv)
    delete xcv;
  if (xcm)
    delete xcm;
}

void QCorrels::chosenChannel(QString id) {
  string s=id.ascii();
  int ch=-1;
  if (s.substr(0,1) == "D") 
    ch = 63;
  else if (s.substr(0,1) == "A")
    ch = 60 + atoi(s.substr(1,1))-1;
  else {
    int col = atoi(s.substr(0,1));
    int row = atoi(s.substr(1,1));
    if (col<1 || col>8 || row<1 || row>8)
      ch = -1; // Bad number - what to do?
    else
      ch = cr2hw(col-1,row-1);
  }
  if (ch>=0)
    emit channelChosen(ch);
}

void QCorrels::reset() {
  if (acv)
    acv->reset();
  if (xcv)
    xcv->reset();
  if (xcm)
    xcm->reset();
  update();
}

void QCorrels::decay(float factor) {
  if (acv)
    acv->decay(factor);
  if (xcv)
    xcv->decay(factor);
  if (xcm)
    xcm->decay(factor);
  update();
}

void QCorrels::setBins(int bins) {
  nbins = bins;
  if (acv) {
    acv->setBins(bins);
    for (int c=0; c<TOTALCHANS; c++) 
      (QHisto*)(graphptrs[c])->setDataPtr(acv->dataptr(c),nbins);
  }
  if (xcv) {
    xcv->setBins(bins);
    for (int c=0; c<TOTALCHANS; c++) 
      (QHisto*)(graphptrs[c])->setDataPtr(xcv->dataptr(c),nbins);
  }
  if (xcm) {
    xcm->setBins(bins);
    for (int c=0; c<TOTALCHANS; c++) 
      (QHisto*)(graphptrs[c])->
	setDataPtr(xcm->dataptr(c,basechannel<0?c:basechannel),nbins);
  }
  update();
}

void QCorrels::setXRange(int time) {
  maxtime=time;
  if (acv)
    acv->setXRange(time);
  if (xcv)
    xcv->setXRange(time);
  if (xcm)
    xcm->setXRange(time);
  update();
}

void QCorrels::setYRange(float maxval) {
  forall(&QHisto::setMaxValue,maxval);
  update();
}

void QCorrels::setXSpacing(int ivalbins) {
  forall(&QHisto::setXSpacing,ivalbins);
  update();
}

void QCorrels::setYSpacing(float val) {
  forall(&QHisto::setYSpacing,val);
  update();
}

void QCorrels::setDecay(int tau_ms0) {
  tau_ms = tau_ms0;
}

void QCorrels::setAutoRedraw(int time_ms) {
  if (timer_id)
    killTimer(timer_id);
  timer_id = 0;
  if (time_ms) 
    timer_id = startTimer(time_ms);
}

void QCorrels::timerEvent(QTimerEvent *qte) {
  autoDecay();
  update();
}

void QCorrels::autoDecay() {
  if (tau_ms) {
    struct timeval now; gettimeofday(&now,0);
    float dt_ms = (now.tv_sec-lastdecay.tv_sec)*1000. +
      (now.tv_usec-lastdecay.tv_usec)/1000.;
    lastdecay = now;
    decay(exp(-dt_ms/tau_ms));
  }
}

void QCorrel::setBaseChannel(int ch) {
  if (ch>=0 && ch<TOTALCHANS)
    basechannel = ch;
  if (acv) {
    delete acv;
    acv=0;
    xcv = new CrossCorrelVector(TOTALCHANS);
    xcv->setBins(nbins); xcv->setRange(maxtime);
    for (int c=0; c<TOTALCHANS; c++) 
      (QHisto*)(graphptrs[c])->setDataPtr(xcv->dataptr(c),xcv->bins());
  }
  if (xcv)
    xcv->setBase(basechannel);
  else if (xcm) {
    for (int c=0; c<TOTALCHANS; c++) 
      (QHisto*)(graphptrs[c])->
	setDataPtr(xcm->dataptr(c,basechannel,nbins));
  }
  update();
}

void QCorrel::setAutoCorrel() {
  basechannel=-1;
  if (xcv) {
    delete xcv;
    xcv=0;
    acv = new AutoCorrelVector(TOTALCHANS);
    acv->setBins(nbins); acv->setRange(maxtime);
    for (int c=0; c<TOTALCHANS; c++) 
      (QHisto*)(graphptrs[c])->setDataPtr(acv->dataptr(c),nbins);
  } else if (xcm) {
    for (int c=0; c<TOTALCHANS; c++) 
      (QHisto*)(graphptrs[c])->
	setDataPtr(xcm->dataptr(c,c),nbins);
  }
  update();
}

void QCorrel::addSpikes(SpikeSFCli const &spikesf,
			timeref_t beginidx, timeref_t endidx) {
  for (timeref_t t=beginidx; t<endidx; t++) {
    Spikeinfo const &si=spikesf[t];
    int ch = si.channel;
    timeref_t ti = si.time;
    if (acv)
      acv->add_xcor(ch,ti);
    if (xcv)
      xcv->add_xcor(ch,ti);
    if (xcm)
      xcm->add_xcor(ch,ti);
  }
}

void QCorrel::setSpeedy(bool speedy) {
  if (speedy) {
    // fast, so auto/cross vector
    if (xcm)
      delete xcm;
    xcm=0;
    if (basechannel<0) {
      if (xcv)
	delete xcv;
      xcv=0;
      if (!acv) {
	acv = new AutoCorrelVector(TOTALCHANS);
	acv->setBins(nbins); acv->setRange(maxtime);
	for (int c=0; c<TOTALCHANS; c++) 
	  (QHisto*)(graphptrs[c])->setDataPtr(acv->dataptr(c),nbins);
      }
    } else {
      if (acv)
	delete acv;
      acv=0;
      if (!xcv) {
	xcv = new CrossCorrelVector(TOTALCHANS);
	xcv->setBins(nbins); xcv->setRange(maxtime);
	for (int c=0; c<TOTALCHANS; c++) 
	  (QHisto*)(graphptrs[c])->setDataPtr(xcv->dataptr(c),xcv->bins());
      }
    }
  } else {
    // slow, so full matrix
    if (acv)
      delete acv;
    acv=0;
    if (xcv)
      delete xcv;
    xcv=0;
    if (!xcm) {
      xcm = new CorrelMatrix(TOTALCHANS);
      xcm->setBins(nbins); xcm->setRange(maxtime);
      for (int c=0; c<TOTALCHANS; c++) 
	(QHisto*)(graphptrs[c])->
	  setDataPtr(xcm->dataptr(c,basechannel<0?c:basechannel),nbins);
    }
  }
}
