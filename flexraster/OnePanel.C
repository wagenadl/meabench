// OnePanel.C

#include "OnePanel.H"

#include "Storage.H"
#include "ControlPanel.H"
#include "Storage.H"
#include "RD_Stim.H"
#include "SimpleRaster.H"

OnePanel::OnePanel(QWidget *parent, Storage *src, ControlPanel *ctrlp):
  QWidget(parent) {
  data = new RD_Stim(this,src,ctrlp->controlinfo(),MAXSPIKES_SPONT);
  raster = new SimpleRaster(this,data,ctrlp->controlinfo());
  connect(ctrlp,SIGNAL(updateinfo()),raster,SLOT(update()));
  connect(ctrlp,SIGNAL(newsource()),data,SLOT(reset()));
  setMinimumSize(100,100);
  raster->show();
}

void OnePanel::resizeEvent(QResizeEvent *qre) {
  sdbx("OnePanel::resizeEvent");
  raster->setGeometry(0,0,qre->size().width(),qre->size().height());
}
