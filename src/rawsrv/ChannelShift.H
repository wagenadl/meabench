// ChannelShift.H

#ifndef CHANNELSHIFT_H

#define CHANNELSHIFT_H

#include <common/Types.H>

class ChannelShift {
  /*:C ChannelShift
   *:D This class is a utility to detect channel shifts that seem to be
       happening more frequently these days (9/11/09). It works by tracking
       one channel, usually A2 (#61) which is permanently held at rail or
       contains a rail-to-rail 60 Hz signal.
  */
public:
  ChannelShift(int channel=NCHANS+1, raw_t dzero=2048, raw_t thresh=256);
  /*:F constructor
   *:D Does not do anything except to optionally specify the channel to track
       and to fix digital zero and the acceptance threshold (see check()).
   */
  void setDZero(raw_t dzero);
  /*:F setDZero
   *:D Specifies the digital value corresponding to zero volts.
  */
  raw_t getDZero() const { return dzero; }
  /*:F getDzero
   *:D Returns the digital value corresponding to zero volts.
   */
  void setThresh(raw_t thresh);
  /*:F setThresh
   *:D Specifies the digital value corresponding to zero volts.
  */
  raw_t getThresh() const { return thresh; }
  /*:F getThresh
   *:D Returns the digital value corresponding to zero volts.
   */
  void setChannel(int channel);
  /*:F setChannel
   *:D Specifies which channel to track. Any channel can be used, but
       typically the auxiliary channels A1-A3 (#60-#62) are most useful.
  */
  int getChannel() const { return channel; }
  /*:F getChannel
   *:D Returns which channel is being used for tracking.
   */
  bool check(Sample const *data, int nSamples) const;
  /*:F check
   *:D Scans through a chunk of data and checks whether the tracked channel
       is still in the right spot.
   *:N This call is reasonably quick since the other channels are not
       looked at.
   *:N This simply checks that at least 90% of the data is above threshold.
   */
  int whereIs(Sample const *data, int nSamples) const;
  /*:F whereIs
   *:D If check() says that the tracked channel is not in its spot, use
       this function to find out where it is.
   *:R A non-negative return value is the channel number where the tracked
       channel was found.
       Negative return value indicates that the data were ambiguous. This
       happens, for instance, when several channels are pegged due to a
       stimulation artifact. In that case, simply try again with the next
       chunk of data.
  */
private:
  int channel;
  raw_t dzero;
  raw_t thresh;
};

#endif
