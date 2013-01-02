/* monitor/MeaMon.H: part of meabench, an MEA recording and analysis tool
** Copyright (C) 2004  Daniel Wagenaar (wagenaar@caltech.edu)
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

#ifndef MEAMON_H

#define MEAMON_H

#include "tern.H"
#include <string>
#include <QTimerEvent>

using namespace std;
#include <base/Timeref.H>
#include <QTableWidget>

enum Progs { RAWSRV=0, RERAW, RESPIKE, EXTRACTWINDOW, SALPA, P60HZ, SPIKESRV, SPRAWSRV, RMS, NPROGS };

class MeaMon: public QTableWidget  {
  Q_OBJECT
public:
  MeaMon(QWidget *parent=0);
  ~MeaMon();
public slots:
  void refresh();
  void setAutoRefresh(int ms);
protected:
  void showEvent(QShowEvent *);
  void timerEvent(QTimerEvent *e);
private:
  void check(enum Progs p);
  virtual void setText(int r, int c, string const &s);
private:
  static char const *progname(enum Progs p);
  static char const *shmname(enum Progs p);
  static bool israw(enum Progs p);
  static bool isspike(enum Progs p);
  //  static int pidof(enum Progs p);
  static string nicethousands(timeref_t t);
  void resizeme();
private:
  class SFCVoid *sfcli[NPROGS];
  class WakeupCli *sleepers[NPROGS];
  tern running[NPROGS];
  int runcount[NPROGS];
  int trigcount[NPROGS];
private:
  int timerId;
};

#endif
