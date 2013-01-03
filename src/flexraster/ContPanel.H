// ContPanel.H

#ifndef CONTPANEL_H

#define CONTPANEL_H

#include <qwidget.h>

class ContPanel: public QWidget {
  Q_OBJECT;
public:
  ContPanel(class QWidget *parent,class Storage const *src,
	    class ControlPanel const *ctrlp);
public slots:
  void resizeEvent(QResizeEvent *qre);
  void closeEvent(QCloseEvent *) { delete this; }
  void updateData();
  void polish();
private slots:
  void rezoom(int);
private:
  class ContRaster *raster;
  class Scrollbar *scrollbar;
  class QSlider *zoombar;
  class QLabel *zoominfo;
  class Storage const *src;
};

#endif
