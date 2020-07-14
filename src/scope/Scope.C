/* scope/Scope.C: part of meabench, an MEA recording and analysis tool
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

// Scope.C - made from ScopeI.C made from Scope.ui, but modified

/* FIXME (DW 010111) Is it appropriate for delxxxsrc to tell scopes
   about it? Or should this be done at higher level? I think it's OK,
   because both are used exclusively by reopen_xxx. */

#include "QMultiScope.H"
#include "QScope.H"
#include <rawsrv/Defs.H>
#include <spikesrv/Defs.H>
#include <base/WakeupCli.H>
#include <base/dbx.H>
#include <base/Sprintf.H>
#include <base/Error.H>
#include <common/CommonPath.H>

#include "FreezeInfo.H"
#include "Scope.H"
#include "Defs.H"

#include ".ui/ui_Scope.h"

#include <QSocketNotifier>
#include <QLabel>
#include <QSpinBox>
#include <QCheckBox>
#include <QComboBox>
#include <QMessageBox>
#include <QFileDialog>
#include <QPushButton>

/* 
 *  Constructs a Scope which is a child of 'parent', with the 
 *  name 'name' and widget flags set to 'f' 
 */
Scope::Scope(QWidget *parent): QWidget(parent) {
  ui = new Ui_Scope();
  ui->setupUi(this);
  ui->scopes->postConstruct();
  sleeper = spikeslp = 0;
  rawsf = 0;
  spikesf = 0;
  rawqsn = spikeqsn = 0;
  rawpoll = spkpoll = 0;
  freezer = 0;

  enableDbx(ui->dbxon->isChecked());

  QFont f( font() );
  f.setPixelSize( 14 );
  f.setBold( true );
  setFont( f ); 
  
  ui->scrollback->hide();
  
  // copy range, divs, window, &c to scope
  copybasics();
  ui->scopes->setLayout(ui->elclayout->currentText());
  ui->scopes->setCenter();
  ui->scopes->setSpeedy(ui->speedy->currentIndex());
  setRawSource(ui->rawsrc->currentText());
  setSpikeSource(ui->spikesrc->currentText());
  
  // reset time indicator
  ui->time->setText("- s");
  freeze(ui->freezeflag->isChecked());
}

Scope::~Scope() {
  if (freezer)
    freezer->die();
  delsrcs();
  delete ui;
}

template <class SFCLI> void Scope::setStatus(QLabel *dest,
					      Wakeup::Codes code,
					      bool polling, SFCLI *sf) {
  bool sfrun = sf?sf->running():false;
  bool sfhasrun = sf?sf->hasrun():false;
  string sfstat;
  if (sfrun) 
    sfstat = "Run";
  else if (sfhasrun)
    sfstat = "Stop";
  else if (sf)
    sfstat = "New";
  else
    sfstat = "Absent";
  string wakestat;
  if (polling)
    wakestat = "Run";
  else if (code==Wakeup::Start)
    wakestat = "Start";
  else if (code==Wakeup::Stop)
    wakestat = "Stop";
  else
    wakestat = "Ready";
  string txt = sfstat + "/" + wakestat;
  dest->setText(txt.c_str());
}

void Scope::copybasics() {
  ui->scopes->setExtent(ui->range->value());
  ui->scopes->setGuideSpacing(ui->divs->value());
  ui->scopes->setLength(ui->window->value());
  ui->scopes->setPreTrig(ui->pretrig->value());
}

void Scope::enableDbx(bool d) {
  switchdbx(d);
}

void Scope::freeze(bool on) {
  if (freezer)
    delete freezer;
  freezer = 0;
  if (on)
    freezer = new FreezeInfo(this,
			     ui->scopes, ui->scrollback, ui->time,
			     ui->window->value(),
			     rawsf, spikesf,
			     ui->expander);
  if (on)
    ui->savebuf->show();
  else
    ui->savebuf->hide();

  seentrig = true;
}

void Scope::savefrozen() {
  if (!freezer) {
    QMessageBox::warning(this, "meabench: Scope",
			 "No scrollback buffer present. Cannot save. This is probably a bug. You're sure that `Freeze' is ticked? Sorry.",
			 "Dismiss");	
    return;
  }
  // This shouldn't be modal, I guess, but I'm lazy.
  QString fn=QFileDialog::getSaveFileName(this,
					  "Save scrollback buffer",
					  QString(),
					  "*.raw");
  if (!freezer) {
    QMessageBox::warning(this, "meabench: Scope",
			 "No scrollback buffer present. Cannot save. This is probably a bug. You're sure that `Freeze' is ticked? Sorry.",
			 "Dismiss");	
    return;
  }
  if (!fn.isEmpty())
    freezer->save(fn);
}

void Scope::delsrcs() {
  delrawsrc();
  delspikesrc();
}

void Scope::delrawsrc() {
  freeze(false);
  if (ui->scopes)
      ui->scopes->setSource(QSSource(0));
  if (sleeper)
    delete sleeper;
  sleeper=0;
  if (rawsf)
    delete rawsf;
  rawsf=0;
  if (rawqsn)
    delete rawqsn;
  rawqsn=0;
}

void Scope::delspikesrc() {
  freeze(false);
  if (ui->scopes)
    ui->scopes->setSpikeSource(0);
  if (spikeslp)
    delete spikeslp;
  spikeslp=0;
  if (spikesf)
    delete spikesf;
  spikesf=0;
  if (spikeqsn)
    delete spikeqsn;
  spikeqsn=0;
}

