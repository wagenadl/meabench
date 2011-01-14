// srvMCMeata.C

#include "srvMCMeata.H"

#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ioctl.h>


static float AUX_RANGE_MV[] = { 4092, 1446, 819.6, 409.2 };

srvMCMeata::srvMCMeata(bool use128) throw(Error) {
  frame_size_rawt = MCC_FILLBYTES/sizeof(raw_t);
  buf = new raw_t[frame_size_rawt];
  
  mcc_fd = open("/dev/MCCard",O_RDONLY);
  if (mcc_fd<0)
    throw SysErr("neurosock/mcmeata: Cannot open device");

  setCHN(use128 ? MC128 : MC64);
  setGain(3);
  stop();
  isrunning = false;
  i_run_start = i_now = 0;
  i_firstsoftoverrun=i_lastsoftoverrun=0;
  n_softoverruns=0;
}

srvMCMeata::~srvMCMeata() {
  stop();
  close(mcc_fd);
  delete [] buf;		  
}

void srvMCMeata::start() {
  //  fprintf(stderr,"[dbg] Trying to start\n");
  if (ioctl(mcc_fd, MCCARD_IOCSTART_SAMPLING)<0)
    throw SysErr("MCMeata","Cannot start sampling");
  isrunning = true;
  i_run_start = i_now;
  //  fprintf(stderr,"[dbg] Running...\n");
}

void srvMCMeata::stop() {
  ioctl(mcc_fd, MCCARD_IOCSTOP);
  isrunning = false;
  //  fprintf(stderr,"[dbg] Stopped.\n");
}

void srvMCMeata::setGain(int n) {
  gain=n;
  if (ioctl(mcc_fd, MCCARD_IOCGAIN,&gain)<0)
    throw SysErr("MCMeata","Cannot set gain.");
}

void srvMCMeata::setCHN(int n) {
  chn = n;
  if (ioctl(mcc_fd, MCCARD_IOSETCHANNELS,&chn)<0)
    throw SysErr("MCMeata","Cannot set CHN. Invalid CHN index or not enough memory for DMA Buffers.");
}

void srvMCMeata::fillInfo(NS_Info &i) {
  fprintf(stderr,"[dbg] srvMCMeata::fillInfo\n");
  struct MCCard_stats stats;
  int r = ioctl(mcc_fd,MCCARD_IOQSTATS,&stats);
  if (r)
    throw SysErr("MCMeata","Cannot read MCCard driver stats");
  fprintf(stderr,"[dbg] Read card stats OK\n");

  i.bytes_per_sample = sizeof(raw_t);
  i.samples_per_scan = TOTALCHANS;
  i.bytes_per_frame = MCC_FILLBYTES;
  i.scans_per_frame = i.bytes_per_frame / (i.bytes_per_sample * i.samples_per_scan);
  i.scans_per_second = MCC_FREQHZ;
  i.gain_setting = gain;
  i.mV_per_digi_aux = AUX_RANGE_MV[i.gain_setting] / 2048;
  i.uV_per_digi_elc = i.mV_per_digi_aux / 1.2;
  i.num_electrodes = 60;
  i.num_aux = 4;
  i.digital_half_range = 2048;
  i.digital_zero = 2048;
  i.frames_transmitted = i_now - i_run_start;
  i.run_start_frame = i_run_start;
  i.is_running = isrunning;
  i.neurosock_overruns = n_softoverruns;
  i.meata_overruns = stats.overruns;
  i.other_errors = stats.Hardware_errors;
  i.total_errors = i.neurosock_overruns + i.meata_overruns + i.other_errors;
  i.last_error_frame = i_lastsoftoverrun;
  i.card_setting = chn;
  fprintf(stderr,"gain: %i. nframes: %i. isrunning: %c\n",
	  gain,
	  i.frames_transmitted,
	  i.is_running ? 'y': 'n');
  fprintf(stderr,"softover: %i. hardover: %i. HW error: %i. [last: %i]\n",
	  i.neurosock_overruns,
	  i.meata_overruns,
	  i.other_errors,
	  i.last_error_frame);
}

raw_t *srvMCMeata::nextFramePlease(raw_t *dst) {
  if (dst==0)
    dst=buf;

  int readSize = chn==MC128 ? 2*MCC_FILLBYTES : MCC_FILLBYTES;

  int len = ::read(mcc_fd, dst, readSize);
  if (len<0) 
    throw SysErr("MCMeata","Read failed");
  else if (len!=readSize)
    throw Error("MCMeata","Got less than expected");

  i_now ++;
  return buf;
}

