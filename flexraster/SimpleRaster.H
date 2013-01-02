// SimpleRaster.H

#ifndef SIMPLERASTER_H

#define SIMPLERASTER_H

#include <QWidget>
#include <QPolygon>

const int MAXPTS = 1000000;

class SimpleRaster: public QWidget {
  Q_OBJECT;
public:
  SimpleRaster(QWidget *parent, class RasterData const *src,
	       class ControlInfo const *info);
  virtual ~SimpleRaster();
  void setLabel(QString const &s);
protected:
  virtual void paintEvent(QPaintEvent *);
protected:
  class RasterData const *src;
  class ControlInfo const *info;
  static QPolygon ptar;
  QString label;
};

#endif
