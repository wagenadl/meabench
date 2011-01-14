// LinePanel.C

#include "LinePanel.H"
#include <qpainter.h>
#include "Storage.H"
#include "RD_Stim.H"
#include "ControlPanel.H"
#include "SimpleRaster.H"
#include <common/ChannelNrs.H>

LinePanel::LinePanel(QWidget *parent, Storage *src, ControlPanel *ctrlp0,
		     bool h_not_v0):
  QWidget(parent), ctrlp(ctrlp0), h_not_v(h_not_v0) {
  for (int hw=0; hw<NCHANS; hw++) {
    data[hw] = new RD_Stim(this,src,ctrlp->controlinfo(),MAXSPIKES_PANEL);
    data[hw]->setchannel(hw);
    connect(ctrlp,SIGNAL(newsource()),data[hw],SLOT(reset()));
    connect(data[hw],SIGNAL(first(const RD_Stim *)),
	    SLOT(enable(const RD_Stim *)));
    raster[hw]=0;
    pos[hw]=0;
  }
  dbx("LinePanel OK");
  setPaletteBackgroundColor(QColor("black"));
  setMinimumSize(100,100);
}

void LinePanel::enable(RD_Stim const *rds) {
  sdbx("LinePAnel::enable %p",rds);
  for (int hw=0; hw<NCHANS; hw++) 
    if (rds==data[hw]) {
      if (raster[hw]==0) {
	raster[hw] = new SimpleRaster(this,data[hw],ctrlp->controlinfo());
	connect(ctrlp,SIGNAL(updateinfo()),raster[hw],SLOT(update()));
	int c,r; QString s; hw2cr(hw,c,r); s.sprintf("%i%i",c,r);
	raster[hw]->setLabel(s);
	raster[hw]->show();
	sdbx("LinePanel::enable %i",hw);
      }
    }
  resizeEvent(0);
  update();
}

void LinePanel::resizeEvent(QResizeEvent *) {
  sdbx("LinePanel::resizeEvent");
  int wid = size().width();
  int hei = size().height();
  int nenabled=0;
  for (int hw=0; hw<NCHANS; hw++) 
    if (raster[hw])
      nenabled++;
  if (nenabled==0)
    nenabled=1; // no division by zero, please

  int ord=0;
  for (int c=0; c<8; c++) {
    for (int r=0; r<8; r++) {
      int hw = cr2hw(c,r);
      if (hw>=0 && hw<NCHANS && raster[hw]) {
	if (h_not_v) {
	  pos[hw] = wid*ord/nenabled;
	  raster[hw]->setGeometry(pos[hw]+1,1,
				  wid*(ord+1)/nenabled - pos[hw]-2,
				  hei-2);
	} else {
	  pos[hw] = hei*ord/nenabled;
	  raster[hw]->setGeometry(1,pos[hw]+1,
				  wid-2,
				  hei*(ord+1)/nenabled - pos[hw]-2);
	}
	ord++;
      }
    }
  }
}
