// LinePanel.H

#ifndef LINEPANEL_H

#define LINEPANEL_H

#include <qwidget.h>
#include <common/Config.H>
#include "RD_Stim.H"

class LinePanel: public QWidget {
  Q_OBJECT;
public:
  LinePanel(class QWidget *parent,
	    class Storage *src, class ControlPanel *ctrlp,
	    bool h_not_v);
public slots:
  void resizeEvent(QResizeEvent *);
  void closeEvent(QCloseEvent *) { delete this; }
  void enable(RD_Stim const *);
private:
  class ControlPanel const *ctrlp;
  class SimpleRaster *raster[NCHANS];
  class RD_Stim *data[NCHANS];
  bool h_not_v;
  int pos[NCHANS];
};


#endif
