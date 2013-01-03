// Storage.C

#include "Storage.H"

#include <common/Types.H>
#include <algorithm>

Storage::Storage() {
  capacity=MAXSPIKES;
  spikes = new Spike[capacity];
  trials = new Trial[MAXTRIALS];
  trigch = 60;
}

void Storage::extend() {
  unsigned int capacity0 = 2*capacity;
  Spike *newpts = new Spike[capacity0];
  for (int i=0; i<nspk; i++)
    newpts[i] = spikes[i];
  delete [] spikes;
  spikes = newpts;
  capacity=capacity0;
}

Storage::~Storage() {
  delete [] spikes;
  delete [] trials;
}

void Storage::reset() {
  nspk = 0; ntri = 0; nsafe=0;
  lasttime=0;
  faking=true;
  lasttri=Trial();
  trials[0] = lasttri; ntri++;
}

void Storage::add(Spikeinfo const *src, unsigned int n, bool updat) {
  if (nspk==0 && n>0) { // hack first trial time to match first spike
    long long t = FREQKHZ*1000*int(src[0].time/(FREQKHZ*1000));
    lasttri.time=t;
    trials[0].time=t;
  }
  while (n>0) {
    if (nspk>=capacity)
      extend();
    Spikeinfo const &si = *src++;
    if (si.channel==trigch) {
      identitrig(si);
      if (ntri<MAXTRIALS)
	trials[ntri++] = lasttri;
      faking=false;
    } else if (si.channel<NCHANS) {
      int latency = si.time - lasttri.time;
      if ((faking && latency>DTFAKE_SAMS) || latency>MINFAKE_SAMS) {
	lasttri = Trial(lasttri.time + DTFAKE_SAMS*int(latency/DTFAKE_SAMS));
	if (ntri<MAXTRIALS)
	  trials[ntri++]=lasttri;
	faking=true;
	latency = si.time - lasttri.time;
      }
      unsigned int mytri=ntri-1;
      while (mytri>0 && latency<0) {
	--mytri;
	latency = si.time - trials[mytri].time;
      }
      spikes[nspk++] = Spike(mytri, latency, si.channel, short(100*si.height/(10*si.threshold+1)));
    }
    --n;
  }
  while (nsafe<nspk) {
    short lat = spikes[nsafe].latency;
    long long int time = trials[spikes[nsafe].trial].time + lat;
    if (time > lasttri.time+MINPOST_SAMS)
      break;
    if (lat>MINPOST_SAMS && time > lasttri.time-MAXPRE_SAMS) {
      // This fails if there are intermediate trials, but I don't think
      // that that is possible (3/23/04). Anyway, it fails mildly.
      spikes[nsafe].trial = ntri-1;
      spikes[nsafe].latency = time - lasttri.time;
    }
    nsafe++;
  }

  if (nspk>0)
    lasttime = trials[spikes[nspk-1].trial].time + spikes[nspk-1].latency;
  if (updat) 
    emit update();
}

#include "identitrig.H"

void Storage::identitrig(Spikeinfo const &si) {
  int hw = ::identitrig(si);
  if (hw<0 || hw>NCHANS)
    hw=NCHANS; // if cannot determine channel#, just put in a generic marker.
  // (Can't use -1, 'coz that's for fake stim (time caused).)
  lasttri.channel = hw;
  lasttri.time = si.time;
}
  
void Storage::dump() {
  fprintf(stderr,"Storage::Dump\n");
  fprintf(stderr,"Number of trials: %i\n",ntri);
  for (int i=0; i<ntri; i++) 
    fprintf(stderr,"  Trial #%4i: %8.3fs / ch=%2i\n",
	    i,trials[i].time/25000.,trials[i].channel);
  fprintf(stderr,"Number of spikes: %i [%i]\n",nspk,nsafe);
  int lasttri=-2; int n=0;
  for (int i=0; i<nspk; i++) {
    if (spikes[i].trial==lasttri && n>=3)
      continue;
    fprintf(stderr,"  Spike #%5i: tri=%5i; lat=%5.1fms; ch=%2i\n",
	    i,spikes[i].trial,spikes[i].latency/25.0,spikes[i].channel);

    if (lasttri==spikes[i].trial)
      n++;
    else
      n=0;
    lasttri = spikes[i].trial;
  }
}

unsigned int Storage::firstspikeintrial(unsigned int trial) const {
  Spike s(trial,-32767);
  Spike const *it = lower_bound(spikes,spikes+nsafe,s);
  return it - spikes;
}

unsigned int Storage::firsttrialafter(long long int time) const {
  Trial t(time);
  Trial const *it = lower_bound(trials,trials+ntri,t);
  return it - trials;
}
