// RD_Rec.H

#ifndef RD_REC_H

#define RD_REC_H

#include <RasterData.H>

class RD_Rec: public RasterData {
  Q_OBJECT;
public:
  RD_Rec(QObject *parent, class Storage const *src,
	 class ControlInfo const *info, unsigned int capacity):
    RasterData(parent,src,info,capacity) { }
public slots:
  virtual void update();
};

#endif
