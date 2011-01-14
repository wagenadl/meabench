/* rawsrv/RS_Sock.H: part of meabench, an MEA recording and analysis tool
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

// RS_SOCK.H

#ifndef RS_SOCK_H

#define RS_SOCK_H

#include <rawsrv/RawSource.H>
#include <neurosock/Defs.H>
#include <rawsrv/ChannelShift.H>

class RS_Sock: public RS_Base {
private:
#ifdef MEA_ULTRAFAST
  static const int RSS_QUANTUM = 128;
#else
  static const int RSS_QUANTUM = 256;
#endif
  static const int RSS_FREQKHZ = 25;
public:
  RS_Sock(int gainsetting) throw(Error);
  ~RS_Sock();
  virtual unsigned int read(Sample *dst, unsigned int amount) throw(Error);
  virtual SFAux::HWStat status() throw(Error);
  virtual void start() throw(Error);
  virtual void stop();
  void reportInfo(bool live=false) throw(Error);
  void refreshInfo() throw(Error);
  void connect();
  virtual void setChannels(long long excludeChannels,
			   long long doubleChannels) throw(Error);
  virtual SFAux::SourceInfo sourceInfo();
  void setHost(string const &ipadr, int port);
  string const &hostAddress() { return hostaddress; }
  int hostPort()  { return port; }
  NS_Info const &getInfo() const { return info; }
  virtual void enableShiftTracking(int ch);
  virtual void disableShiftTracking();
public:
  inline static int quantum() { return RSS_QUANTUM; }
  static int ngains() { return RANGES; }
  static real_t range(int gain) { return (gain>=0 && gain<RANGES) ? RANGE[gain] : 0; }
  static char const *identify();
private:
  void readHeader() throw(Error);
  void sendCommand(NS_Command::NS_Cmd cmd, long long arg=0) throw(Error);
  NS_Info::NS_Reason readUntilInfo() throw(Error);
  void readFrame(Sample *dst) throw(Error);
  inline bool frameIsInfo(Sample *frame);
private:
  NS_Info info;
  string hostaddress;
  int port;
  int fd;
  int running;
  bool connected;
  ChannelShift cs;
  bool trackshift;
  Sample *shiftbuffer;
  int currentshift;
private:
#ifdef NIHARDWARE
  static const int RANGES=7;
  static const real_t RANGE[RANGES];
#else
  static const int RANGES=4;
  static const real_t RANGE[RANGES];
#endif
};

typedef RS_Sock RawSource;

#endif
