/* scope/FreezeInfo.C: part of meabench, an MEA recording and analysis tool
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

// FreezeInfo.C

#include "FreezeInfo.H"

#include <string.h>
#include <qmessagebox.h>
//Added by qt3to4:
#include <QLabel>

FreezeInfo::FreezeInfo(QObject *parent,
		       QMultiScope *backend0, QScrollBar *frontend0,
		       QLabel *timereport0, int width_ms0,
		       RawSFCli *rawsrc, SpikeSFCli *spikesrc,
		       QWidget *hideme0) throw(Error):
  QObject(parent) {
  origrawsf = rawsrc;
  origspikesf = spikesrc;
  backend = backend0;
  frontend = frontend0;
  timereport = timereport0;
  hideme = hideme0;
  width_ms = width_ms0;
  dead = false;
  
  dbx("Freezeinfo constructor - building sources");
  if (rawsrc) 
    rawsf = new SFFreeze<Sample, RawAux>(*rawsrc);
  else
    rawsf = 0;
  if (spikesrc)
    spikesf = new SFFreeze<Spikeinfo, SpikeAux>(*spikesrc);
  else
    spikesf = 0;
  dbx("Freezeinfo constructor - done building sources");

  QSSource qss = backend->source();
  qss.sf = rawsf;
  backend->setSource(qss);
  backend->setSpikeSource(spikesf);

  if (rawsf) {
    int dt_ms = (rawsf->safelatest() - rawsf->safefirst()) / FREQKHZ;
    t0 = rawsf->safelatest() - dt_ms*FREQKHZ;
    sdbx("Freezeinfo: dt_ms=%i t0=%Li first=%Li latest=%Li",
	 dt_ms,t0,rawsf->safefirst(),rawsf->safelatest());
  } else {
    t0 = 0;
  }
  frontend->setSizePolicy(QSizePolicy(QSizePolicy::Expanding,
				      QSizePolicy::Fixed));
  frontend->show();
  if (hideme)
    hideme->hide();
  resetslider();
  connect(frontend,SIGNAL(valueChanged(int)),this,SLOT(scrollTo(int)));
  frontend->setValue(frontend->maximum());
  scrollTo(frontend->maximum());
}

FreezeInfo::~FreezeInfo() {
  if (dead) {
    // Should I or shouldn't I notify the backend? I don't know if it's
    // still alive...
  } else {
    QSSource qss = backend->source();
    qss.sf = origrawsf;
    backend->setSource(qss);
    backend->setSpikeSource(origspikesf);
    if (hideme)
      hideme->show();
    frontend->hide();
  }

  // The foll. may disturb the backend severely, if it isn't notified...
  if (rawsf)
    delete rawsf;
  if (spikesf)
    delete spikesf;
}

void FreezeInfo::resetslider() {
  dbx("Freezeinfo resetslider");
  if (rawsf) {
    int b = (rawsf->safelatest()-t0)/FREQKHZ - width_ms;
    frontend->setRange(0,b);
  }
  //  frontend->setTickInterval(width_ms);
  frontend->setPageStep(width_ms);
  frontend->setSingleStep(width_ms/10);
  frontend->setValue(frontend->value()); // hmmmm
}

void FreezeInfo::setLengthMS(int time_ms) {
  sdbx("Freezeinfo setlengthms %i",time_ms);
  backend->setLength(time_ms);
  width_ms = time_ms;
  resetslider();
}

int FreezeInfo::valueMS() {
  return frontend->value();
}

bool FreezeInfo::save(QString const &filename) {
  FILE *dst=fopen(filename.toAscii().constData(),"wb");
  if (!dst) {
    QString text = "Failed to write to `";
    text+=filename;
    text+="'. Unix error: `";
    text+=strerror(errno);
    text+="'. Sorry.";
    QMessageBox::warning(backend, "meabench: Scope", text, "Dismiss");			 
    // report error
    return false;
  }
  for (timeref_t t=rawsf->safefirst(); t<rawsf->safelatest(); t++)
    fwrite((*(SFCVoid*)(rawsf))[t], sizeof(Sample),1,dst);
  fclose(dst);
  return true;
}

void FreezeInfo::scrollTo(int dt_ms) {
  timeref_t endtime = t0 + (dt_ms+width_ms)*FREQKHZ;
  sdbx("freezeinfo scrollto: %i - endtime=%Li",dt_ms,endtime);
  backend->refresh(endtime);
  timeref_t ival = endtime - backend->first();
  timereport->setText(Sprintf("%.3f s",ival/1000./FREQKHZ).c_str());
  sdbx("freezeinfo scrollto: refresh done");
}
