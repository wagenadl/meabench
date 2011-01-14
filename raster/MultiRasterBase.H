// MultiRasterBase.H

#ifndef MULTIRASTERBASE_H

#define MULTIRASTERBASE_H

#define AUTOTIMER 1

#include <gui/QMultiGraph.H>
#include "QRDispInfo.H"
#include <spikesrv/Defs.H>

class MultiRasterBase: public QMultiGraph {
  Q_OBJECT
public:
#ifdef AUTOTIMER
  static const int IVAL_MS = 200;
#else
  static const int MINDELAY=32; // min. spikes per refresh
  static const int MINIVAL_MS = 200;
#endif
public:
  MultiRasterBase(QWidget *parent=0, char const *name=0, WFlags f=0); //:c
  virtual ~MultiRasterBase();
public slots:
  virtual void wakeupSpike(int);
  virtual void setPreMS(int prems); //:f setPreMS
  /*:D Change the amount of window length to reserve for pretrig time
       for all rasters.
   *:A prems: time in ms
   *:R -
   */
  virtual void setPostMS(int postms); //:f setPostMS
  /*:D Change the amount of window length to reserve for pretrig time
       for all rasters.
   *:A prems: time in ms
   *:R -
   */
  virtual void setPixPerTrig(int n);
  virtual void setTrigChannel(int ord);
  virtual void setSecChannel(int ord);
  virtual void setSpikeSource(const QString &);
  virtual void setDownOnly(bool on);
  virtual void setUseThresh(bool on);
  virtual void setThreshVal(int perc);
  virtual void freeze(bool);
#ifdef AUTOTIMER
  virtual void autotime();
#endif
public:
  virtual void refresh(QRDispInfo const *q=0); //:f refresh
  /*:D refresh() refreshes all plots reading recent data from the
       spike source.
   */
  timeref_t period() const { return qrs.presams + qrs.postsams; } //:f period
  /*:D period() returns the window width in sample periods
   *:A -
   *:R window width in samples
   */
  timeref_t first() const { return qrs.firsttrial; } //:f first
  /*:D first() returns the first trigger shown
   *:A -
   *:R first trigger in use.
   */
  virtual void restart(); //:f restart
  /*:D restart() clears the graphs ready for new data
   *:A -
   *:R -
   *:N latest() will return the timeref of the start of the new run when called
       immediately after a restart().
   *:E This vsn doesn't actually clear the display.
   */
  QRDispInfo dispinfo() const { return qrs; }
signals:
  void reportSourceStatus(const QString &);
  void reportTime(const QString &);
  void reportTriggers(const QString &);
protected:
  void delsrc();
  virtual void pollresponse();
  virtual void mayscroll();
protected:
  QRDispInfo qrs;
  SpikeSFCli *spikesf;
  timeref_t ssf_latest;
  bool yeah_yeah_imrunning;
  WakeupCli *sleeper;
  string sourcename; 
  class QSocketNotifier *spikeqsn;
  int trigch, secch;
  int nexttrial;
  timeref_t latest_spike_time;
  timeref_t latest_trig_time;
  timeref_t auto_trig_time;
#ifdef AUTOTIMER
  QTimer *timer;
#else
  struct timeval lastresponse;
#endif
  int threshval;
  bool usethresh;
  bool frozen;
};

#endif
