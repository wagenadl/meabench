/* spikesrv/SpikeDets.H: part of meabench, an MEA recording and analysis tool
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

// SpikeDets.H

#ifndef SPIKEDETS_H

#define SPIKEDETS_H

#include <spikesrv/Defs.H>

#include <string>
#include <base/Error.H>
#include <common/ChannelNrs.H>
#include <common/NoiseLevels.H>
#include <spikesrv/Exclude.H>

class SpkDet { //:C SpkDet
  /*:D SpkDet is a base class for spike detectors.
       A spike detector is a filter that reads raw data from a
       SloppyFifo<Sample> and writes information about detected spikes
       to a SloppyFifo<Spikeinfo>. The spike detector should not worry
       about collecting context. This will be done by the base class.
     . Constructors for SpkDet inheritants should take the form:
     .   SpkDet(SloppyFifo<Sample> &src, SloppyFifo<Spikeinfo> &dst),
     . and pass these parameters to the constructor for SpkDet.
     . NEW! As of 1/23/01 timestamps in Spikeinfo are relative to start
       of run of raw source.
  */
public:
  static const int A_MINSTD = 100;
  enum Type { None=0, SNEO=2, BDT=1, BandFlt=3, Source=4, Butter=5,
	      AdaFlt=6, AdaPerc=7, LimAda=8, End=9 };
  static string describe(Type s) {
    switch (s) {
    case None: return "None"; break;
    case SNEO: return "SNEO"; break;
    case BDT:  return "BDT"; break;
    case BandFlt: return "BandFlt-25"; break;
    case Butter: return "Butter"; break;	
    case Source: return "Source"; break;
    case AdaFlt: return "AdaFlt-40"; break;
    case AdaPerc: return "AdaPerc-98%-82ms-1s"; break;
    case LimAda: return "LimAda-70%-98%-20ms-1s-5.0"; break;
    default:   return "???"; break;
    }
  }
  virtual ~SpkDet() {}
  void setexclude(Exclude const &e) { exclude=e; } //:f setexclude
  /*:D setexclude(e) sets the list of electrode channels to be excluded
       from detection.
   *:A e: Exclude set
   *:R -
   *:N Spike detector "Source" does not support this.
   */
  static SpkDet *construct(RawSFCli const &src, //:+
			   SpikeSFSrv &dst, //:+
			   SpkDet::Type type) ; //:f construct
  /*:D Constructs a spike detector according to type.
   *:A src, dst: as specified in the class description
       type: spike detector type
   *:R A pointer to the newly constructed detector, or zero for type=0.
   *:N The caller is responsible for eventually destructing the detector.
   *:E Exception is thrown for Source, which must be constructed through
       DataSrc::srclevelspikedet.
     . Exception is also thrown for BDT (not implemented).
   */
  static timeref_t addcontext(SpikeSFSrv &spksf, timeref_t first, //:+
			      RawSFCli const &src); //:f addcontext
  /*:D end=addcontext(spksf, start, src) adds context to the spikes in
       the sloppy fifo spksf. This is presented here as a static function
       to stress the independence of any actual spike detector. It is
       necessary to keep track of the last contextified spike externally.
   *:A spksf: spike sloppyfifo on which to add the context.
     . first: offset on that fifo specifying where to start contextification.
     . src: raw data sloppyfifo from which to obtain the context.
   *:R offset on the spike sloppyfifo of next spike to be contextified. This
       is typically almost spksf.latest(), but the most recent few spikes
       may not yet be contextifiable simply because the relevant data isn't
       available on the src stream yet.
   *:N It doesn't hurt to contextify a spike twice, but it is a waste of
       time.
     . This scheme of adding context implies that spike infos are available
       on the sloppy fifo before they have their context field filled.
       Clients of the shared memory fifo need to be aware of that.
  */
