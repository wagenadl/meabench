// GridPanel.H

#ifndef GRIDPANEL_H

#define GRIDPANEL_H

#include <qwidget.h>

class GridPanel: public QWidget {
  Q_OBJECT;
public:
  GridPanel(class QWidget *parent,
	    class Storage *src, class ControlPanel *ctrlp,
	    bool rec_not_stim);
public slots:
  void resizeEvent(QResizeEvent *qre);
  void closeEvent(QCloseEvent *) { delete this; }
private:
  class SimpleRaster *raster[8][8];
  class RasterData *data[8][8];
};

#endif
