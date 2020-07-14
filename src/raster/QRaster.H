/* raster/QRaster.H: part of meabench, an MEA recording and analysis tool
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

// QRaster.H - a single trace graph

#ifndef QRASTER_H

#define QRASTER_H

#include <gui/QGraph.H>

#include <common/Types.H>
#include <common/Config.H>
#include <common/ChannelNrs.H>
#include <rawsrv/Defs.H>
#include <spikesrv/Defs.H>
#include "SpikeBuffer.H"
#include "QRDispInfo.H"
//Added by qt3to4:
#include <QResizeEvent>
#include <QMouseEvent>

class QRaster: public QGraph { //:C QRaster
  /*:D QRaster is a graph showning a raster for a single channel for a fixed
       time window.
     . See QGraph for comments on derived methods.
  */
  Q_OBJECT
  static const int QRBUFLEN=15; // 32K spikes/channel -> 30 MB
public:
  QRaster(QWidget *parent=0, QGraph *controller=0); //:c
  virtual ~QRaster(); //:~
  virtual void mouseDoubleClickEvent(QMouseEvent *);
  void setSource(QRDispInfo const *qrs); //:f setSource
  /*:D setSource(qrs) defines the data source for subsequent
       plotting. If double buffering is required, the refresh() function
       described below allows to specify source pointers for each frame.
   *:A qrs: source description
   *:R -
   *:S -
   *:E -
   *:N This function should be called before the first redraw, otherwise
       empty plots will result.
     . qrs must stay available until a new source is specified.
     . Set qrs=0 to force empty plots.
  */
  void refresh(QRDispInfo const *qrs=0); //:f refresh
  /*:D refresh() refreshes the graph, using data at the most recently used
       source. refresh(t) specifies a new start trigger for this refresh.
   *:A qrs (optional): define new parameters
   *:R -
   */
  inline void addspike(Spikeinfo const &si) {
    dots->add(si);
  }
  void reset() {
    dots->reset();
    re_source();
  }
  void setMulti(bool m) {
    multi=m;
  }
protected:
  virtual void resizeEvent(QResizeEvent *);
  virtual void drawContents(QPainter *);
  virtual QGraph *make_satellite(QGraph *controller); //:f make_satellite
  virtual void init_satellite(QGraph *); //:f init_satellite
  friend class QMultiRaster;
private:
  inline int trig2y(int tri) const {
    return yoffset + hei-1-(tri+1-qrs->firsttrial)*pixpertrial;
  } // +1 prevents annoying division by zero
  inline int dt2x(int tms) const {
    return int(xoffset + (tms+qrs->presams)*xscl);
  } // +1 prevents annoying division by zero
  void dumpme();
  void re_source();
  void makegreen(QPainter *qp, int tri);
  void fill(int hw, SpikeBuffer const *dots);
private:
  QRDispInfo const *qrs;
  SpikeBuffer *dots;
  bool own;
  int pixperspike, pixpertrial;
  float xscl;
  bool multi;
};

#endif
