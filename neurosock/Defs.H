#ifndef NEUROSOCK_DEFS_H
#define NEUROSOCK_DEFS_H

#include "Error.H"
#include "common/Config.H"
#include <sys/poll.h>
#include <string.h>

// Configuration variables
#define NEUROSOCK_FRAME_IS_INFO 0x4001
#define LOCAL_HOST "127.0.0.1"
#define NEUROSOCK_PORT0 0xeeee
#define NEUROSOCK_PORT1 0xeeef

/* Protocol:
   1) The client connects to the host.
      - Only one client is supported at any one time.
   2) The host transmits a struct NS_Header, which reports
      configuration information to the client.
   3) The host waits for commands.
   4) The client issues such commands.
   5) The host responds.
   6) After the client has sent a START command, the host will send
      DATA frames, until it receives a STOP command or something
      goes badly wrong.
   7) End of recording is marked by sending an INFO frame.
   8) This INFO frame contains a "reason" field that tells the client
      whether the frame was sent spontaneously (host side run
      termination), in response to a STOP command (client side run
      termination), or in response to a GETINFO command.
      This is important, because of a possible race condition: suppose
      the client sends a STOP command, and the host experiences an
      error condition before it has replied. It will terminate the run
      in response to that error condition, and send an INFO frame. A
      little while later, it receives the STOP command, and sends
      another INFO frame. Now the client, if unaware of this, would be
      expecting only one INFO frame. It would then leave the second
      INFO frame on the stream, to be read at the next run. At that
      point, the client would incorrectly conclude that that run ended
      immediately.
   9) The client closes the connection.
*/       

// Command packet
struct NS_Command {
  enum NS_Cmd {
    START,    /*:D START enables data acquisition. If the card was
                   already running, this command is silently ignored.
	       *:A -
	       *:R Data frames start being sent.
	       *:E If the data acquisition fails at any time, an INFO
	           frame is sent with an explanation of the problem.
	           The client should take such an INFO frame as an
	           end-of-run marker, and pretend that it has called
	           STOP.
	       */
    STOP,      /*:D STOP disables data acquisition. It will respond by
                    sending an INFO frame. Because of network delays,
                    the client should be prepared for a few more data
                    frames to come through after it has sent the STOP
                    command. The INFO frame sent by the server once it
                    has processed the STOP command is the signal that
                    data acquisition has really stopped.
		*:A -
		*:R A few more data frames may be sent, terminated by
		    an INFO frame.
	       */
    SETFREQ,   /*:D SETFREQ changes the sampling frequency.
		*:A sampling frequency, in Hertz.
		*:R -
		*:N This call cannot be honored (and will be silently
		    ignored) while data acquisition is in progress.
	       */
    SETGAIN,   /*:D SETGAIN changes the gain setting.
		*:A gain setting: 0, 1, 2, or 3.
		*:R -
		*:N This call cannot be honored (and will be silently
		    ignored) while data acquisition is in progress.
	       */
    GETINFO,    /*:D GETINFO reports the current config and status
 		    info.
	        *:A -
		*:R Responds by sending an INFO frame.
		*:N This call cannot be honored (and will be silently
		    ignored) while data acquisition is in progress.
	       */
    EXCLUDECHANNELS, /*:D EXCLUDECHANNELS marks a subset of channels for
		          exclusion from the recording. A zero value will
			  be returned instead of an electrode reading.
  		      *:A bitfield of channels, in MCS order.
		      *:R -
   		      *:N This call cannot be honored (and will be silently
		          ignored) while data acquisition is in progress.
		     */
    DOUBLECHANNELS,  /*:D DOUBLECHANNELS marks a subset of channels to
		          be sampled twice. This is useful if the first
			  sampling tends to be polluted by crosstalk from
			  a stimulation channel. Only the second reading
			  is written to the output.
  		      *:A bitfield of channels, in MCS order.
		      *:R -
   		      *:N This call cannot be honored (and will be silently
		          ignored) while data acquisition is in progress.
		     */
  } cmd;
  unsigned int arg;
  unsigned int arg1;
};

#define STRVERSION(v) #v

class NS_Info {
public:
  NS_Info() {
    // static information
    strncpy(progversion, STRVERSION(VERSION), 8);
    bytes_per_sample = 0;
    samples_per_scan = 0;
    scans_per_frame = 0;
    bytes_per_frame = 0;
    scans_per_second = 0;
    gain_setting = 0;
    uV_per_digi_elc = 0;
    mV_per_digi_aux = 0;
    num_electrodes = 0;
    num_aux = 0;
    digital_half_range = 0;
    digital_zero = 0;
    
    // dynamic information
    frames_transmitted = 0;
    is_running = 0;
    total_errors = 0;
    neurosock_overruns = 0;
    meata_overruns = 0;
    other_errors = 0;
    last_error_frame = 0;
    card_setting = 0;

    exclude_map = 0;
    double_map = 0;

    reason = HEADER;
  }
  
public:
  // static information
  char progversion[8];
  long long exclude_map;
  long long double_map;

  int bytes_per_sample;
  int samples_per_scan;
  int scans_per_frame;
  int bytes_per_frame;
  float scans_per_second;
  int gain_setting;
  float uV_per_digi_elc;
  float mV_per_digi_aux;
  int num_electrodes;
  int num_aux;
  int digital_half_range;
  int digital_zero;
  
  // dynamic information
  int frames_transmitted;
  bool is_running;
  int total_errors;
  int neurosock_overruns;
  int meata_overruns;
  int other_errors;
  int last_error_frame;
  int run_start_frame;
  int card_setting;

  enum NS_Reason { HEADER, HOST_INITIATED, STOP_RESPONSE, INFO_RESPONSE } reason;
public:
  char const *reasonText() const {
    return
      reason==HEADER ? "Header" :
      reason==HOST_INITIATED ? "Host initiated" :
      reason==STOP_RESPONSE ? "Stop response" :
      reason==INFO_RESPONSE ? "Info response" :
      "Unknown";
  }
};

struct NS_Header {
  NS_Info info;
};

#endif //DEFS_H
