// ControlPanel.H

#ifndef CONTROLPAN_H

#define CONTROLPAN_H

#include <qobject.h>
#include <qsocketnotifier.h>
#include <spikesrv/Defs.H>

#include "ControlInfo.H"

class ControlPanel: public QObject {
  Q_OBJECT;
public:
  ControlPanel(class Storage *src, class QApplication *qap=0);
  ~ControlPanel();
  ControlInfo const *controlinfo() const { return &info; }
public slots:
  void create_spont();
  void create_8x8rec();
  void create_8x8stim();
  void create_vstim();
  void create_hstim();
  void create_cont();
  void update_time();
  void update_source(QString const &);
  void update_pre(int);
  void update_post(int);
  void update_up(bool);
  void update_down(bool);
  void update_pixtri(int);
  void wakeup();
signals:
  void updateinfo(); // signals that SimpleRasters should be redrawn
  void newsource(); // same, but requires rebuild of RasterData
private:
  class controlpanel *ctrlp;
  Storage *storage;
  ControlInfo info;
  SpikeSFCli *spksrc;
  WakeupCli *sleeper;
  QSocketNotifier *socknotif;
  timeref_t latestspk;
  bool isnew;
private:
  void closesrc();
};

#endif
