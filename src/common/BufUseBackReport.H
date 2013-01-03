// BufUseBackReport.H

#ifndef BUFUSEBACKREPORT_H

#define BUFUSEBACKREPORT_H

#include <common/MEAB.H>
#include <base/Timeref.H>
#include <common/Config.H>

class BufUseBackReport {
public:
  static const int BUFUSEIVAL = FREQKHZ * 1000 / 2;
public:
  BufUseBackReport() {
    MEAB::rawin->sleeper.report_yesbufuse();
  }
  void enable(timeref_t starttime=0) {

    if (MEAB::varspeed) {
      MEAB::rawout->waker.send(Wakeup::CanSlow);
      fprintf(stderr,"Automatic source slow-down supported\n");
    
      if (starttime) {
	MEAB::rawin->sleeper.report_bufuse(starttime);
	nextbufuse = starttime + BUFUSEIVAL;
      } else {
	nextbufuse=0;
      }
    } else {
      MEAB::rawin->sleeper.report_nobufuse();
    }
  }
  inline void ivalreport(timeref_t now) {
    sdbx("ivalreport [%c] now=%Li next=%Li",MEAB::varspeed?'y':'n',
	 now,nextbufuse);
    if (MEAB::varspeed && now>=nextbufuse) {
      MEAB::rawin->sleeper.report_bufuse(now);
      nextbufuse = now + BUFUSEIVAL;
    }
  }
  inline void nowreport(timeref_t now) {
    if (MEAB::varspeed)
      MEAB::rawin->sleeper.report_bufuse(now);
  }
private:
  int i;
  timeref_t nextbufuse;
};


#endif
