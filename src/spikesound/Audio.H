/* spikesound/Audio.H: part of meabench, an MEA recording and analysis tool
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

#ifndef AUDIO_H

#define AUDIO_H

#include <string>
using namespace std;

#include <base/Error.H>

typedef short sample[2];

class Audio {
public:
  static const int AUDIO_DEFAULT_FREQ = 44100; 
  static const int AUDIO_LOG_FRAG = 12;
  // that's log_2(16384) or 93 ms per fragment
  static const int AUDIO_FRAGS = 2; 
public:
  Audio(char const *dev="/dev/dsp") ;
  ~Audio() { close(); }
  void open() ;
  void close();
  bool isopen() const { return audio_fd >= 0; }
  void write() ;
  sample *buffer;
  int frag_length; // in samples!
  int audio_fd;
  int sample_freq;
  string audio_dev;
};

#endif