void Scope::setRawSource(const QString &name) {
  current_raw_name = name.toUtf8().constData();
  reopen_raw();
  freeze(false);
  ui->freezeflag->setChecked(false);
}

void Scope::setLengthMS(int time_ms) {
  if (freezer)
    freezer->setLengthMS(time_ms);
  else
    ui->scopes->setLength(time_ms);
}

void Scope::reopen_raw() {
  delrawsrc();
  if (current_raw_name == "") {
    ui->spikestatus->setText("No source");
    return;
  }
  try {
    sleeper =
      new WakeupCli("scope/raw",
		    CommonPath(current_raw_name,WAKESUFFIX).c_str());
    sleeper->setival(MINDELAY*FREQKHZ); // this should be made nicer
    rawsf =
      new RawSFCli(CommonPath(current_raw_name,SFSUFFIX).c_str());
    setStatus(ui->rawstatus,Wakeup::Null, rawpoll,rawsf);
    ui->scopes->setSource(QSSource(rawsf,rawsf->latest(),0,
			       ui->window->value(),
			       ui->pretrig->value()));
    copybasics();
    rawqsn = new QSocketNotifier(sleeper->fd(),QSocketNotifier::Read,
				 this);
    connect(rawqsn,SIGNAL(activated(int)),this,SLOT(wakeupRaw(int)));
  } catch (Error const &e) {
    ui->rawstatus->setText("Not found");
    e.report();
    delrawsrc();
  }
  ui->scopes->restart();
  nexttrig = ui->scopes->latest();
  seentrig = true;
}
/* 
 * public slot
 */
void Scope::setSpikeSource(const QString &name) {
  current_spike_name = name.toUtf8().constData();
  reopen_spike();
}

void Scope::reopen_spike() {
  delspikesrc();
  if (current_spike_name == "") {
    ui->spikestatus->setText("No source");
    return;
  }
  try {
    spikeslp = new WakeupCli("scope/spike",
			     CommonPath(current_spike_name,
					WAKESUFFIX).c_str());
    spikesf =
      new SpikeSFCli(CommonPath(current_spike_name,SFSUFFIX).c_str());
    setStatus(ui->spikestatus,Wakeup::Null, spkpoll,spikesf);
    ui->scopes->setSpikeSource(spikesf);
    spikeqsn = new QSocketNotifier(spikeslp->fd(),QSocketNotifier::Read,
				   this);
    connect(spikeqsn,SIGNAL(activated(int)),this,SLOT(wakeupSpike(int)));
  } catch (Error const &e) {
    ui->spikestatus->setText("Not found");
    e.report();
    delspikesrc();
  }
  ui->scopes->restart();
}

void Scope::wakeupRaw(int) {
  //  dbx("wakeup raw");
  try {
    int r = sleeper->poll();
    switch (r) {
    case Wakeup::Trig: // fall through to Poll stuff.
      if (seentrig) {
	nexttrig = rawsf->aux()->trig.t_latest  + rawsf->first();
	seentrig = false;
      }
    case Wakeup::Poll:
    {
      if (!rawpoll) {
	rawpoll=true;
	setStatus(ui->rawstatus, Wakeup::Poll, rawpoll, rawsf);
      }
      if (ui->freezeflag->isChecked())
	break;
      timeref_t sftime = rawsf->latest();
      timeref_t next;
      if (ui->trigflag->isChecked()) {
	next = nexttrig + ui->scopes->period() - ui->pretrig->value()*FREQKHZ;
	if (next > sftime || next==ui->scopes->latest())
	  break;
	seentrig = true;
      } else {
	timeref_t last = ui->scopes->latest();
	next = last + ui->scopes->period();
	if (next > sftime)
	  break;
	if (sftime - next > MAXLAG)
	  next = sftime; // lose some (partial) frames
	//      sdbx("Scope: refresh %.3f",next/25000.0);
      }
      ui->scopes->refresh(next);
      timeref_t ival = next - ui->scopes->first();
      ui->time->setText(Sprintf("%.3f s",ival/1000./FREQKHZ).c_str());
    } break;
    case Wakeup::Start: {
      freeze(false);
      ui->freezeflag->setChecked(false);
      rawpoll=false;
      setStatus(ui->rawstatus, Wakeup::Start, rawpoll, rawsf);
      copybasics();
      ui->scopes->restart();
      nexttrig = ui->scopes->latest(); seentrig = true;
    } break;
    case Wakeup::Stop:
      rawpoll=false;
      setStatus(ui->rawstatus, Wakeup::Stop, rawpoll, rawsf);
      break;
    default: break;
    }
  } catch (Expectable const &e) {
    // probably EOF
    e.report();
    reopen_raw();
  }
}

void Scope::wakeupSpike(int) {
  dbx("wakeup spike");
  try {
    int r = spikeslp->poll();
    switch (r) {
    case Wakeup::Poll: 
      if (!spkpoll) {
	spkpoll=true;
	setStatus(ui->spikestatus, Wakeup::Poll, spkpoll, spikesf);
      }
      break;
    case Wakeup::Start:
      spkpoll=false;
      setStatus(ui->spikestatus, Wakeup::Start, spkpoll, spikesf);
      break;
    case Wakeup::Stop: 
      spkpoll=false;
      setStatus(ui->spikestatus, Wakeup::Stop, spkpoll, spikesf);
      break;
    default: break;
    }
  } catch (Expectable const &e) {
    // probably EOF
    e.report();
    reopen_spike();
  }
}

