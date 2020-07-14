/* spikesound/Spikesound.C: part of meabench, an MEA recording and analysis tool
** Copyright (C) 2000-2003  Daniel Wagenaar (wagenaar@caltech.edu)
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

#include "Spikesound.H"
#include <QMessageBox>
#include <QRadioButton>
#include <QSpinBox>
#include <QLabel>
#include <common/CommonPath.H>
#include <common/ChannelNrs.H>
#include <math.h>
#include ".ui/ui_Spikesound.h"

/* 
 *  Constructs a Spikesound which is a child of 'parent', with the 
 *  name 'name' and widget flags set to 'f' 
 */
Spikesound::Spikesound(QWidget *parent): QWidget(parent) {
  ui = new Ui_spikesound();
  ui->setupUi(this);
  sn = 0;
  source = 0;
  for (int i=0; i<3; i++)
    useanalog[i]=false;
  setSource("spike");
  setVolume(ui->volume->value());
}

Spikesound::~Spikesound() {
  delete ui;
}

void Spikesound::setNegativeOnly(bool on) {
  negvonly = on;
}

void Spikesound::setPlay(bool on) {
  dbx(Sprintf("setplay: %c",on?'y':'n'));
  try {
    if (on) {
      audio.open();
      if (sn)
	delete sn;
      sn=0;
      sn = new QSocketNotifier(audio.audio_fd, QSocketNotifier::Write,
			       this);
      QObject::connect(sn, SIGNAL(activated(int)),
		       this, SLOT(canWrite()));
    } else {
      if (sn)
	delete sn;
      sn=0;
      audio.close();
    }
  } catch (Error const &e) {
    char const *msg = e.msg().c_str();
    QMessageBox::critical(this,
			  "Spike sound",msg,
			  QMessageBox::Ignore,
			  QMessageBox::NoButton,
			  QMessageBox::NoButton);
  }
}

inline float sq(float a) { return a*a; }

void Spikesound::canWrite() {
  dbx(Sprintf("canwrite mul=%i",multiplier));
  if (source && audio.isopen()) {
    float left=0;
    float right=0;
    int mark=0;
    timeref_t next = source->latest();
    while (last < next) {
      Spikeinfo const &si = (*source)[last];
      last++;
      int hw = si.channel;
      if (hw>=NCHANS) {
	if (hw<NCHANS+3) 
	  if (useanalog[hw-NCHANS])
	    mark++;
      } else {
	int stereo = hw2stereo(hw);
	float mul = float(si.height) / si.threshold;
	if (negvonly && si.height>0)
	  continue;
	if (rethresh && fabs(mul)<rethresh_at)
	  continue;
	right+=stereo*sq(mul); left+=(8-stereo)*sq(mul);
      }
    }
    int N = (1<<Audio::AUDIO_LOG_FRAG)/sizeof(sample);
    for (int i=0; i<N; i++) {
      float env = (1-exp(-i/(.05*N)))*exp(-i/(.25*N)) * multiplier;
      audio.buffer[i][0] = short(fl(env*left * rand() / RAND_MAX));
      audio.buffer[i][1] = short(fr(env*right * rand() / RAND_MAX));
    }
    if (mark>0) {
      for (int i=0; i<N; i++) {
	short v = short(mark*multiplier*100*sin(i*2*3.141592*440/44100));
	audio.buffer[i][0] += v;
	audio.buffer[i][1] += v;
      }
    }
    audio.write();
    dbx("wrote");
  } else {
    ui->playbutton->setChecked(false);
  }
}

void Spikesound::useRethresh(bool on) {
  setRethresh(ui->threshold->value());
  rethresh = on;
  sdbx("usethresh %c",on?'y':'n');
}

void Spikesound::setRethresh(int v) {
  rethresh_at = v/100.0;
  sdbx("setrethresh %g",rethresh_at);
}

void Spikesound::setSource(const QString &s) {
  dbx(Sprintf("setsource: %s",s.toUtf8().constData()));
  if (source)
    delete source;
  source=0;
  try {
    source = new SpikeSFCli(CommonPath(s.toUtf8().constData(),SFSUFFIX).c_str());
    last = source->latest();
    ui->status->setText("OK");
    ui->playbutton->setEnabled(true);
  } catch (Error const &e) {
    e.report();
    ui->status->setText("Not found");
    ui->playbutton->setEnabled(false);
    ui->playbutton->setChecked(false);
  }
}

void Spikesound::setVolume(int v) {
  multiplier = int(32000*pow(10,v/10.0));
}

void Spikesound::setA1(bool on) {
  useanalog[0] = on;
}
    
void Spikesound::setA2(bool on) {
  useanalog[1] = on;
}
    
void Spikesound::setA3(bool on) {
  useanalog[2] = on;
}
    
