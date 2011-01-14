/* gui/QGraph.H: part of meabench, an MEA recording and analysis tool
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

// QGraph.H - a single trace graph

#ifndef QGRAPH_H

#define QGRAPH_H

#include <qframe.h>
#include <qpen.h>
#include <set>
#include <vector>
#include <string>
#include <base/Foreach.H>

class QGraph: public QFrame { //:C QGraph
  /*:D QGraph is a graph showning something for a single channel for a fixed
       time window.
     . A QGraph can own a number of satellite QGraphs showing the same
       data. Normally, double clicking on a QGraph creates a new
       satellite.
     . Satellites inherit source pointers, extent, center and guide spacing
       from the controller. They do not inherit pens. Modifying pens on
       satellites is currently not an option.
  */
  Q_OBJECT
public:
  QGraph(QWidget *parent=0, QGraph *controller=0); //:c
  /*:D This is the standard constructor for any QWidget, with the
       addition of a final argument specifying a controller. (See above.)
   *:A parent: as for QWidget
     . controller: set to non-zero if this QGraph is a satellite, and
       should report its death to some controller.
   *:N Satellites are typically generated on user request to zoom in on some
       interesting channel. Thus QGraphs with non-zero controller may want
       to display their data in a more elaborate way.
  */
  virtual ~QGraph(); //:~
  /*:D The destructor will kill any satellites. If this widget is itself
       a satellite, it will notify its controller.
  */
  void setNameAndId(QString const &name, QString const &id); //:f setNameAndId
  /*:D Sets the name and the ID for this graph. The ID is printed in the top
       left corner of the graph, and the name is used in the title bar
       of satellites.
   *:A name, id: strings to use as name resp id.
   *:R -
   */
  void setTracePen(QPen const &pen); //:f setTracePen
  /*:D setTracePen(pen) sets the pen to be used to draw the graph. See Qt
       documentation for details on pens.
   *:A pen: pen to be used.
   *:R -
   */
  void setGuidePen(QPen const &pen); //:f setGuidePen
  /*:D setGuidePen(pen) sets the pen to be used to draw guide lines.
       See Qt documentation for details on pens.
   *:A pen: pen to be used.
   *:R -
   */
  void setAuxPen(QPen const &pen); //:f setAuxPen
  /*:D setAuxPen(pen) sets the pen to be used to draw auxillary information,
       such as spikes. See Qt documentation for details on pens.
   *:A pen: pen to be used.
   *:R -
   */
  void setZeroPen(QPen const &pen); //:f setZeroPen
  /*:D setZeroPen(pen) sets the pen to be used to draw a zero line.
       See Qt documentation for details.
   *:A pen: pen to be used.
   *:R -
   */
  void setBackgroundColor(const QColor &col); //:f setBackgroundColor
  /*:D Reimplemented from QWidget to propagate to satellites. */
protected:
  virtual void resizeEvent(QResizeEvent *); //:f resizeEvent
  /*:D Recalculates the geometry variables. Reimplementations in descendent
       classes MUST call this method first.
   *:N Reimplemented from QWidget.
  */
  virtual void drawContents(QPainter *); //:f drawContents
  /*:D This method prints the unit number in the top left corner. Descendents
       are advised to call this method from their reimplementation prior to
       drawing their own contents.
   *:N Reimplemented from QWidget.
  */
  virtual void mouseDoubleClickEvent(QMouseEvent *);
  virtual void closeEvent(QCloseEvent *);
protected:
  virtual QGraph *make_satellite(QGraph *controller); //:f make_satellite
  /*:D This method creates a new satellite with the info of the object
       on which it is invoked for use as a satellite. Arg controller
       should be passed on to the constructor of the parent class.
     . To be reimplemented by descendents.
   *:A controller: as for QGraph constructor
   *:R New QGraph. Caller will take care of ultimate destruction.
   *:N The copying of information should be delegated to init_satellite().
   */
  virtual void init_satellite(QGraph *); //:f init_satellite
  /*:D This method initializes a satellite with the info of the object
       on which it is invoked for use as a satellite.
     . To be reimplemented by descendents. Before doing anything else,
       re-implementations should call the parent's implementation.
   *:A Pointer to QGraph to fill. Dynamic cast this to your own type
       before use.
   *:R -
   */
protected:
  template <class D> void forall(void (D::*foo)()) {
    foreach(satellites.begin(),satellites.end(),foo);
  }
////  template <class D, class A>
////    void forall(void (D::*foo)(A const &),A const &a0) {
////    foreach(satellites.begin(),satellites.end(),foo,a0);
////  }
  template <class D, class A> void forall(void (D::*foo)(A &),A &a0) {
    foreach(satellites.begin(),satellites.end(),foo,a0);
  }
  template <class D, class A> void forall(void (D::*foo)(A),A a0) {
    foreach(satellites.begin(),satellites.end(),foo,a0);
  }
private:
  void satellitedying(QGraph *satellite);
protected:
  // All of these READ ONLY for descendents.
  QPen zero_pen, guide_pen, trace_pen, aux_pen, black_pen; //:d xxx_pen
  /*:D Various pens for drawing parts of the widget. Name indicates recommended
       use, but different implementations may have different interpretations.
   *:N Read-only for descendents.
   */
  QGraph *controller;
  std::set<QGraph *> satellites;
  int hei, wid; //:d hei,wid
  /*:D height and width of the contents part of the widget.
   *:N Read-only for descendents.
   */
  int xoffset, yoffset; //:d xoffset, yoffset
  /*:D Offsets from top left of widget (i.e. margin width).
   *:N Read-only for descendents.
   */
protected:
  QString myname;
  QString myid;
};

#endif
