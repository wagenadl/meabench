// SimpleRaster.H

#ifndef SIMPLERASTER_H

#define SIMPLERASTER_H

#include <qwidget.h>

const int MAXPTS = 1000000;

class SimpleRaster: public QWidget {
  Q_OBJECT;
public:
  SimpleRaster(QWidget *parent, class RasterData const *src, class ControlInfo const *info);
  virtual ~SimpleRaster();
  void setLabel(QString const &s);
protected:
  virtual void paintEvent(QPaintEvent *);
protected:
  class RasterData const *src;
  class ControlInfo const *info;
  static QPointArray ptar;
  QString label;
};

#endif
