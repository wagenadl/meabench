/* scope/QMultiScope.H: part of meabench, an MEA recording and analysis tool
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

// QMultiScope.H

#ifndef QMMULTISCOPE_H

#define QMMULTISCOPE_H

#include <gui/QMultiGraph.H>
#include "QScope.H"

class QMultiScope: public QMultiGraph { //:C QMultiScope
  Q_OBJECT
  /*:D QMultiScope is a giant widget containing 8x8 QScopes. */
public:
  QMultiScope(QWidget *parent=0, char const *name=0, WFlags f=0); //:c
  ~QMultiScope(); //:~
  void setSource(QSSource const &qss); //:f setSource
  /*:D setSource(qss) sets the source to be used for subsequent
       plots. The number of channels is fixed by the constants in Common.H.
     . The channel member of QSSource is not used.
     . The "length" and "pretrig" fields are in ms, unlike for QScope.
     . Uses SloppyFifos as input, so beware to give this process enough
       CPU time, or plots may not look pretty.
   *:A qss: source description
   *:R -
   */
  void setSpikeSource(SpikeSFCli *spikesf); //:f setSpikeSource
  /*:D setSpikeSource(spikesf) sets the source from which spike infos will
       be read to add to the waveform displays.
   *:A spikesf: source of spikeinfos, or 0 for none.
   *:R -
   *:N Becomes effective at next update().
   */
public slots:
  void setExtent(int extent); //:f setExtent
  /*:D setExtent(extent) sets the extents for all traces.
   *:A extent: new extent in uV
   *:R -
   */
  void setGuideSpacing(int spc); //:f setExtent
  /*:D setGuideSpacing(spc) sets the guide line spacing for all traces.
   *:A spc: new guide line spacing in uV
   *:R -
   */
  void setCenter(); //:f setCenter
  /*:D setCenter() sets the centers for all traces, using the most recent
       input data.
   *:A -
   *:R -
   */
  void setSpeedy(int s); //:f setSpeedy
  /*:D setSpeedy(s) enables or disable fast and sloppy redraw.
   *:A s: See enum SpeedyMode
   *:R -
   */
  void setLength(int length); //:f setLength
  /*:D Change the window length for all scopes.
   *:A length: window length (in ms)
   *:R -
   */
  void setPreTrig(int pretrig); //:f setPreTrig
  /*:D Change the amount of window length to reserfve for pretrig time
       for all scopes.
   *:A pretrig: time in ms
   *:R -
   */
public:
  void refresh(timeref_t t=0); //:f refresh
  /*:D refresh() refreshes all plots using the old data pointer. refresh(t)
       specifies a new end time reference. The data is read from the
       SloppyFifo specified in the most recent setSource() call.
   *:A t (optional): new end time.
   */
  timeref_t latest() const { return qss.endtime; } //:f latest
  /*:D latest() returns the latest timeref passed to refresh, i.e. the
       timestamp of the right end of the graphs.
   *:A -
   *:R timestamp
   */
  timeref_t period() const { return qss.length; } //:f period
  /*:D period() returns the window width in sample periods
   *:A -
   *:R window width in samples
   */
  timeref_t first() const { return qss.sf ? qss.sf->first() : 0; } //:f first
  /*:D first() returns the timeref of start of raw run.
   *:A -
   *:R start of run timeref
   */
  void restart(); //:f restart
  /*:D restart() clears the graphs ready for new data
   *:A -
   *:R -
   *:N latest() will return the timeref of the start of the new run when called
       immediately after a restart().
   *:E This vsn doesn't actually clear the display.
   */
  QSSource source() const;
protected:
  virtual QGraph *constructGraph(int ch); //:f constructGraph
private:
  void addSpikes(); //:f addSpikes
  /*:D addSpikes() adds the spikes in sf to the current display for as far
       as the time lies within the currently displayed window.
   *:N Spikes are read from the spike source set in setSpikeSource.
  */
private:
  QSSource qss;
  SpikeSFCli *spikesf;
};

#endif