public:
  SpkDet(RawSFCli const &src0, SpikeSFSrv &dst0); //:c
    /*:D SpkDet(src,dst) constructs the base class for the spike detector.
         It doesn't do anything except setting up the source and destination
	 references.
     *:A src: reference to source
       . dst: reference to destination.
    */
  virtual void setthresh(float thresh0) { //:f setthresh
    thresh=thresh0; thresh2=thresh*thresh; }
  /*:D setthresh(thresh) sets the threshold for spike detection. Current
       implementation interpret this number in terms of the noise level
       of their preprocessed raw data stream.
   *:A thresh0: new threshold
   *:R -
   *:N Detectors can choose how to interpret the variables "thresh" and
       "thresh2" (see below). Additionally, they may re-implemted this fn.
   */
  virtual void detectrows(timeref_t /*start*/, timeref_t /*end*/) {}
  //:f detectrows
  /*:D detectrows(start, end) should make the detector examine the
       data from start to end, looking for spikes. Some detectors may use a
       built-in delay, in which case they'll read from (start-delay) to
       (end-delay). This is OK, as long as start>=delay. The result should
       be written to the output SloppyFifo<Spikeinfo>.
   *:A start, end: start and end times of current data block (references into
       a SloppyFifo<Sample> that should be presented to the constructor).
   *:R -
   *:N This method MUST be implemented
   */
  virtual void detectrowsshow(timeref_t start, timeref_t end, //:+
			      RawSFSrv *rawdest); //:f detectrowsshow
  /*:D detectrowsshow(start, end, rawdest) is as detectrows, except that it
       is expected to dump filtered raw data into rawdest.
   *:A start, end: start and end times of current data block (references into
       a SloppyFifo<Sample> that should be presented to the constructor).
     . rawdest: drain for post filter raw data.
   *:R -
   *:N This method may be left unimplemented
   */
  virtual void skiprows(timeref_t /*start*/, timeref_t /*end*/) {} //:f skiprows
  /*:D skiprows(start, end) should make the detector examine the
       data from start to end. Spikes found in the data should be ignored
       though. This function serves to keep filters &c up-to-date during
       periods outside of trigger windows.
   *:A start, end: start and end times of current data block (references into
       a SloppyFifo<Sample> that should be presented to the constructor).
   *:R -
   *:N Doesn't have to be implemented. Should not call detect_analog.
   */
  virtual void reset(); //:f reset
  /*:D This call should reset the spike detector to "untrained" state.
   *:A -
   *:R -
   *:N This method needn't be implemented if the detector requires no
       training. Implementors should call the base class' method.
   */
  virtual bool train(timeref_t /*start*/, timeref_t /*end*/) { //:f train
    /*:D This call should use the data in [start,end) to (partially) train
         the spike detector. When training is completed to satisfaction,
	 return true.
     *:A start, end: start and end times of current data block (references into
         a SloppyFifo<Sample> that should be presented to the constructor).
     *:R True iff training is complete.
     *:N This method needn't be implemented if the detector requires no
         training.
     */
    return true; }
  virtual void setnoise(NoiseLevels const &nl); //:f setnoise
  /*:D Should set the noise levels from nl.
   *:A nl: NoiseLevels to use.
   *:R -
   *:N Implementation *must* call its parent to set analog noise
   */
  virtual NoiseLevels getnoise(); //:f getnoise
  /*:D Should return the noise levels currently used by the spike detector.
   *:A -
   *:R NoiseLevels. If no noise levels are known (detector not trained), a
       non-ready NoiseLevels may be returned.
   *:N The dummy implementation always returns a non-ready NoiseLevels, with
       analog channel noise levels filled in if available.
     . Implementation should call its parent to deal with
       the analog channels.
   */
  virtual void setclean(bool c) { clean=c; } //:f setclean
  /*:D Set the "clean" flag true or false. Implementations can do some
       initialization at this stage, if desired.
   *:A c: new "clean" flag value
   *:R -
   *:N Implementation should call its parent. Most detectors don't need to
       implement this.
  */
  
  //////////////////////////////////////////////////////////////////////
  // Functions implemented for real here
  inline void detect_analog(timeref_t t, Sample const &s) {
    for (int i=0; i<ANALOG_N; i++)
      if (s[ANALOG_BASE + i] < alothresh[i])
	analog_ready[i] = true;
      else if (analog_ready[i] && s[ANALOG_BASE+i]>ahithresh[i]) {
	timeref_t t0 = src.first();
	Spikeinfo &si(dst[dst.latest()]);
	si.channel = ANALOG_BASE + i;
	si.time = t-t0;
	si.height = 0; si.width = 1;
	dst.donewriting(1);
	analog_ready[i] = false;
      }
  }
  virtual void startrun(); //:f startrun
  /*:D This call should prepare the spike detector for a new run, e.g.
       resetting filters to a known state.
   *:A -
   *:R -
   *:N This method needn't be implemented if the detector requires no
       preparation. Implementors should call the base class' method.
   */
  virtual timeref_t safetycushion() { return 0; } //:f safetycushion
  /*:D This call should return the number of samples the spike detector
       wants to have ahead of it in the source, beyond the "end" passed
       to detectrows.
   *:A -
   *:R duration of safety margin, in samples.
   *:N This method does not need to be reimplemented. Default is zero.
   */
  virtual bool canclean() { return 0; } //:f canclean
  /*:D This call should return true iff the detector can honor the "clean"
       flag.
   *:A -
   *:R True iff "clean" flag supported
   *:N This method does not need to be reimplemented. Default is false.
   */
  void train_analog(Sample const &s);
  void post_train_analog();
  virtual float reportthreshold(int /*c*/) const { return 0; }
  virtual char const *thresholdunit() const { return "-"; }
  void dump_training_info();
  void post_training_info();
  void analog_disable(int ch) {
    ch-=NCHANS;
    if (ch<0||ch>=ANALOG_N)
      return;
    alothresh[ch] = 10000;
    analog_ready[ch] = false;
    ahithresh[ch] = 20000;
  }
private:
  bool analog_ready[ANALOG_N];
  double asx[ANALOG_N], asxx[ANALOG_N];
  int an[ANALOG_N];
  raw_t alothresh[ANALOG_N], ahithresh[ANALOG_N];
protected:
  float thresh; //:d thresh
  /*:D Threshold value for spike detection. Implementations can interpret this
       in any appropriate way.
  */
  float thresh2; //:d thresh2
  /*:D Provided for convenience, this variable stores the square of thresh.
   */
  Exclude exclude; //:d exclude
  /*:D List of excluded electrode channels. */
  RawSFCli const &src; //:d src
  SpikeSFSrv &dst; //:d dst
  bool clean; //:d clean
  /*:D True if user wants "cleaned" spikes, whatever that means for current
       detector.
  */
};
  

#endif
