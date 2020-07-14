// ControlPanel.C

#include "ControlPanel.H"

#include ".ui/ui_controlpanel.h"
#include "Storage.H"
#include "OnePanel.H"
#include "GridPanel.H"
#include "LinePanel.H"
#include "ContPanel.H"

#include <QPushButton>
#include <QSpinBox>
#include <QComboBox>
#include <QCheckBox>
#include <QLabel>

#include <spikesrv/Defs.H>
#include <common/CommonPath.H>
#include <base/dbx.H>

ControlPanel::ControlPanel(Storage *strg) {
  storage = strg;
  ctrlp = new Ui_controlpanel();
  ctrlp->setupUi(this);
  
  spksrc=0; sleeper=0; socknotif=0; isnew=false;

  info.presams = ctrlp->t_pre->value()*FREQKHZ;
  info.postsams = ctrlp->t_post->value()*FREQKHZ;
  info.up = ctrlp->use_up->isChecked();
  info.down = ctrlp->use_down->isChecked();
  info.pixtri = ctrlp->pixtri->value();

  connect(ctrlp->open_spont,SIGNAL(clicked()),SLOT(create_spont()));
  connect(ctrlp->open_8x8rec,SIGNAL(clicked()),SLOT(create_8x8rec()));
  connect(ctrlp->open_8x8stim,SIGNAL(clicked()),SLOT(create_8x8stim()));
  connect(ctrlp->open_vstim,SIGNAL(clicked()),SLOT(create_vstim()));
  connect(ctrlp->open_hstim,SIGNAL(clicked()),SLOT(create_hstim()));
  connect(ctrlp->open_cont,SIGNAL(clicked()),SLOT(create_cont()));

  connect(ctrlp->source,SIGNAL(activated(const QString &)),SLOT(update_source(const QString &)));

  connect(ctrlp->use_up,SIGNAL(toggled(bool)),SLOT(update_up(bool)));
  connect(ctrlp->use_down,SIGNAL(toggled(bool)),SLOT(update_down(bool)));
  connect(ctrlp->t_pre,SIGNAL(valueChanged(int)),SLOT(update_pre(int)));
  connect(ctrlp->t_post,SIGNAL(valueChanged(int)),SLOT(update_post(int)));
  connect(ctrlp->pixtri,SIGNAL(valueChanged(int)),SLOT(update_pixtri(int)));
  
  connect(storage,SIGNAL(update()),SLOT(update_time()));
  
};

ControlPanel::~ControlPanel() {
  closesrc();
}

void ControlPanel::closesrc() {
  if (sleeper)
    delete sleeper;
  sleeper=0;

  if (spksrc)
    delete spksrc;
  spksrc=0;

  if (socknotif)
    delete socknotif;
  socknotif=0;
}

//////////////////////////////////////////////////////////////////////
void ControlPanel::create_spont() {
  sdbx("create_spont");
  OnePanel *p = new OnePanel(0,storage,this);
  p->setWindowTitle("Flex raster - spontaneous");
  p->show();
}

void ControlPanel::create_8x8rec() {
  sdbx("create_8x8rec");
  GridPanel *p = new GridPanel(0,storage,this,true);
  p->setWindowTitle("Flex raster - by recording channel");
  p->resize(800,600);
  p->show();
}

void ControlPanel::create_8x8stim() {
  sdbx("create_8x8stim");
  GridPanel *p = new GridPanel(0,storage,this,false);
  p->setWindowTitle("Flex raster - by stimulation channel");
  p->resize(800,600);
  p->show();
}

void ControlPanel::create_vstim() {
  sdbx("create_vstim");
  LinePanel *p = new LinePanel(0,storage,this,false);
  p->setWindowTitle("Flex raster - by stimulation channel");
  p->resize(800,600);
  p->show();
}

void ControlPanel::create_hstim() {
  sdbx("create_hstim");
  LinePanel *p = new LinePanel(0,storage,this,true);
  p->setWindowTitle("Flex raster - by stimulation channel");
  p->resize(800,600);
  p->show();
}

void ControlPanel::create_cont() {
  sdbx("create_cont");
  ContPanel *p = new ContPanel(0,storage,this);
  sdbx("cc: %p",p);
  p->setWindowTitle("Flex raster - continuous");
  p->resize(1000,400);
  p->show();
}

//////////////////////////////////////////////////////////////////////
void ControlPanel::update_pre(int n) {
  sdbx("update_pre");
  info.presams=n*FREQKHZ; emit updateinfo();
}

void ControlPanel::update_post(int n) {
  sdbx("update_post");
  info.postsams=n*FREQKHZ; emit updateinfo();
}

void ControlPanel::update_pixtri(int n) {
  sdbx("update_pixtri");
  info.pixtri=n; emit updateinfo();
}

void ControlPanel::update_up(bool n) {
  sdbx("update_up");
  info.up=n; emit newsource();
}

void ControlPanel::update_down(bool n) {
  sdbx("update_down");
  info.down=n; emit newsource();
}

//////////////////////////////////////////////////////////////////////
void ControlPanel::update_source(QString const &s) {
  closesrc();
  if (s=="") {
    ctrlp->source_status->setText("No source");
  } else {
    try {
      sleeper = new WakeupCli("flexraster",
			      CommonPath(s.toUtf8().constData(),WAKESUFFIX).c_str());
      sleeper->setival(100); // hmm...
      spksrc = new SpikeSFCli(CommonPath(s.toUtf8().constData(),SFSUFFIX).c_str());
      latestspk = spksrc->latest();
      socknotif = new QSocketNotifier(sleeper->fd(),QSocketNotifier::Read,this);
      connect(socknotif,SIGNAL(activated(int)),this,SLOT(wakeup()));
      ctrlp->source_status->setText("Connected");
      isnew=true;
    } catch (Error const &e) {
      ctrlp->source_status->setText("Not found");
      e.report();
    }
  }
  storage->reset();
  emit newsource();
}

void ControlPanel::wakeup() {
  try {
    int r = sleeper->poll();
    switch (r) {
    case Wakeup::Start:
      ctrlp->source_status->setText("Started");
      storage->reset();
      emit newsource();
      isnew=true;
      break;
    case Wakeup::Stop:
      ctrlp->source_status->setText("Stopped");
      isnew=true;
      break;
    case Wakeup::Poll:
      if (isnew) {
	ctrlp->source_status->setText("Running");
	isnew=false;
      }
      break;
    }
  } catch (Expectable const &e) {
    // probably EOF
    e.report();
    update_source(ctrlp->source->currentText()); // re-open
    return;
  }

  Spikeinfo const *first = &(*spksrc)[latestspk];
  timeref_t current = spksrc->latest();
  Spikeinfo const *last = &(*spksrc)[current];
  if (first<last) {
    storage->add(first,last-first);
  } else if (last<first) {
    Spikeinfo const *start = spksrc->gettodata();
    Spikeinfo const *end = start + spksrc->bufsize();
    storage->add(first,end-first,false);
    storage->add(start,last-start);
  }
  latestspk=current;
}

//////////////////////////////////////////////////////////////////////
void ControlPanel::update_time() {
  double time = storage->time()/1000./FREQKHZ;
  QString s;
  s.sprintf("%.3f",time);
  ctrlp->time->setText(s);
  s.sprintf("%i",storage->ntrials());
  ctrlp->trigs->setText(s);
}
