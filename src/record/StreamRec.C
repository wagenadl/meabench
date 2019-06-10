/* record/StreamRec.C: part of meabench, an MEA recording and analysis tool
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

// StreamRec.C

#include "StreamRec.H"

#include <record/Recorder.H>
#include <record/SpikeRecorder.H>
#include <base/SFCVoid.H>
#include <base/WakeupCli.H>
#include <base/Sigint.H>
#include <record/Descriptions.H>
#include <common/AutoType.H>
#include <common/ChannelNrs.H>
#include <common/Types.H>
#include <common/Config.H>
#include <common/CommonPath.H>
#include <rms/Defs.H>

#include <time.h>
#include <pwd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

const int RAWBUFUSEIVAL = FREQKHZ * 1000 / 2;
const int SPKBUFUSEIVAL = 1024;

//////////////////////////////////////////////////////////////////////

class Describe: public WriteDescription {
public:
  Describe(string const &fn): WriteDescription(fn+".desc",22), datafn(fn) { }
  void describe_trig(int pretrig, int posttrig) {
    print(dTRIGWIN,Sprintf("%i samples (%i ms)",
			   pretrig+posttrig,
			   (pretrig+posttrig)/FREQKHZ));
    print(dPRETRIG,Sprintf("%i samples (%i ms)",
			   pretrig,
			   pretrig/FREQKHZ));
    print(dPOSTTRIG,Sprintf("%i samples (%i ms)",
			   posttrig,
			   posttrig/FREQKHZ));
  }
  void describe_base(SFAux const *aux) {
    if (!aux)
      throw Error("Describe","Not an SFAux based source");
    if (aux->trig.on) {
      print(dNTRIG, aux->trig.n_latest);
    }
    if (aux->hwstat.on) {
      print("WARNING",Sprintf("There were hardware problems"));
      print(dHWERR,aux->hwstat.errors);
      print(dOVERRUN,aux->hwstat.overruns);
    } else {
      print(dHARDWARE,"No hardware problems detected");
    }
    print(dDIGIZERO,aux->sourceinfo.digizero);
    print(dDIGIMIN,aux->sourceinfo.digimin);
    print(dDIGIMAX,aux->sourceinfo.digimax);
    print(dDIGIRANGE,aux->sourceinfo.nominalrange);
    print(dRAWSAMFREQ,Sprintf("%g kHz",aux->sourceinfo.freqhz/1000.));
    print(dGAIN,Sprintf("%g uV/digistep [+- %g uV range]",
			aux->sourceinfo.uvperdigi,
			aux->sourceinfo.uvperdigi*
			aux->sourceinfo.nominalrange));
    print(dAUXGAIN,Sprintf("%g mV/digistep [+- %g uV range]",
	    aux->sourceinfo.aux_mvperdigi,
			   aux->sourceinfo.aux_mvperdigi*
			   aux->sourceinfo.nominalrange));
    if (aux->paraccumlen)
      print(dPARACCUM,aux->paraccum);
  }
  void describe_raw(RawSFCli const *src) {
    if (!src)
      throw Error("Describe","Not a raw source");
    describe_base(src->aux());
    print(dRUNLENGTH,
	  Sprintf("%.3f s",
		  float(src->latest()-src->first())/
		  src->aux()->sourceinfo.freqhz));
  }
  void describe_spike(SpikeSFCli const *src) {
    if (!src)
      throw Error("Describe","Not a spike source");
    describe_base(src->aux());
    print(dSPKDET,src->aux()->type);
    print(dTHRESH,"%.1f",src->aux()->thresh);
    print(dNSPIKES, src->latest()-src->first());
    print(dSPKUNIT, src->aux()->unit);
    for (int row=0; row<8; row++) {
      string s="";
      for (int col=0; col<8; col++) {
	if ((col==0||col==7)&&(row==0||row==7))
	  s += Sprintf("%6s ","");
	else
	  s += Sprintf("%6.2f ",src->aux()->threshs[cr2hw(col,row)]);
      }
      print(dSPKTHRESH, s);
    }
  }
  void describe_rms(RMSSFCli const *src) {
    if (!src)
      throw Error("Describe","Not an rms source");
    describe_base(src->aux());
    print(dRMSPERIOD,Sprintf("%i samples [%.3f seconds]",
			     src->aux()->output_period,
			     double(src->aux()->output_period) /
			     src->aux()->sourceinfo.freqhz));
  }
  void describe_generic(SFCVoid const *src, string const &comments,
			string const &type, string const &stream,
			string const &termination, bool trig,
			time_t starttime_s) {
    print(dDATAFN,datafn);
    print(dCREAPROG,"record (meabench)");
    struct passwd *pwd = getpwuid(getuid());
    if (pwd)
      print(dCREAUSER,Sprintf("%s (%s)",
	      pwd->pw_gecos,pwd->pw_name));
    else
      print(dCREAUSER,Sprintf("??? (%i)",getuid()));
    printnonl(dSTART,asctime(localtime(&starttime_s)));
    time_t now = time(0);
    printnonl(dDATE,asctime(localtime(&now)));
    printnonl(dEND,asctime(localtime(&now)));
    print(dCOMMENTS,comments);
    print(dDATATYP,type);
    print(dIMMSRC,stream);
    //    print(dRUNSTART,src->first());
    print(dBUFUSE,src->bufuse_deepreport());
    print(dTERMINATION,termination);
    print(dTRIGGERED,trig?dYES:dNO);
  }
private:
  string datafn;
};

//////////////////////////////////////////////////////////////////////

StreamRec::StreamRec(string const &stream, string const &type,
		     string const &basefn,
		     bool describe, bool usestream,
		     bool evenifexists)  {
  trig=false;
  construct(stream, type,basefn, describe, usestream, evenifexists);
}

StreamRec::StreamRec(string const &stream, string const &type,
		     int pretrig0, int posttrig0,
		     string const &basefn,
		     bool describe, bool usestream,
		     bool evenifexists)  {
  trig=true;
  construct(stream, type,basefn, describe, usestream, evenifexists);
  pretrig=pretrig0; posttrig=posttrig0;
}
  

void StreamRec::construct(string const &stream, string const &type,
			  string const &basefn,
			  bool describe0, bool usestream,
			  bool evenifexists)  {
  sleeper=0;
  source=0;
  recorder=0;
  trigfh=0;
  hasthread=false;
  describe=describe0;
  sourcename = stream;
  typenam = type;
  filename = basefn;
  if (usestream)
    filename+="."+stream;
  filename+="."+type;
  trigname=filename+".trig";

  if (!evenifexists)
    ensurenonexistent(filename);

  try {
    sleeper = new WakeupCli("record",
			    CommonPath(stream,WAKESUFFIX).c_str());
    sleeper->setival(RECIVAL);
    
    string strm = CommonPath(stream,SFSUFFIX);
    if (type == SPIKETYPE)
      source = new SpikeSFCli(strm.c_str());
    else if (type == RAWTYPE)
      source = new RawSFCli(strm.c_str());
    else if (type == RMSTYPE)
      source = new RMSSFCli(strm.c_str());
    else {
      source = new SFCVoid(strm.c_str());
      if (trig) 
	fprintf(stderr,"Warning: Cannot do triggered recording from %s\n",
		sourcename.c_str());
      trig=false;
    }
    
    if (type == SPIKETYPE)
      recorder = new SpikeRecorder(source,filename);
    else
      recorder = new Recorder(source,filename);
    if (trig) {
      trigfh=fopen(trigname.c_str(),"w");
      if (!trigfh)
	throw SysErr("StreamRec","Cannot write to trigger file");
    }
  } catch (...) {
    if (sleeper)
      delete sleeper;
    if (source)
      delete source;
    if (recorder)
      delete recorder;
    sleeper=0;
    source=0;
    recorder=0;
    throw;
  }
}

StreamRec::~StreamRec() {
  if (hasthread) 
    fprintf(stderr,"Warning: destructing StreamRec without terminating thread. This is a BUG.\n");
  if (sleeper)
    delete sleeper;
  if (source)
    delete source;
  if (recorder)
    delete recorder;
  if (trigfh)
    fclose(trigfh);
}

void StreamRec::run(int lim_s)  {
  if (hasthread)
    throw Error("StreamRec","Already running");

  limit_s = lim_s;

  if (pthread_attr_init(&attr))
    throw SysErr("StreamRec","Cannot create thread attributes");
  if (pthread_create(&thread, &attr, &StreamRec::thread_code, (void*)this))
    throw SysErr("StreamRec","Cannot create thread");
  hasthread=true;
}

StreamRec::TerminationCode StreamRec::wait()  {
  if (!hasthread)
    return NOTRUNNING;
  hasthread=false;
  if (pthread_join(thread,0)) {
    //    if (errno==ESRCH)
    //      return; // thread no longer exists
    throw SysErr("StreamRec","Thread can't be joined");
  }
  return termination_code;
}

void *StreamRec::thread_code(void *arg) {
  StreamRec *me = (StreamRec*)arg;
  try {
    me->exec();
  } catch (Error const &e) {
    e.report();
  }
  pthread_exit(0);
  return 0; // never executed - prevents compiler warning
}

void StreamRec::exec() {
  termination_code = INCOMPLETE;
  string termination = "Incomplete";

  source->bufuse_reset();
  sleeper->report_yesbufuse();
  bool canslow = false;
  fprintf(stderr,"Waiting for START from %s\n",sourcename.c_str());
  int b;
  while ((b=sleeper->block()) != Wakeup::Start) {
    if (b==Wakeup::CanSlow)
      canslow=true;
  }

  if (canslow) {
    sleeper->report_bufuse(source->first());
    fprintf(stderr,"Automatic source slow-down supported\n");
  } else {
    sleeper->report_nobufuse();
  }
  
  if (trig) {
    SFAux const *aux = (SFAux const *)(source->aux());
    if (!aux->trig.on) {
      fprintf(stderr,"Warning: Stream %s is untriggered. Recording continuously\n",sourcename.c_str());
      trig=false;
    }
  }

  starttime_s = time(0);
  //  nextbufusereport_s = starttime_s + BUFUSEREPORTIVAL_S;
  fprintf(stderr,"Recording from %s into %s with%s triggering\n",
	  sourcename.c_str(), filename.c_str(), trig?"":"out");

  if (trig) 
    recorder->set_bounds(0,0);
  else
    recorder->set_bounds(0,INFTY);
  recorder->skip_to(source->first());

  bool timeout=false;
  try {
    timeref_t lasttrig=0;
    timeref_t nextbufuse = source->first() + sleeper->getival()*2;
    time_t endtime_s = limit_s ? starttime_s + limit_s : starttime_s + 1000000;
    while (!Sigint::isset()) {
      if (time(0)>endtime_s) {
	timeout=true;
	break;
      }
      int res=sleeper->block();
      timeref_t oldest=recorder->save_some();
      if (oldest!=INFTY)
  	source->bufuse_update(oldest);
      if (trig) {
  	SFAux const *aux = (SFAux const *)(source->aux());
  	if (aux->trig.t_latest != lasttrig) {
  	  lasttrig = aux->trig.t_latest;
  	  recorder->set_bounds(lasttrig-pretrig,lasttrig+posttrig);
	  sdbx("Writing trigger info: %i %.5f (%p)\n",aux->trig.n_latest,
		  aux->trig.t_latest/(FREQKHZ*1000.0),trigfh);
	  fprintf(trigfh,"%i %.5f\n",aux->trig.n_latest,
		  aux->trig.t_latest/(FREQKHZ*1000.0));
  	}
      }
      if (res==Wakeup::Stop)
  	break;
      sdbx("canslow: %c oldest: %Li nextbufuse: %Li ival=%Li",
	   canslow?'y':'n',
	   oldest,
	   nextbufuse,
	   sleeper->getival());
      if (canslow && oldest>=nextbufuse) {
	sleeper->report_bufuse(oldest);
	nextbufuse=oldest + sleeper->getival()*2;
      }
//OLD/	    time_t now = time(0);
//OLD/	    if (now >= nextbufusereport_s) {
//OLD/	      nextbufusereport_s = now + BUFUSEREPORTIVAL_S;
//OLD/	      time_t sec = now - starttime_s;
//OLD/	      int min = sec/60;
//OLD/	      sec -= min*60;
//OLD/	      int hr = min/60;
//OLD/	      min -= hr*60;
//OLD/	      fprintf(stderr,"[%i:%02i:%02i] Intermediate buffer use for %s: %s\n",
//OLD/		      hr,min,int(sec),filename.c_str(),
//OLD/		      source->bufuse_deepreport().c_str());
//OLD/	    }
    }
  } catch (Intr const &i) {
    // save last bit upon ^C.
    timeref_t oldest=recorder->save_some();
    if (oldest!=INFTY)
      source->bufuse_update(oldest);
    throw;
  } catch (Error const &e) {
    e.report();
    termination = e.msg();
    termination_code = TRANSMISSIONERROR;
  }
  if (Sigint::isset()) {
    fprintf(stderr,"Recording from %s interrupted\n",sourcename.c_str());
    termination = "Interruption (no check for hardware problems done)";
    termination_code = INTERRUPT;
  } else if (timeout) {
    fprintf(stderr,"Recording from %s stopped by time limit\n",sourcename.c_str());
    if (termination_code == INCOMPLETE) {
      termination = "Time limit reached";
      termination_code = TIME_LIMIT;
    }
  } else {
    fprintf(stderr,"Recording from %s ended\n",sourcename.c_str());
    if (termination_code == INCOMPLETE) {
      termination = "End of source stream";
      termination_code = SOURCE_END;
    }
  }

  fprintf(stderr,"Buffer usage: %s\n",source->bufuse_deepreport().c_str());
  
  if (describe) {
    Describe d(filename);
    d.describe_generic(source, comments, typenam, sourcename,
		       termination, trig, starttime_s);

    sdbx("Source: %p",source);
    sdbx("As spike: %p",dynamic_cast<SpikeSFCli const *>(source));
    sdbx("As RMSCli: %p",dynamic_cast<RMSSFCli const *>(source));
    
    if (typenam==RAWTYPE)
      d.describe_raw(dynamic_cast<RawSFCli const *>(source));
    else if (typenam==SPIKETYPE)
      d.describe_spike(dynamic_cast<SpikeSFCli const *>(source));
    else if (typenam == RMSTYPE)
      d.describe_rms(dynamic_cast<RMSSFCli const *>(source));
    if (trig)
      d.describe_trig(pretrig,posttrig);
  }
}

void StreamRec::ensurenonexistent(string const &fn)  {
  struct stat s;
  int r=stat(fn.c_str(),&s);
  if (r<0)
    return; // non-exist, so good
  if (s.st_size==0)
    return;
  throw Expectable("StreamRec",Sprintf("File exists - won't record. Use `!rm %s' to delete it",fn.c_str()));
}
