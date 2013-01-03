/* monitor/MonTable.H: part of meabench, an MEA recording and analysis tool
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

// MonTable.H

#ifndef MONTABLE_H

#define MONTABLE_H

#include <base/Timeref.H>
#include <QTableWidget>
#include "Progs.H"

class MonTable: public QTableWidget {
  Q_OBJECT
public:
  MonTable(QWidget *parent=0);
  ~MonTable();
public slots:
//  void polish();
  void refresh();
private:
  class WakeupCli *sleepers[NPROGS];
  int state[NPROGS];
  int pollcnt[NPROGS];
  int trigcnt[NPROGS];
  Prop props[NPROGS];
private:
  static string pidof(string const &prog);
  static string shmidof(string const &shm, string *first=0, string *latest=0);
  static string nicethousands(timeref_t t);
  string stateof(enum Progs r, string const &sock); // unlike pidof and shmidof, this function keeps state and cannot be static.
  virtual void setText(int r, int c, string const &s);
};

#endif
