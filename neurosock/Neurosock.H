/* Neurosock/Neurosock.H: part of meabench, an MEA recording and analysis tool
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

// Neurosock.H


#ifndef NEUROSOCK_H

#define NEUROSOCK_H


#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>

#ifdef UEIHARDWARE
#include "srvPdMeata.H"
#endif
#ifdef MCSHARDWARE
#include "srvMCMeata.H"
#endif

#include "FrameBuffer.H"


class Neurosock {
public:
  Neurosock(srvMeata *srv, FrameBuffer *fb, int port0, int myhalf);
  ~Neurosock();
  void Poll();
private:
  int connect();
  void processCommand();
  void sendDataFrame();
  void dropFrame();
  void sendInfoFrame(NS_Info::NS_Reason reason);
  void sendFrame(char *ptr);
  void sendHeader();
  void closeSocket();
  void setExclude(long long arg) { excludeChannels=arg; }
  void setDouble(long long arg) { doubleChannels=arg; }
private:
  int confd;
  int sockfd;
  int consecutive_errors;
  int chn_setting; //Set of channels to read
  bool isActive;
  int port;
  struct pollfd pollfd;
  srvMeata *srv;
  FrameBuffer *fb;
  long long excludeChannels;
  long long doubleChannels;
private:
  static int nActive;
};


#endif
