/* monitor/Monitor.C: part of meabench, an MEA recording and analysis tool
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

// Monitor.C - generated bui uic, but modified

#include "Monitor.H"
#include ".ui/ui_Monitor.h"

#include <QCheckBox>
#include <QSpinBox>
#include <QTimerEvent>
#include "MonTable.H"
#include ".ui/ui_Monitor.h"
#include <base/dbx.H>

Monitor::Monitor(QWidget* parent): QWidget(parent) {
  ui = new Ui_Monitor();
  ui->setupUi(this);
  lighton = false;
  timerId = 0;
}

Monitor::~Monitor() {
  delete ui;
}

void Monitor::setAutoRefresh() {
  dbx("setAutoRefresh");
  if (timerId)
    killTimer(timerId);
  dbx("sAR: timers killed");
  if (ui->autorefresh->isChecked()) {
    timerId = startTimer(100*ui->window->value());
    ui->autolight->setAutoFillBackground(true);
    dbx("sAR: timer started");
  } else {
    ui->autolight->setAutoFillBackground(false);
  }
  dbx("sAR: done");
}

#include <stdio.h>

void Monitor::timerEvent(QTimerEvent *e) {
  if (e->timerId()==timerId) {
    ui->monitor->refresh();
    lighton=!lighton;
    QPalette p = ui->autolight->palette();
    p.setColor(QPalette::Window, QColor(0,lighton?255:128,0));
    ui->autolight->setPalette(p);
  } else {
    killTimer(e->timerId());
  }
}

void Monitor::showEvent(QShowEvent *) {
  ui->monitor->refresh();
  setAutoRefresh();
}
