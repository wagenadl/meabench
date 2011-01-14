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

#include <qcheckbox.h>
#include <qspinbox.h>
#include "MonTable.H"

#include <base/dbx.H>

/* 
 *  Constructs a MonitorI which is a child of 'parent', with the 
 *  name 'name' and widget flags set to 'f' 
 */
MonitorI::MonitorI( QWidget* parent,  const char* name, WFlags fl )
    : Monitor( parent, name, fl )
{
  lighton = false;
}

/*  
 *  Destroys the object and frees any allocated resources
 */
MonitorI::~MonitorI()
{
    // no need to delete child widgets, Qt does it all for us
}

void MonitorI::setAutoRefresh() {
  dbx("setAutoRefresh");
  killTimers();
  dbx("sAR: timers killed");
  if (autorefresh->isChecked()) {
    startTimer(100*window->value());
    dbx("sAR: timer started");
  }
  dbx("sAR: done");
}

#include <stdio.h>

void MonitorI::timerEvent(QTimerEvent *e) {
  monitor->refresh();
  lighton=!lighton;
  autolight->setBackgroundColor(QColor(0,lighton?255:128,0));
}

void MonitorI::polish() {
  monitor->refresh();
  setAutoRefresh();
}
