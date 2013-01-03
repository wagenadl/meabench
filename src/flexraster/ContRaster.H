// ContRaster.H

#ifndef CONTRASTER_H

#define CONTRASTER_H

#include "SimpleRaster.H"

class ContRaster: public SimpleRaster {
  Q_OBJECT;
public:
  ContRaster(QWidget *parent, class Storage const *src,
	     class ControlInfo const *info);
  virtual ~ContRaster();
  void limitUpdate(bool limit0) { limit=limit0; }
public slots:
  virtual void move(long long t0, long long dt);
  virtual void dataUpdate();
protected:
  virtual void paintEvent(QPaintEvent *);
  virtual void resizeEvent(QResizeEvent *);
private:
  class Storage const *src;
  long long t0; // window start, samples
  long long dt; // window width, samples
  long long lastupdate;
  bool limit;
};

#endif
