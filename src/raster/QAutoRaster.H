/* raster/QAutoRaster.H: part of meabench, an MEA recording and analysis tool
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

#ifndef QAUTORASTER_H

#define QAUTORASTER_H

#include <gui/QGraph.H>
#include <sys/time.h>
#include "QRaster.H"
#include "MultiRasterBase.H"
#include <QVBoxLayout>

#include <math.h>

#include <QLayout>

class QAutoRaster: public MultiRasterBase { //:C QAutoRaster
  Q_OBJECT
  /*:D QAutoRaster is a giant widget containing 8x8 QRasters. */
public:
  static const int MAXCLUSTERS = 80;
  typedef QVBoxLayout Layout;
public:
  QAutoRaster(QWidget *parent=0);
  ~QAutoRaster(); //:~
public:
  void refresh(QRDispInfo const *q=0); //:f refresh
  /*:D refresh() refreshes all plots reading recent data from the
       spike source.
   */
  void restart(); //:f restart
  /*:D restart() clears the graphs ready for new data
   *:E This vsn doesn't actually clear the display.
   */
public slots:
  virtual void setTolerance(int tol) { tolerance=tol; sdbx("setTolerance %i",tol); }
protected:
  void pollresponse();
  void mayscroll(int i);
  QGraph *constructGraph(int) { return 0; }
private:
  Layout *layout;
  QRaster *raster[MAXCLUSTERS];
  float cluster_mean[MAXCLUSTERS];
  int cluster_label[MAXCLUSTERS];
  int cluster_weight[MAXCLUSTERS];
  QRDispInfo qrs[MAXCLUSTERS];
  int nexttrial[MAXCLUSTERS];
  int nclusters;
  int latesti;
  bool notfresh;
  int tolerance;
private:
  void onerefresh();
  QRaster *inject(int j,float where);
  inline int find(Spikeinfo const &si) {
    float val = (si.context[18]+si.context[19]+si.context[20]+si.context[21])/4;
    return find(val);
  }
  inline int find(float val) {
    for (int i=0; i<nclusters; i++) {
      float v = cluster_mean[i];
      if (val>v-tolerance && val<v+tolerance) {
	int w = cluster_weight[i];
	cluster_mean[i] = (w*v+val) / (w+1);
	cluster_weight[i] = w+1;
	float dl = cluster_mean[i] - cluster_label[i];
	if (dl>5 || dl<-5) {
	  cluster_label[i] = int(cluster_mean[i]+.5);
	  string s=Sprintf("%i",mean2cr(cluster_mean[i])); //,cluster_label[i]);
	  string t = "Cluster near "; t+=s;
	  raster[i]->setNameAndId(t.c_str(),s.c_str());
	}
	sdbx("Old cluster: %i [tol=%i]",i,tolerance);
	return i;
      }
    }
    if (nclusters<MAXCLUSTERS) {
      inject(nclusters, val);
      sdbx("New cluster: %i",nclusters);
      return nclusters++;
    }
//    int ibest=0;
//    float dbest = 1e9;
//    for (int i=0; i<nclusters; i++) {
//      float v = cluster_mean[i];
//      float d = (val-v)*(val-v);
//      if (d<dbest) {
//	dbest=d;
//	ibest=i;
//      }
//    }
//    return ibest;
    return -1;
  }
  inline int mean2hw(float mean) {
    return int(floor(-0.03352894*mean + 64.56456086 + .5));
  }
  inline int mean2cr(float mean) {
    int c,r;
    int hw = mean2hw(mean);
    sdbx("mean2cr: %g -> %i",mean,hw);
    if (hw<0 || hw>59)
      return 0;
    hw2cr(hw,c,r);
    return c*10+r+11;
  }
};

#endif
