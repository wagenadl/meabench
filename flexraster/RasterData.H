// RasterData.H

#ifndef RASTERDATA_H

#define RASTERDATA_H

#include <qobject.h>

struct RasterPoint {
  short x;
  unsigned short y;
  RasterPoint(short x_=0, unsigned short y_=0): x(x_), y(y_) { }
};

class RasterData: public QObject {
  Q_OBJECT;
public:
  RasterData(QObject *parent, class Storage const *src,
	     class ControlInfo const *info,
	     unsigned int capacity);
  virtual ~RasterData();
  RasterPoint const &operator[](unsigned int n) const { return points[n]; }
  RasterPoint &operator[](unsigned int n) { return points[n]; }
  unsigned int npts() const { return npts_; }
  unsigned short maxy() const { return maxy_; }
  void dump() const;
  void setchannel(char c=-1) { channel=c; } // Only for RD_Stim/Rec
public slots:
  virtual void update();
  void reset();
signals:
  void updated();
protected:
  void extend();
  void reset_();
protected:
  class Storage const *src;
  class ControlInfo const *info;
  unsigned int capacity;
  unsigned int lastspk; // src[lastspk] hasn't been handled yet.
  int lasttri; // lasttri corresponds to maxy
  RasterPoint *points;
  unsigned int npts_;
  unsigned short maxy_;
  bool use;
  char channel;
};

#endif
