// FrameBuffer.C

#include "FrameBuffer.H"
#include <unistd.h>

FrameBuffer::FrameBuffer(int logn, srvMeata *src0) {
  src=src0;
  inidx=outidx[0]=outidx[1]=0;
  hasthread = false;
  mask=(1<<logn)-1;
  maxahead=mask;

  for (int m=0; m<src->getNumMEAs(); m++) {
    frames[m] = new raw_t* [1<<logn];
    for (int n=0; n<=mask; n++)
      frames[m][n] = new raw_t[src->frameSize_rawt()];
  }

  buffer = new raw_t[src->frameSize_rawt()*2];

  n_softover[0] = n_softover[1]=0;
  i_lastsoftover[0] = i_lastsoftover[1]=-1;
  hasthread = false;
}

FrameBuffer::~FrameBuffer() {
  try {
    stop();
  } catch (Error const &e) {
    e.report();
  }
}

void FrameBuffer::stop() throw(Error) {
  if (hasthread) {
    terminate=true;    
    void *retval;
    int r=pthread_join(thread,&retval);
    if (r)
      throw SysErr("FrameBuffer","Cannot terminate thread");
    hasthread=false;
    if (retval)
      throw SysErr("FrameBuffer","Thread exited badly");
  }
}

static void *start_routine(void *me) {
  FrameBuffer *realme = (FrameBuffer*)me;
  try {
    realme->threadcode();
  } catch (Error const &e) {
    e.report();
    return me;
  }
  return 0;
}

void FrameBuffer::start() throw(Error) {
  if (!hasthread) {
    terminate=false;
    int r=pthread_create(&thread,0, &start_routine, (void*)this);
    if (r)
      throw SysErr("FrameBuffer","Cannot create thread");
    hasthread=true;
  }
}

raw_t *FrameBuffer::read(int iMEA) throw(Error) {
  //  fprintf(stderr,"[dbg] Framebuffer::read(%i) ...\n",chn_setting);
  if (!hasthread || (iMEA>=src->getNumMEAs())) {
    // fprintf(stderr,"[dbg]   Framebuffer::read -> 0\n");
    return 0;
  }
  while (1) {
    int inidx_, outidx_;
    { MLock ml(lock);
      inidx_ = inidx;
      outidx_ = outidx[iMEA];
    }
    if (inidx_ != outidx_)
      break;
    //    fprintf(stderr,"\n[dbg]   Framebuffer::read waiting\n");
    cond.wait();
    //    fprintf(stderr,"\n[dbg]   Framebuffer::read done waiting\n");
  }

  raw_t *retval;
  { MLock ml(lock);
    retval = frames[iMEA][outidx[iMEA] & mask];
    outidx[iMEA]++;
  }
  //  fprintf(stderr,"\n[dbg]   Framebuffer::read -> %p\n",retval);
  return retval;
}    

void FrameBuffer::fillInfo(int k, NS_Info &info) {
  if (k<0 || k>1)
    throw Error("FrameBuffer","resetsoftover(0/1)");

  MLock ml(lock);
  info.neurosock_overruns = n_softover[k];
  if (i_lastsoftover[k] > info.last_error_frame)
    info.last_error_frame = i_lastsoftover[k];
  info.total_errors = info.neurosock_overruns + info.meata_overruns
    + info.other_errors;
}

void FrameBuffer::resetsoftover(int k) {
  if (k<0 || k>1)
    throw Error("FrameBuffer","resetsoftover(0/1)");

  MLock ml(lock);
  n_softover[k]=0;
  i_lastsoftover[k]=-1;
} 

void FrameBuffer::threadcode() throw(Error) {
  { MLock ml(lock);
    n_softover[0] = n_softover[1]=0;
    i_lastsoftover[0] = i_lastsoftover[1]=-1;
  }

  fprintf(stderr,"[dbg] Framebuffer: starting\n");
  src->start();

  while (!terminate) {
    // Drop frames on slow clients
    int nusers = src->getNumMEAs();
    for (int k=0; k<nusers; k++) {
      MLock ml(lock);
      if (inidx > outidx[k]+maxahead) {
	int drop = inidx-(outidx[k]+maxahead);
	n_softover[k] += drop;
	i_lastsoftover[k] = outidx[k];
	outidx[k] += drop;
      }
    }

    // Let's read new frame!
    if (src->getNumMEAs()==2) {
      //Bifurcate the data stream
      src->nextFramePlease(buffer);
      for (int seg=0; seg<256; seg++) {
        for (int chn=0; chn<64; chn++) {
          frames[0][inidx & mask][64*seg+chn] = buffer[128*seg+chn];
          frames[1][inidx & mask][64*seg+chn] = buffer[128*seg+chn+64];
	}
      }
    } else {
      src->nextFramePlease(frames[0][inidx & mask]);
    }

    { MLock ml(lock);
      inidx++;
    }

    cond.signal();
  }
  
  src->stop();
  fprintf(stderr,"[dbg] Framebuffer: stopping\n");
}
