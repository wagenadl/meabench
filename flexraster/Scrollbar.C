// Scrollbar.C

#include "Scrollbar.H"
#include <base/dbx.H>
#include <base/Sprintf.H>

const int GRAIN = 100; // 4 ms

Scrollbar::Scrollbar(QWidget *parent,
		     long long t0_, long long t1_, long long dt_):
  QScrollBar(parent) {
  QScrollBar::setRange(0, 1000);
  setSingleStep(1);
  setPageStep(10);
  setValue(0);
  setOrientation(Qt::Horizontal);
  t0=t0_; t1=t1_; dt=dt_;
  connect(this,SIGNAL(valueChanged(int)),SLOT(moved(int))); // yeah, really
}

void Scrollbar::polish() {
  QScrollBar::setRange(int(t0/GRAIN), int((t1-dt)/GRAIN));
  setPageStep(dt/GRAIN);
  int ddt = dt/GRAIN/10;
  setSingleStep(ddt?ddt:1);
}  

void Scrollbar::setRange(long long t0_, long long t1_) {
  t0=t0_;
  t1=t1_>t0_+GRAIN ? t1_:t0_+GRAIN;
  QScrollBar::setRange(int(t0/GRAIN), int((t1-dt)/GRAIN));
  moved(value());
}

void Scrollbar::setStep(long long dt_) {
  dt=dt_;
  setPageStep(dt/GRAIN);
  int ddt = dt/GRAIN/10;
  setSingleStep(ddt?ddt:1);
  moved(value());
}

void Scrollbar::moved(int x) {
  emit moved((long long)x*GRAIN,dt);
}
