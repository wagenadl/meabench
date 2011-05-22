/* common/EasyClient.H: part of meabench, an MEA recording and analysis tool
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

// EasyClient.H

#ifndef EASYCLIENT_H

#define EASYCLIENT_H

#define COMPILEEASYCLIENT 0

#include <base/SFCli.H>
#include <base/WakeupCli.H>
#include <base/Error.H>
#include <common/Config.H>
#include <common/CommonPath.H>
#include <rawsrv/Defs.H>
#include <spikesrv/Defs.H>
#include <stdio.h>
#include <common/MultiFile.H>

template <class Data, class Aux> class EasyClient: //:C EasyClient
/*:D EasyClient provides access to meabench streams through a simplified
     interface. You probably don't want to use this class, because
     StreamClient is even easier. If you need more control, SFCli and
     WakeupCli are a better choice. */
  public SFCli<Data,Aux> {
public:
  static const int OVERRUN_THRESHOLD = 80; //:n OVERRUN_THRESHOLD
  /*:D This constant defines at which buffer use percentage ensure() will
       throw an error.
  */
  
  // Useful functions from SFCli:
  // first(), latest(), []
  // end() and all iterator stuff
  // aux()
public:
  EasyClient(string const &streamname) throw(Error): //:c
    /*:D Construct a client based on a stream. 
     *:A streamname. The streamname must be
         specified without extensions and directory.
	 (Example: "reraw", not "/home/wagenaar/.meabench/reraw.sf".)
     *:E Errors thrown if the stream does not exist.
     *:N This constructor does not wait for a start on the stream. See
         StreamClient below for a convenient system that does.
       . The server is requested to send polls every 50*25 records. This
         is probably good for raw data, but may be too much for spike data.
	 Use setival() to change this value.
    */
    SFCli<Data,Aux>(CommonPath(streamname,SFSUFFIX).c_str()),
    waker("EasyClient",CommonPath(streamname,WAKESUFFIX).c_str()) {
    waker.setival(50*FREQKHZ);
  }
  void setival(int records) { //:f setival
    /*:D setival(n) sets the interval for wakeup calls to this client to n.
         This interval affects the granularity of block() and its friends.
    */
    waker.setival(records);
  }
  void ensure(timeref_t const &t, int thresh=OVERRUN_THRESHOLD) { //:f ensure
    /*:D Ensure(t) checks whether timeref t is still available in the
         fifo. If it suspects it isn't, an error is thrown.
    */
    if (SFCli<Data,Aux>::bufuse_update(t) >= thresh)
      throw Error("EasyClient","Buffer overrun");
  }
  string bufuse() { return SFCli<Data,Aux>::bufuse_deepreport(); } //:f bufuse
  /*:D Returns the buffer usage of the stream as a string. */
  bool block() throw(Error) { //:f block
    /*:D block() puts the caller to sleep until a message is received from
         the server.
     *:A -
     *:R True unless the code received is `Stop'.
     */
    if (waker.block()==Wakeup::Stop)
      return false;
    return true;
  }
  bool blockuntil(timeref_t t) throw(Error) { //:f blockuntil
    /*:D blockuntil(t) blocks until timeref t is available on the fifo,
         or until a Stop code is received.
     *:A timestamp
     *:R True unless a `Stop' code is received.
     */
    int res=0;
    while (SFCli<Data,Aux>::latest()<t)
      if ((res=waker.block())==Wakeup::Stop)
	return false;
    return true;
  }
  bool trig(int dt) throw(Error) { //:f trig
    /*:D trig(dt) blocks until dt samples after a trigger.
     *:A dt: time to wait post trigger (samples)
     *:R True unless a `Stop' code is received.
     *:N Do not use this on a spike stream. It won't work, because timerefs
         in spikestreams counts spikes and do not correspond to real times.
    */
    int res=0;
    while ((res=waker.block())!=Wakeup::Trig)
      if (res==Wakeup::Stop)
	return false;
    return block(SFCli<Data,Aux>::aux()->trig.t_latest +
		 SFCli<Data,Aux>::first() + dt);
  }
  void waitstart() throw(Error) { //:f waitstart
    /*:D waitstart() blocks until a `Start' code is received.
     *:A -
     *:R -
     *:N This fn does not treat `Stop' specially.
     */
    while (waker.block()!=Wakeup::Start)
      ;
  }
  bool read(Data *dest, timeref_t start, int length) throw(Error) { //:f read
    /*:D read(dst, t, l) reads l records into the destination buffer starting
         from timeref t.
     *:A dest: destination pointer for data
       . start: timeref in stream for start of transfer
       . length: number of objects (NOT bytes!) to transfer
     *:R True unless `Stop' was received prematurely.
     *:E This fn will transfer rubbish if length exceeds (or comes near) the
         fifo buffer size, so don't transfer unreasonably large chunks.
    */
    if (!blockuntil(start+length))
      return false;
    while (length--)
      *dest++ = (*this)[start++];
    return true;
  }
  int readn(Data *dest, timeref_t start, int length) throw(Error) { //:f readn
    /*:D readn(dst, t, l) reads l records into the destination buffer starting
         from timeref t.
     *:A dest: destination pointer for data
       . start: timeref in stream for start of transfer
       . length: number of objects (NOT bytes!) to transfer
     *:R Number of records read. This will equal l, unless `Stop' was
         received prematurely.
     *:E This fn will transfer rubbish if length exceeds (or comes near) the
         fifo buffer size, so don't transfer unreasonably large chunks.
    */
    if (!blockuntil(start+length))
      length = SFCli<Data,Aux>::latest() - start;
    int l=length;
    while (l--)
      *dest++ = (*this)[start++];
    return length;
  }
private:
  WakeupCli waker;
};

