/* scope/QScope.H: part of meabench, an MEA recording and analysis tool
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

// QScope.H - a single trace graph

#ifndef QSCOPE_H

#define QSCOPE_H

#include <gui/QGraph.H>

#include <common/Types.H>
#include <common/Config.H>
#include <common/ChannelNrs.H>
#include <rawsrv/Defs.H>
#include <spikesrv/Defs.H>

class QSSource { //:C QSSource
  /*:D Helper class to pass source information around. */
public:
  QSSource(RawSFCli const *sf0=0, timeref_t t0=0, //:+
	   unsigned int ch0=0, timeref_t len0=0, timeref_t pret0=0): //:c
    /*:A sf: pointer to source SloppyFifo
       . t: end time reference of frame to be plotted
       . channel: channel to be selected from the sf.
       . length: length of window in samples. E.g. length=25000 for a
                 1s window.
       . pretrig: length of window part before trigger, in samples.
    */
    sf(sf0), endtime(t0), channel(ch0), length(len0), pretrig(pret0) {}
public:
  RawSFCli const *sf;
  timeref_t endtime;
  unsigned int channel;
  timeref_t length;
  timeref_t pretrig;
};

class QScope: public QGraph { //:C QScope
  /*:D QScope is a graph showning a trace for a single channel for a fixed
       time window.
     . This version obtains all data from SFSleepers, and is therefore
       rather specialized for meabench.
     . See QGraph for comments on derived methods.
  */
  Q_OBJECT
public:
  enum SpeedyMode { TrueBlue=0, MinMax=1, AvgOnly=2 };
public:
  QScope(QWidget *parent=0, QGraph *controller=0); //:c
  ~QScope(); //:~
  virtual void mouseDoubleClickEvent(QMouseEvent *);
  void setSource(QSSource const &qss); //:f setSource
  /*:D setSource(qss) defines the data source for subsequent
       plotting. If double buffering is required, the refresh() function
       described below allows to specify source pointers for each frame.
   *:A qss: source description (see class QSSource docs)
   *:R -
   *:S -
   *:E -
   *:N This function should be called before the first redraw, otherwise
       empty plots will result.
     . Set sf=0 to force empty plots.
  */
  void setCenter(raw_t center); //:f setCenter
  /*:D setCenter(center) explicitly sets the center line to a given value
       in input space.
   *:A center: value of center line, in input units.
   *:R -
   */
  void setLength(timeref_t length); //:f setLength
  /*:D Change the window length.
   *:A length: window length (in samples)
   *:R -
   */
  void setPreTrig(timeref_t pretrig); //:f setPreTrig
  /*:D Change the pretrigger time
   *:A pretrig: pre trigger time (in samples)
   *:R -
   */
  void setCenter(); //:f setCenter
  /*:D setCenter() finds the center line by averaging the previous plot.
   *:A -
   *:R -
   */
  void setExtent(int extent); //:f setExtent
  /*:D setExtent(extent) explicitly sets the vertical extent of the graph.
       The number specified corresponds to half the height of the graph.
   *:A extent: the new extent, in uV
   *:R -
   *:N Note that there is not setExtent() function that sets the extent
       automatically.
  */
  void setGuideSpacing(int spacing); //:f setGuideSpacing
  /*:D setGuideSpacing(spacing) sets the distance between eye guide lines
       on the graph. One guide line is always placed at the center, and others
       are on regular intervals. Set spacing>extent to have only a central
       guideline. To have no guide lines at all, set spacing to zero.
   *:A spacing: distance between guide lines, in uV
   *:R -
  */
  void refresh(timeref_t t=0); //:f refresh
  /*:D refresh() refreshes the trace, using data at the most recently used
       source. refresh(t) specifies a new end time for this refresh. The sf,
       channel and stride are remembered from the most recent call of
       setSource().
   *:A t (optional): new end time reference
   *:R -
   *:N t=0 does not force empty plots, unlike for setSource().
   */
  void addSpike(Spikeinfo const &si); //:f addSpike
  /*:D addSpike(si) adds a dot to the graph at the position of the spike si.
   *:A si: spike info describing the spike
   *:R -
   */
  void setSpeedy(int /* enum SpeedyMode */ s); //:f setSpeedy
  /*:D setSpeedy(s) enables or disable fast and sloppy redraw.
   *:A s: See enum SpeedyMode
   *:R -
   *:N This is not propagated to satellites - satellites are always precise.
   */
protected:
  virtual void resizeEvent(QResizeEvent *);
  virtual void drawContents(QPainter *);
  virtual QGraph *make_satellite(QGraph *controller); //:f make_satellite
  virtual void init_satellite(QGraph *); //:f init_satellite
private:
  int value2y(raw_t v) { return coffset - (v-center)*halfhei/extent; }
  int time2x(timeref_t t) {
    return xoffset + (t - (qss.endtime-qss.length))*wid/qss.length;
  }
  void dumpme();
  float uvpd();
private:
  QSSource qss;
  //  SFCli<Sample,RawAux> const *sf;
  //  timeref_t endtime;
  //  timeref_t length;
  //  unsigned int channel;
  raw_t center;
  raw_t extent; // in input units, not uV
  raw_t guide_spacing; // in input units, not uV
  int halfhei, coffset;
  QPointArray *qpa;
  vector<Spikeinfo> spikes;
  unsigned int nspikes;
  SpeedyMode speedy;
};

#endif
