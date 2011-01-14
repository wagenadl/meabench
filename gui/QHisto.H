/* gui/QHisto.H: part of meabench, an MEA recording and analysis tool
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

// QHisto.H

#ifndef QHISTO_H

#define QHISTO_H

#include <gui/QGraph.H>

class QHisto: public QGraph {
public:
  QHisto(QWidget *parent=0, QGraph *controller=0);
  virtual ~QHisto();
  void setDataPtr(float const *data, int n);
  float maxValue() const { return maxyvalue; }
  void setMaxValue(float value);
  void setYSpacing(float value);
  void setXSpacing(int ivalbins);
public:
  virtual void resizeEvent(QResizeEvent *); //:f resizeEvent
  /*:D Recalculates the geometry variables. Reimplementations in descendent
       classes MUST call this method first.
   *:N Reimplemented from QWidget.
  */
  virtual void drawContents(QPainter *); //:f drawContents
g  /*:D This method prints the unit number in the top left corner. Descendents
       are advised to call this method from their reimplementation prior to
       drawing their own contents.
   *:N Reimplemented from QWidget.
  */
  virtual void mouseDoubleClickEvent(QMouseEvent *);
signals:
  void chosen(QString);
private:
  int bin2x(int n) { return x0_ + bin2x_*n; } // count [0,...,(2n-1)]
  int val2y(float val) { return y0_ - val2y_*val; }
  void rescaleX();
  void rescaleY();
private:
  int nbins;
  int horisubdivs; // vertical lines every x bins
  float maxyvalue;
  float vertsubdivs;
  float val2y_;
  float bin2x_;
  int x0_;
  int y0_;
  float const *data;
};

#endif
