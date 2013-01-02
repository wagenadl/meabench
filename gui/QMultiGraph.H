/* gui/QMultiGraph.H: part of meabench, an MEA recording and analysis tool
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

// QMultigraph.H

#ifndef QMULTIGRAPH_H

#define QMULTIGRAPH_H

#include <QFrame>
#include <QLayout>

#include <vector>

#include <gui/QGraph.H>
#include <base/Foreach.H>
#include <common/ChannelNrs.H>

class QMultiGraph: public QFrame { //:C Qmultigraph
  /*:D QMultiGraph is a giant widget containing 8x8 QGraphs.
       The corner traces are used to represent the analog channels. */
  Q_OBJECT
public:
  QMultiGraph(QWidget *parent=0); //:c
  ~QMultiGraph(); //:~
  void postConstruct(); //:f postConstruct
  /*:D This must be called just after construction to do most of the real
       work. It is implemented in this awkward way because post construction
       depends on virtual functions implemented by descendents.
   *:A -
   *:R -
  */
  void resizeEvent(QResizeEvent *qre); //:f resizeEvent
  /*:D resizeEvent() is inherited from QWidget. Here, it recalculates the
       layout of the children for 8x8 or Hex.
   *:A see QWidget
    *:R -
    */
  void setDump(char const *fn, bool frame);
public slots:
  void setLayout(QString const &lay); //:f setLayout
  /*:D setLayout changes the layout of electrodes.
       Causes a redraw of the display.
   *:A lay: must be one of "8x8", "Hex", "JN".
   *:R -
   */
  void setTracePen(QPen const &pen); //:f setTracePen
  /*:D setTracePen(pen) sets the pen to be used to draw the graphs. See Qt
       documentation for details on pens.
   *:A pen: pen to be used.
   *:R -
   */
  void setZeroPen(QPen const &pen); //:f setZeroPen
  /*:D setGuidePen(pen) sets the pen to be used to draw a zero line.
       See Qt documentation for details on pens.
   *:A pen: pen to be used.
   *:R -
   */
  void setGuidePen(QPen const &pen); //:f setGuidePen
  /*:D setGuidePen(pen) sets the pen to be used to draw guide lines.
       See Qt documentation for details on pens.
   *:A pen: pen to be used.
   *:R -
   */
  void setAuxPen(QPen const &pen); //:f setSpikePen
  /*:D setAuxPen(pen) sets the pen to be used to draw auxiliary stuff such
       as spike markers. See Qt documentation for details on pens.
   *:A pen: pen to be used.
   *:R -
   */
protected:
  virtual QGraph *constructGraph(int ch)=0; //:f constructGraph
  /*:D constructGraph(ch) construct a brand new graph for use as graph of
       channel number ch. To be reimplemented by descendents.
   *:A ch: channel number to be represented by the graph.
   *:R -
   */
  template <class D> void forall(void (D::*foo)()) {
    mforeach(graphptrs.begin(),graphptrs.end(),foo);
  }
////  template <class D, class A>
////    void forall(void (D::*foo)(A const &),A const &a0) {
////    mforeach(graphptrs.begin(),graphptrs.end(),foo,a0);
////  }
  template <class D, class A>
    void forall(void (D::*foo)(A const *),A const *a0) {
    mforeach(graphptrs.begin(),graphptrs.end(),foo,a0);
  }
  template <class D, class A> void forall(void (D::*foo)(A &),A &a0) {
    mforeach(graphptrs.begin(),graphptrs.end(),foo,a0);
  }
  template <class D, class A> void forall(void (D::*foo)(A),A a0) {
    mforeach(graphptrs.begin(),graphptrs.end(),foo,a0);
  }
  void dumpme(); // dump a frame
  void dumpNext(); // increments the "set" for dumping
private:
  void recolor_children();  
  void resize_children(int wid, int hei);
  void setNamesAndIDs();
protected:
  // READ ONLY for descendents.
  vector<QGraph *> graphptrs;
  enum Layouts {
    LAYOUT_8x8,
    LAYOUT_HEX,
    LAYOUT_JN,
  };
  Layouts layout;
  bool dumping, framedump;
  int dumpset, dumpframe;
  string dumpfn;
};

#endif
