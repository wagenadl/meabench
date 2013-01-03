/* meabench_mcs.h - Part of MEABench
   Copyright (C) 2000-2013 Daniel Wagenaar and others

   MEABench comes with ABSOLUTELY NO WARRANTY; for details see the
   file "COPYING". This is free software, and you are welcome to
   redistribute it under certain conditions; again, see the file
   "COPYING" for details.
*/

#ifndef MEABENCH_MCS_H
#define MEABENCH_MCS_H

#include <stdint.h>

#define MCCARD_MAX_BUFFERS 30    /* max # of buffers to try and allocate */
#define MCCARD_MIN_BUFFERS 4     /* min # of buffers to accept */

#ifdef __linux__
/* Linux: we use ioctl to communicate with the driver */
#define MCCARD_IOC_MAGIC 'k'
#define MCCARD_IOCSTART_SAMPLING     _IO(MCCARD_IOC_MAGIC,  0)
#define MCCARD_IOCSTOP               _IO(MCCARD_IOC_MAGIC,  1)
#define MCCARD_IOCHARD_RELEASE       _IO(MCCARD_IOC_MAGIC,  2)
#define MCCARD_IOCGAIN               _IOW(MCCARD_IOC_MAGIC, 3, uint32_t)
#define MCCARD_IOQGET_DRIVER_VERSION _IOR(MCCARD_IOC_MAGIC, 4, uint32_t)
#define MCCARD_IOQSTATS              _IOW(MCCARD_IOC_MAGIC, 5, uint32_t *)
#define MCCARD_IOQPARAM              _IOW(MCCARD_IOC_MAGIC, 6, uint32_t *)
#define MCCARD_IOCENDIGI             _IO(MCCARD_IOC_MAGIC,  7)
#define MCCARD_IOCFORCEFIFO          _IOW(MCCARD_IOC_MAGIC, 8, uint32_t)
#define MCCARD_IOSETCHANNELS         _IOW(MCCARD_IOC_MAGIC, 9, uint32_t)
#define MCCARD_IOQINFO               _IOW(MCCARD_IOC_MAGIC, 10, uint32_t *)

#define MEABENCH_OS_OK
#endif

#ifdef __APPLE__
/* MacOSX */
/* This is in need of some clean up */

// Ring Buffer config.. Need 64ch x 2 bytes @ 25 kHz with each
// buffer of ~10 milliseconds
// for a buffer size of 2^3  4K memory pages per DMA buffer..
#define MAX_BUFFERS 60
#define MIN_BUFFERS 10   
#define kDMABUFFERSIZE 8 // Size of each buffer kDMABUFFERSIZE * PAGE_SIZE (4K) 
#define kNUMDMABUFFERS 30   //Number of buffers we would like to allocate
#define DEFAULT_CHANNEL_COUNT 7 //2^DEFAULT_CHANNEL_COUNT channels
#define kMCCardServiceClassName   "edu_ufl_bme_MCCard"
#define kSharedDMAPCIMemoryType1 101

enum {
    kMCCardStartEngine,
    kMCCardStopEngine,
    kMCCardNextDataPtr,
    kMCCardGetStats,
    kMCCardSetGain,
    kMCCardSetCHN,
    kMCCardForceFIFO,
    kMCCardGetDriverVersion,
    kMCCardGetDriverParameters,
    kNumberOfMethods,
};

#define MEABENCH_OS_OK
#endif

#ifndef MEABENCH_OS_OK
#error Unknown operating system
#endif


/* Structures */

struct MCCard_info { /* This is used by IOQINFO */
  uint32_t SerialNo;
  uint32_t ChannelCount;
  uint8_t CardRevision;
  uint8_t PldRevision;
  uint8_t ok;
  uint8_t dummy;
};

struct MCCard_parameters { /* This is used by IOQPARAM */
  uint32_t CR;
  uint32_t SPCR;
  uint32_t BSR;
  uint32_t iSF;
  uint32_t iCHN;
  uint32_t dma_TransferSize;
  uint32_t Gain;
  uint32_t IRQ;
  uint32_t digi;    
};

struct MCCard_stats { /* Used by IOQSTATS */
  int32_t Bufferstats[MCCARD_MAX_BUFFERS]; /*collected since driver init */
  int32_t DMA_transfers;  /* collected since last start sampling */
  int32_t Hardware_errors; /* "    " */
  int32_t overruns;        /* "    " */
};

#endif
