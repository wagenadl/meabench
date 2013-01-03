/* spikesound/Spikesound.H: part of meabench, an MEA recording and analysis tool
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


#ifndef SPIKESOUNDI_H
#define SPIKESOUNDI_H
#include "SpikesoundD.H"
#include <spikesrv/Defs.H>
#include <qsocketnotifier.h>
#include <qobject.h>
#include "Audio.H"
#include <spikesrv/Filters.H>

class Spikesound: public spikesound { 
  Q_OBJECT
public:
  Spikesound(QWidget* parent = 0, const char* name = 0, WFlags fl = 0);
  ~Spikesound();

public slots:
  void setRethresh(int);
  void setNegativeOnly(bool);
  void setPlay(bool);
  void useRethresh(bool);
  void setSource(const QString &);
  void setVolume(int);
  void canWrite();
  void polish();
  void setA1(bool);
  void setA2(bool);
  void setA3(bool);

private:
  Audio audio;
  QSocketNotifier *sn;
  SpikeSFCli *source;
  timeref_t last;
  bool negvonly;
  bool rethresh;
  float rethresh_at;
  int multiplier;
  bool useanalog[3];
  Bandpass fl,fr;
};

#endif // SPIKESOUNDI_H
