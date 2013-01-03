// RD_Stim.H

#ifndef RD_STIM_H

#define RD_STIM_H

#include <RasterData.H>

class RD_Stim: public RasterData {
  Q_OBJECT;
public:
  RD_Stim(QObject *parent, class Storage const *src,
	  class ControlInfo const *info, unsigned int capacity):
    RasterData(parent,src,info,capacity) { inuse=false; }
public slots:
  virtual void update();
signals:
  void first(RD_Stim const *);
private:
  bool inuse;
};


#endif
