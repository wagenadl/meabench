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

#include <qsocketnotifier.h>
#include <qlabel.h>
#include <qspinbox.h>
#include <qcheckbox.h>
#include <qcombobox.h>
#include <qmessagebox.h>
#include <qfiledialog.h>
#include <qpushbutton.h>

/* 
 *  Constructs a ScopeI which is a child of 'parent', with the 
 *  name 'name' and widget flags set to 'f' 
 */
ScopeI::ScopeI(QWidget* parent, char const *name, WFlags fl):
  Scope(parent, name, fl) {
  scopes->postConstruct();
  sleeper = spikeslp = 0;
  rawsf = 0;
  spikesf = 0;
  rawqsn = spikeqsn = 0;
  rawpoll = spkpoll = 0;
  freezer = 0;
}

/*  
 *  Destroys the object and frees any allocated resources
 */
ScopeI::~ScopeI() {
    // no need to delete child widgets, Qt does it all for us
  if (freezer)
    freezer->die();
  delsrcs();
}

template <class SFCLI> void ScopeI::setStatus(QLabel *dest,
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

void ScopeI::copybasics() {
  scopes->setExtent(range->value());
  scopes->setGuideSpacing(divs->value());
  scopes->setLength(window->value());
  scopes->setPreTrig(pretrig->value());
}

void ScopeI::polish() {
  dbx("ScopeI::polish");
  enableDbx(dbxon->isChecked());
  Scope::polish();

    QFont f( font() );
    f.setPixelSize( 14 );
    f.setBold( TRUE );
    setFont( f ); 
  
  scrollback->hide();
  
  // copy range, divs, window, &c to scope
  copybasics();
  scopes->setLayout(elclayout->currentText());
  scopes->setCenter();
  scopes->setSpeedy(speedy->currentItem());
  setRawSource(rawsrc->currentText());
  setSpikeSource(spikesrc->currentText());
  
  // reset time indicator
  time->setText("- s");
  freeze(freezeflag->isChecked());
}

void ScopeI::enableDbx(bool d) {
  switchdbx(d);
}

void ScopeI::freeze(bool on) {
  if (freezer)
    delete freezer;
  freezer = 0;
  if (on)
    freezer = new FreezeInfo(this,
			     scopes, scrollback, time,
			     window->value(),
			     rawsf, spikesf,
			     expander);
  if (on)
    savebuf->show();
  else
    savebuf->hide();

  seentrig = true;
}

void ScopeI::savefrozen() {
  if (!freezer) {
    QMessageBox::warning(this, "meabench: Scope",
			 "No scrollback buffer present. Cannot save. This is probably a bug. You're sure that `Freeze' is ticked? Sorry.",
			 "Dismiss");	
    return;
  }
  // This shouldn't be modal, I guess, but I'm lazy.
  QString fn=QFileDialog::getSaveFileName(QString::null,"*.raw",this,
					  QString::null,
					  "Save scrollback buffer");
  if (!freezer) {
    QMessageBox::warning(this, "meabench: Scope",
			 "No scrollback buffer present. Cannot save. This is probably a bug. You're sure that `Freeze' is ticked? Sorry.",
			 "Dismiss");	
    return;
  }
  if (!fn.isEmpty())
    freezer->save(fn);
}

void ScopeI::delsrcs() {
  delrawsrc();
  delspikesrc();
}

void ScopeI::delrawsrc() {
  freeze(false);
  if (scopes)
      scopes->setSource(QSSource(0));
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

void ScopeI::delspikesrc() {
  freeze(false);
  if (scopes)
    scopes->setSpikeSource(0);
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

void ScopeI::setRawSource(const QString &name) {
  current_raw_name = name.ascii();
  reopen_raw();
  freeze(false);
  freezeflag->setChecked(false);
}

void ScopeI::setLengthMS(int time_ms) {
  if (freezer)
    freezer->setLengthMS(time_ms);
  else
    scopes->setLength(time_ms);
}

void ScopeI::reopen_raw() {
  delrawsrc();
  if (current_raw_name == "") {
    spikestatus->setText("No source");
    return;
  }
  try {
    sleeper =
      new WakeupCli("scope/raw",
		    CommonPath(current_raw_name,WAKESUFFIX).c_str());
    sleeper->setival(MINDELAY*FREQKHZ); // this should be made nicer
    rawsf =
      new RawSFCli(CommonPath(current_raw_name,SFSUFFIX).c_str());
    setStatus(rawstatus,Wakeup::Null, rawpoll,rawsf);
    scopes->setSource(QSSource(rawsf,rawsf->latest(),0,
			       window->value(),
			       pretrig->value()));
    copybasics();
    rawqsn = new QSocketNotifier(sleeper->fd(),QSocketNotifier::Read,
				 this,0);
    connect(rawqsn,SIGNAL(activated(int)),this,SLOT(wakeupRaw(int)));
  } catch (Error const &e) {
    rawstatus->setText("Not found");
    e.report();
    delrawsrc();
  }
  scopes->restart();
  nexttrig = scopes->latest();
  seentrig = true;
}
/* 
 * public slot
 */
void ScopeI::setSpikeSource(const QString &name) {
  current_spike_name = name.ascii();
  reopen_spike();
}

void ScopeI::reopen_spike() {
  delspikesrc();
  if (current_spike_name == "") {
    spikestatus->setText("No source");
    return;
  }
  try {
    spikeslp = new WakeupCli("scope/spike",
			     CommonPath(current_spike_name,
					WAKESUFFIX).c_str());
    spikesf =
      new SpikeSFCli(CommonPath(current_spike_name,SFSUFFIX).c_str());
    setStatus(spikestatus,Wakeup::Null, spkpoll,spikesf);
    scopes->setSpikeSource(spikesf);
    spikeqsn = new QSocketNotifier(spikeslp->fd(),QSocketNotifier::Read,
				   this,0);
    connect(spikeqsn,SIGNAL(activated(int)),this,SLOT(wakeupSpike(int)));
  } catch (Error const &e) {
    spikestatus->setText("Not found");
    e.report();
    delspikesrc();
  }
  scopes->restart();
}

void ScopeI::wakeupRaw(int) {
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
	setStatus(rawstatus, Wakeup::Poll, rawpoll, rawsf);
      }
      if (freezeflag->isChecked())
	break;
      timeref_t sftime = rawsf->latest();
      timeref_t next;
      if (trigflag->isChecked()) {
	next = nexttrig + scopes->period() - pretrig->value()*FREQKHZ;
	if (next > sftime || next==scopes->latest())
	  break;
	seentrig = true;
      } else {
	timeref_t last = scopes->latest();
	next = last + scopes->period();
	if (next > sftime)
	  break;
	if (sftime - next > MAXLAG)
	  next = sftime; // lose some (partial) frames
	//      sdbx("Scope: refresh %.3f",next/25000.0);
      }
      scopes->refresh(next);
      timeref_t ival = next - scopes->first();
      time->setText(Sprintf("%.3f s",ival/1000./FREQKHZ).c_str());
    } break;
    case Wakeup::Start: {
      freeze(false);
      freezeflag->setChecked(false);
      rawpoll=false;
      setStatus(rawstatus, Wakeup::Start, rawpoll, rawsf);
      copybasics();
      scopes->restart();
      nexttrig = scopes->latest(); seentrig = true;
    } break;
    case Wakeup::Stop:
      rawpoll=false;
      setStatus(rawstatus, Wakeup::Stop, rawpoll, rawsf);
      break;
    default: break;
    }
  } catch (Expectable const &e) {
    // probably EOF
    e.report();
    reopen_raw();
  }
}

void ScopeI::wakeupSpike(int) {
  dbx("wakeup spike");
  try {
    int r = spikeslp->poll();
    switch (r) {
    case Wakeup::Poll: 
      if (!spkpoll) {
	spkpoll=true;
	setStatus(spikestatus, Wakeup::Poll, spkpoll, spikesf);
      }
      break;
    case Wakeup::Start:
      spkpoll=false;
      setStatus(spikestatus, Wakeup::Start, spkpoll, spikesf);
      break;
    case Wakeup::Stop: 
      spkpoll=false;
      setStatus(spikestatus, Wakeup::Stop, spkpoll, spikesf);
      break;
    default: break;
    }
  } catch (Expectable const &e) {
    // probably EOF
    e.report();
    reopen_spike();
  }
}

