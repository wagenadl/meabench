// GridPanel.C

#include "GridPanel.H"

#include "Storage.H"
#include "ControlPanel.H"
#include "Storage.H"
#include "RD_Rec.H"
#include "RD_Stim.H"
#include "SimpleRaster.H"
#include <common/ChannelNrs.H>

GridPanel::GridPanel(QWidget *parent, Storage *src, ControlPanel *ctrlp,
		     bool rec_not_stim):
  QWidget(parent) {
  for (int c=0; c<8; c++) {
    for (int r=0; r<8; r++) {
      int hw = cr2hw(c,r);
      if (hw<NCHANS) {
	if (rec_not_stim)
	  data[c][r] = new RD_Rec(this,src,ctrlp->controlinfo(),MAXSPIKES_8x8);
	else
	  data[c][r] = new RD_Stim(this,src,ctrlp->controlinfo(),MAXSPIKES_8x8);
	data[c][r]->setchannel(hw);
	raster[c][r] = new SimpleRaster(this,data[c][r],ctrlp->controlinfo());
	connect(ctrlp,SIGNAL(updateinfo()),raster[c][r],SLOT(update()));
	connect(ctrlp,SIGNAL(newsource()),data[c][r],SLOT(reset()));
	QString s; s.sprintf("%i%i",c+1,r+1);
	raster[c][r]->setLabel(s);
      } else {
	raster[c][r]=0;
	data[c][r]=0;
      }
    }
  }
  setPaletteBackgroundColor(QColor("black"));
  setMinimumSize(100,100);
  for (int c=0; c<8; c++) 
    for (int r=0; r<8; r++) 
      if (raster[c][r])
	raster[c][r]->show();
}

void GridPanel::resizeEvent(QResizeEvent *qre) {
  sdbx("GridPanel::resizeEvent");
  int wid = qre->size().width();
  int hei = qre->size().height();
  for (int c=0; c<8; c++) 
    for (int r=0; r<8; r++) 
      if (raster[c][r]) 
	raster[c][r]->setGeometry(wid*c/8+1,
				  hei*r/8+1,
				  wid*(c+1)/8-wid*c/8-2,
				  hei*(r+1)/8-hei*r/8-2);
}
