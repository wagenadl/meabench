/*
* Header file for MCCard Device Driver
*/

/* When changing anything here, please make sure that the linux-2.6 version
   is updated as well.
*/

#ifndef MCCARD_H
#define MCCARD_H

#define MCCARD_VERSION 11
#define VENDOR_ID 0x10e8
#define DEVICE_ID 0x80e8

/*#define BUFF_SIZE_BYTES 32768*/
#define MAX_BUFFERS 30    /* max # of buffers to try and allocate */
#define MIN_BUFFERS 4

// Do NOT change following - neurosock counts on them being 0 and 1
#define MC64 0
#define MC128 1

#define MCCARD_MAJOR 250

#define MCCARD_IOC_MAGIC 'k'
#define MCCARD_IOCSTART_SAMPLING  _IO(MCCARD_IOC_MAGIC,0)
#define MCCARD_IOCSTOP _IO(MCCARD_IOC_MAGIC,1)
#define MCCARD_IOCHARD_RELEASE _IO(MCCARD_IOC_MAGIC,2)
#define MCCARD_IOCGAIN _IOW(MCCARD_IOC_MAGIC,3,unsigned int)
#define MCCARD_IOQGET_DRIVER_VERSION _IOR(MCCARD_IOC_MAGIC,4,MCCARD_VERSION)
#define MCCARD_IOQSTATS _IOW(MCCARD_IOC_MAGIC,5, unsigned long *)
#define MCCARD_IOQPARAM _IOW(MCCARD_IOC_MAGIC,6, unsigned long *)
#define MCCARD_IOCENDIGI _IO(MCCARD_IOC_MAGIC,7)
#define MCCARD_IOCFORCEFIFO _IOW(MCCARD_IOC_MAGIC,8,unsigned int)
#define MCCARD_IOSETCHANNELS _IOW(MCCARD_IOC_MAGIC,9,unsigned int)
#define MCCARD_IOQINFO _IOW(MCCARD_IOC_MAGIC,10, unsigned long *)

struct MCCard_info {
  unsigned int SerialNo;
  unsigned int ChannelCount;
  char CardRevision;
  char PldRevision;
  unsigned char ok;
  char dummy;
};

struct MCCard_parameters {
  unsigned int iCHN;
  unsigned long CR;
  unsigned long  SPCR;
  unsigned long BSR;
  unsigned int iSF;
  unsigned long dma_TransferSize;
  unsigned int Gain;
  unsigned int IRQ;
  unsigned int digi;    
};

struct MCCard_stats {
  long int  Bufferstats[MAX_BUFFERS]; /*collected since driver init */
  long int  DMA_transfers;  /* collected since last start sampling */
  long int  Hardware_errors; /* "    " */
  long int  overruns;        /* "    " */
};

#endif //MCCARD_H
