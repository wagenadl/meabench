// OnePanel.H

#ifndef ONEPANEL_H

#define ONEPANEL_H

#include <QWidget>

class OnePanel: public QWidget {
  Q_OBJECT;
public:
  OnePanel(class QWidget *parent,class Storage *src, class ControlPanel *ctrlp);
public slots:
  void resizeEvent(QResizeEvent *qre);
  void closeEvent(QCloseEvent *) { delete this; }
private:
  class SimpleRaster *raster;
  class RasterData *data;
};

#endif
