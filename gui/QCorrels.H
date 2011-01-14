/* gui/QCorrels.H: part of meabench, an MEA recording and analysis tool
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

// QCorrels.H

#ifndef QCORRELS_H

#define QCORRELS_H

#include <gui/QMultiGraph.H>
#include <gui/QHisto.H>
#include <base/CorrelData.H>

#include <sys/time.h>
#include <unistd.h>

class QCorrels: public QMultiGraph {
  Q_OBJECT
public:
  static const int DEFLTDECAY_MS = 0; // 0 = no decay
public:
  QCorrels(QWidget *parent=0, char const *name=0, WFlags f=0); //:c
  ~QCorrels();
  int bins() const { return data[0].bins(); }
  int xRange() const { return data[0].range(); }
  float yRange() const { return (QHisto*)(graphptrs[0])->maxValue(); }
  void autoDecay();
public slots:
  void setBaseChannel(int ch);
  void setAutoCorrel();
  void setSpeedy(bool speedy);
  void reset();
  void decay(float factor);
  void setBins(int bins);
  void setXRange(int time);
  void setYRange(float maxval);
  void setXSpacing(int bins);
  void setYSpacing(float val);
  void addSpikes(SpikeSFCli const &spikesf,
		 timeref_t beginidx, timeref_t endidx);
  void setAutoRedraw(int time_ms);
  void setDecay(int tau_ms);
  virtual void timerEvent(QTimerEvent *);
  virtual void polish();
signals:
  void channelChosen(int);
protected slots:
  void chosenChannel(QString); 
private:
  int basechannel; // -1 for autocorr
  AutoCorrelVector *acv;
  CrossCorrelVector *xcv;
  CorrelMatrix *xcm;
  struct timeval lastdecay;
  int tau_ms;
  int timer_id;
  int nbins;
  int maxtime;
};

#endif
