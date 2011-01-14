/* scope/Scope.H: part of meabench, an MEA recording and analysis tool
** Copyright (C) 2000-2002  Daniel Wagenaar (wagenaar@caltech.edu)
**
** This program is free software; you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation; either version 2 of the License, or
** (at your option) any later version.
**
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with this program; if not, write to the Free Software
** Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

#ifndef SCOPEI_H
#define SCOPEI_H

#include "ScopeD.H"
#include <rawsrv/Defs.H>
#include <spikesrv/Defs.H>

class ScopeI : public Scope { 
  Q_OBJECT
public:
  static const int MINDELAY = 50; // ms
public:
  ScopeI( QWidget* parent = 0, const char* name = 0, WFlags fl = 0 );
  ~ScopeI();
public slots:
  void polish();
  void setRawSource(const QString &);
  void setSpikeSource(const QString &);
  void wakeupRaw(int);
  void wakeupSpike(int);
  void enableDbx(bool);
  void freeze(bool);
  void savefrozen();
  void setLengthMS(int);
private:
  void copybasics();
  void delsrcs();
  void delrawsrc();
  void delspikesrc();
  void reopen_raw();
  void reopen_spike();
  template <class SFCLI> void setStatus(QLabel *dest, Wakeup::Codes code, bool polling, SFCLI *sf);
private:
  WakeupCli *sleeper, *spikeslp;
  RawSFCli *rawsf;
  SpikeSFCli *spikesf;
  class QSocketNotifier *rawqsn, *spikeqsn;
  string current_raw_name, current_spike_name;
  bool rawpoll, spkpoll;
  class FreezeInfo *freezer;
  timeref_t nexttrig;
  bool seentrig;
};

#endif // SCOPEI_H
