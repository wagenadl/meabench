//Neurosock.C

// socktest/server.C - test server for a tcp domain socket


#include "Error.H"
#include "FrameBuffer.H"
#include "Neurosock.H"

#define WARN_PARTIAL if (rem) fprintf(stderr,"Sent partial data [%i]\n",n);

int Neurosock::nActive = 0;

Neurosock::Neurosock(srvMeata *srv0, FrameBuffer *fb0,
		     int port0, int myhalf) {
  srv=srv0; fb=fb0; port = port0;
  consecutive_errors = 0;
  confd = -1;
  chn_setting = myhalf;

  fprintf(stderr,"Creating socket on port 0x%x for %i...\n",port,chn_setting);
  sockfd = socket(PF_INET, SOCK_STREAM, 0);
  struct sockaddr_in srvadr;
  srvadr.sin_family = AF_INET;
  srvadr.sin_port = htons(port);
  srvadr.sin_addr.s_addr = INADDR_ANY;

  if (bind(sockfd, (struct sockaddr*)&srvadr, sizeof(srvadr))) {
    perror("bind failed");
    consecutive_errors++;
  }
  if (::listen(sockfd, 0)) {
    perror("listen failed");
    consecutive_errors++;
  }
  if (fcntl(sockfd,F_SETFL,fcntl(sockfd,F_GETFL) | O_NONBLOCK)) {
    perror("fcntl failed");
    consecutive_errors++;
  }
}

Neurosock::~Neurosock() {
  closeSocket();
}

void Neurosock::sendHeader() {
  NS_Header header;
  srv->fillInfo(header.info);
  fb->fillInfo(chn_setting,header.info);
  header.info.reason = NS_Info::HEADER;

  char *ptr=(char*)&header;
  int rem=sizeof(header);
  while (rem) {
    int n = write(confd,ptr,rem);
    if (n<0) {
      perror("sendHeader: cannot write");
      throw 7;
    } else if (n==0) {
      perror("sendHeader: write returned zero??");
      throw 8;
    }
    rem-=n; ptr+=n;
    WARN_PARTIAL;
  }
}

void Neurosock::sendFrame(char *ptr) {
  int rem=srv->frameSize_rawt()*sizeof(raw_t);
  while (rem) {
    int n = ::write(confd,ptr,rem);
    if (n<0) {
      perror("sendFrame: cannot write");
      throw 9;
    } else if (n==0) {
      perror("sendFrame: write returned zero??");
      throw 10;
    }
    rem-=n; ptr+=n;
    WARN_PARTIAL;
  }
}

void Neurosock::sendInfoFrame(NS_Info::NS_Reason reason) {
  raw_t frame[srv->frameSize_rawt()];
  raw_t *first = (raw_t*)frame;
  first[0] = NEUROSOCK_FRAME_IS_INFO; // impossible value in first scan

  NS_Info info;
  srv->fillInfo(info);
  fb->fillInfo(chn_setting,info);
  info.is_running = fb->isRunning();
  info.reason = reason;
  
  NS_Info *infop = (NS_Info*)(first+info.samples_per_scan);
  *infop = info; // NS_Info in second scan and beyond

  // All other scans are junk. No need to zero them, I suppose.

  sendFrame((char*)frame);
}
  
void Neurosock::sendDataFrame() {
  // map using channelMap, then send
  //  fprintf(stderr,"[dbg] Neurosock(%i)::sendDataFrame\n",chn_setting);
  sendFrame((char*)(fb->read(chn_setting)));
}

void Neurosock::dropFrame() {
  fb->read(chn_setting);
}