template <class Data, class Aux> class StreamClient { //:C StreamClient
  /*:D For the ultimate in ease of access, use StreamClient to read from
       meabench streams. It can read either from a meabench stream or
       from a file, presenting a uniform interface for both.
   *:N This is a class template. Convenient instantiations for `raw' and
       `spike' streams are provided below. */
 public:
  StreamClient(string const &streamname, bool waitforstart) throw(Error) { //:c
    /*:D StreamClient(sn, wfs) constructs a StreamClient connected to
         a meabench stream.
     *:A streamname. The streamname must be
         specified without extensions and directory.
	 (Example: "reraw", not "/home/wagenaar/.meabench/reraw.sf".)
       . wfs: set to true to force waiting for a `Start' command from the
         stream.
     *:N The wakeup i/val is set to 1, so granularity of read() can be as
         small as you want.
    */
    ec = new EasyClient<Data,Aux>(streamname);
    ec->setival(1);
    if (waitforstart) {
      ec->waitstart();
      t=ec->first();
    } else {
      t=ec->latest();
    }
    mf = 0;
  }
  StreamClient(string const &fn) throw(Error) { //:c
    /*:D StreamClient(fn) constructs a StreamClient that reads from a file.
     *:A fn: filename to open, including suffixes.
     *:N If the file is of typical meabench construction, then continuation
         files ("foo.raw-1", etc.) are automatically loaded too.
     */
    ec=0;
    mf = new MultiFile(fn);
    disksize=sizeof(Data);
  }
  ~StreamClient() {
    if (ec)
      delete ec;
    if (mf)
      delete mf;
  }
  StreamClient(FILE *fh) throw(Error) { //:c
    /*:D StreamClient(fh) constructs a StreamClient that reads from an already
         opened file. Typical use is to read from stdin.
     *:A fh: filehandle to read from.
     *:N The file is not closed when the StreamClient is destructed.
     */
    if (!fh)
      throw Error("StreamClient","Bad file handle");
    mf = new MultiFile(fh);
    ec=0;
    disksize=sizeof(Data);
  }
  int lastlength() const { return lastlen; } //:f lastlength
  /*:D lastlength() returns the amount of data returned in the last read()
       call. This is useful when read() returns false.
  */
  void seek(timeref_t lof) {
    if (ec)
      throw Error("StreamClient", "Cannot seek on stream");
    mf->seek(lof);
  }
    
  bool read(Data *dest, int length) { //:f read
    /*:D read(dst,l) reads l records from the stream or file into dst.
      .  Unlike EasyClient's read() function, this is always sequential.
     *:A dest: destination pointer for data.
      .  length: number of records (NOT bytes!) to transfer.
     *:R True unless a `Stop' code or EOF is received prematurely.
     *:E This fn will transfer rubbish if length exceeds (or comes near) the
         fifo buffer size, so don't transfer unreasonably large chunks.
     */
    if (ec) {
      bool r = ec->read(dest,t,length);
      t+=length;
      lastlen=length;
      return r;
    } else {
      int r=mf->read(dest,sizeof(Data),length);
      lastlen=r;
      return r==length;
    }
  }
  int readn(Data *dest, int length) { //:f readn
    /*:D readn(dst,l) reads l records from the stream or file into dst.
      .  Unlike EasyClient's read() function, this is always sequential.
     *:A dest: destination pointer for data.
      .  length: number of records (NOT bytes!) to transfer.
     *:R Number of records read. This will equal l, unless `Stop' was
         received prematurely.
     *:E This fn will transfer rubbish if length exceeds (or comes near) the
         fifo buffer size, so don't transfer unreasonably large chunks.
     */
    lastlen = 0;
    if (ec) {
      length = ec->readn(dest,t,length);
      t+=length;
      lastlen=length;
      return length;
    } else {
      Data *dstptr = dest;
      for (int i=0; i<length; i++) {
	int r = mf->read(dstptr, disksize, 1);
	if (r==1) {
	  dstptr++;
	  lastlen++;
	} else
	  break;
      }
      return lastlen;
    }
  }
  // Triggering *may* be implemented in a later vsn, by reading .trig files.
  // Other helpful stuff *may* be implemented by reading .desc files.
private:
  EasyClient<Data,Aux> *ec;
  MultiFile *mf;
  timeref_t t;
  int lastlen;
  int disksize;
protected:
  void setdisksize(int n) { disksize=n; }
};

typedef EasyClient<Sample,RawAux> FullRaw;
typedef EasyClient<Spikeinfo,SpikeAux> FullSpike;

typedef StreamClient<Sample,RawAux> StreamRaw; //:C StreamRaw
class StreamSpike: public StreamClient<Spikeinfo,SpikeAux> { //:C StreamSpike
public:
  StreamSpike(string const &streamname, bool waitforstart) throw(Error):
    StreamClient<Spikeinfo,SpikeAux>(streamname,waitforstart) { }
  StreamSpike(FILE *fh) throw(Error): StreamClient<Spikeinfo,SpikeAux>(fh) {
    setdisksize(164);
  }
};
/*:D StreamRaw and StreamSpike are instantiations of StreamClient for use
     with raw and spike streams.
*/

#endif
