/* spikesound/Audio.C: part of meabench, an MEA recording and analysis tool
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


#include "Audio.H"
#include <stdlib.h>
#include <errno.h>
#include <stdio.h>


#include <sys/ioctl.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/soundcard.h>

Audio::Audio(char const *dev) throw(Error) {
   audio_dev=dev;
   audio_fd=-1;
   buffer=0;
   sample_freq = AUDIO_DEFAULT_FREQ;
}
void Audio::close() {
  if (audio_fd>=0)
    ::close(audio_fd);
  audio_fd = -1;
  if (buffer)
    delete [] buffer;
  buffer=0;
}

void Audio::open() throw(Error) {
  close();
  audio_fd = ::open(audio_dev.c_str(),O_WRONLY,0);

  if (audio_fd<0)
    throw SysErr("Audio","Cannot open sound device");

  int buffering = AUDIO_LOG_FRAG + (1<<16)*AUDIO_FRAGS;
  if (ioctl(audio_fd, SNDCTL_DSP_SETFRAGMENT, &buffering))
    throw SysErr("Audio","Cannot set buffer parameters");

  int format = AFMT_S16_LE;
  if (ioctl(audio_fd, SNDCTL_DSP_SETFMT, &format)<0)
    throw SysErr("Audio","Cannot set 16 bits little endian");
  if (format != AFMT_S16_LE)
    throw Error("Audio","Cannot set 16 bits little endian");

  int channels = 2;
  if (ioctl(audio_fd, SNDCTL_DSP_CHANNELS, &channels)<0)
    throw SysErr("Audio","Cannot set stereo");
  if (channels != 2)
    throw Error("Audio","Cannot set stereo");

  int freq = sample_freq;
  if (ioctl(audio_fd, SNDCTL_DSP_SPEED, &freq)<0)
    throw SysErr("Audio","Cannot set sampling frequency");
  if (freq != sample_freq)
    throw Error("Audio","Cannot set sampling frequency");

  audio_buf_info info;
  ioctl(audio_fd, SNDCTL_DSP_GETOSPACE, &info);
  fprintf(stderr,"Frag size: %i bytes. Number: %i\n",
	  info.fragsize, info.fragstotal);
  frag_length = info.fragsize / sizeof(sample);

  buffer = new sample[frag_length];
}

void Audio::write() throw(Error) {
  ssize_t n = ::write(audio_fd,buffer,frag_length*sizeof(sample));
  if (n!=frag_length*sizeof(sample))
    throw SysErr("Audio","write failed");
}