void Neurosock::processCommand() {
  NS_Command cmd;
  char *ptr = (char*)&cmd;
  int rem=sizeof(cmd);
  while (rem) {
    int n = read(confd, ptr, rem);
    if (n<0) {
      perror("processCommand: cannot read");
      throw 5;
    } else if (n==0) {
      fprintf(stderr,"Client lost. Bye bye.\n");
      throw 6;
    }
    rem-=n;
    WARN_PARTIAL;
  }
  //  fprintf(stderr,"[dbg] Command is %i\n",int(cmd.cmd));
  union {
      unsigned int a[2];
      long b;
    } u;
 
  switch (cmd.cmd) {
  case NS_Command::START:
    fprintf(stderr,"START\n");
    // This may not be a big deal, if the hardware is already running
    if (!fb->isRunning()) {
      fprintf(stderr,"Starting hardware...\n");
      fb->start();
    }
    fb->resetsoftover(chn_setting);
    if (!isActive)
      nActive++;
    isActive = true; //isActive turns sendDataFrame on and off
    fprintf(stderr,"Running...\n");
    break;
  case NS_Command::STOP:
    fprintf(stderr,"STOP\n");
    if (isActive)
      nActive--;
    isActive=false; // This will leave the connection open, but cause the socket to start dropping frames
    fprintf(stderr,"Socket stopped. Not stopping hardware.\n");
    sendInfoFrame(NS_Info::STOP_RESPONSE);
    break;
  case NS_Command::SETFREQ:
    if (fb->isRunning()) {
      if (nActive>0) 
	fprintf(stderr,"Cannot stop hardware for SETFREQ\n");
      else {
	fprintf(stderr,"Stopping hardware...\n");
	fb->stop();
      }
    }
    srv->setFreq(cmd.arg);
    break;
  case NS_Command::SETGAIN:
    if (cmd.arg!=srv->getGain()) {
      if (fb->isRunning()) {
	if (nActive>0) 
	  fprintf(stderr,"Cannot stop hardware for SETGAIN\n");
	else {
	  fprintf(stderr,"Stopping hardware...\n");
	  fb->stop();
	}
      }
      srv->setGain(cmd.arg);
    }
    break;
  case NS_Command::GETINFO:
    sendInfoFrame(NS_Info::INFO_RESPONSE);
    break;
  case NS_Command::EXCLUDECHANNELS:
    // This is PDMeata stuff, likely disfunctional
    u.a[0] = cmd.arg; u.a[1] = cmd.arg1;
    setExclude(u.b);
    srv->setChannelList(excludeChannels, doubleChannels);
    break;
  case NS_Command::DOUBLECHANNELS:
    // This is PDMeata stuff, likely disfunctional
    u.a[0] = cmd.arg; u.a[1] = cmd.arg1;
    setDouble(u.b);
    srv->setChannelList(excludeChannels, doubleChannels);
    break;
  default:
    fprintf(stderr,"Unknown command %i received.\n",int(cmd.cmd));
    throw 6;
  }
}

int Neurosock::connect() {
  struct sockaddr_in conadr;
  socklen_t conlen = sizeof(conadr);
  confd = ::accept(sockfd, (struct sockaddr*)&conadr, &conlen); //confd = Connected File Descriptor
  if (confd<0) {
    if (errno==EAGAIN) {
      // nothing to do
      if (fb->isRunning()) {
        dropFrame();
      } else
	usleep(10*1000);
    } else {
      perror("accept failed");
      if (++consecutive_errors >= 10) {
        fprintf(stderr,"Ten consecutive connections failed. Giving up.\n");
	exit(2);
      }
    }
    return confd;
  };

  consecutive_errors = 0;
    
  fprintf(stderr,"Connected. fd=%i [len: %i [sizeof=%i]]\n",
	  confd,conlen,int(sizeof(conadr)));

  fprintf(stderr,"Connected. Address: %s:%i\n",
    inet_ntoa(conadr.sin_addr), conadr.sin_port);
  if (!fb->isRunning()) {
    fprintf(stderr,"[dbg] Going to reset\n");
    srv->reset();
    fprintf(stderr,"[dbg] Reset OK\n");
  }
  sendHeader();
  fprintf(stderr,"[dbg] Sent header OK\n");
    
  pollfd.fd = confd;
  pollfd.events = POLLIN | POLLOUT;

  return confd;
};


void Neurosock::closeSocket() {
  fprintf(stderr,"Closing connection\n");
  if (confd>=0)
    if (close(confd))  
      perror("close failed");
  if (isActive)
    nActive--;
  isActive = false;
  confd = -1;
  fprintf(stderr,"\nNeurosock is now accepting connections on port 0x%x...\n",port);
};

void Neurosock::Poll() {
  if (confd<0) {
    confd=connect();
    return;
  }
  
  try {
    int res = poll(&pollfd, 1, -1);
    if (res<0) {
      perror("poll failed");
      throw 1;
    } else if (res==0) {
      fprintf(stderr,"Poll mysteriously returned zero");
      throw 2;
    }
    // So, either we can read, or write, or both
    if (pollfd.revents & POLLIN) {
      fprintf(stderr,"[dbg] Going to process command\n");
      processCommand();
    } else if (pollfd.revents & POLLOUT) {
      // fprintf(stderr,"[dbg] Going to send data? %c\n",hwRunning?'y':'n');
      if (isActive) 
        sendDataFrame();
      else if (fb->isRunning()) 
	dropFrame();
    } else if (pollfd.revents & (POLLERR|POLLHUP|POLLNVAL)) {
      fprintf(stderr,"Poll reported trouble on port 0x%x: 0x%x\n",port,pollfd.revents);
      throw 3;
    }
  } catch (Error const &e) {
    e.report();
    closeSocket();
  } catch (...) {
    closeSocket();
  };
};
