/* 
 * read_card.c -- program reads data from MCCard DMA enabled driver in 10 ms chunks
 */


#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <asm/io.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <linux/types.h>
#include <sys/ioctl.h>
#include <linux/types.h>
#include "MCCard.h"

int main(int argc, char **argv)
{
    unsigned int i,sweep,print_enabled=0,sample_time=2,Gain=3;
    unsigned long chh=0,chl=0;
    unsigned long channel_buf[10000],count;	
    int fd, numbytes,bytes_processed,get_stats=0;    
    struct MCCard_stats Stats;
    struct MCCard_parameters Parameters;


    while ((argc > 1) && (argv[1][0]=='-')) {

	switch (argv[1][1]) {
	    
	    case 'p':
		print_enabled=1; /* output data to stdout */
		break;
	    case 't':
		sample_time=atoi(&argv[1][2]); /* number of buffers to read */
		printf("Reading %d samples\n",sample_time);
		break;
	    case 's':	
		get_stats=1;
		break;
	    case 'g':
		Gain=atoi(&argv[1][2]);
		printf("Gain set to %d\n",Gain);
		break;
	    case 'h':
		printf("usage: rawread -t<#buffers to read> -p<print> -s<stats> -g<Gain 0-3>\n");
		exit(0);
	}    
    ++argv;
    --argc;
    }

  /* Open the device */
  if ((fd = open("/dev/MCCard", O_RDONLY))==-1) {
     printf("Opening device failed (%d) with error no: %d: %s\n",fd,errno,
            strerror(errno));
     exit(0);
     }
 
 /* Set Gain of card */
  if (ioctl(fd, MCCARD_IOCGAIN,&Gain)) {
     perror("IOctl error");exit(0);
   }

  /* Read Card parameters to determine buffer size */
   if (ioctl(fd, MCCARD_IOQPARAM,&Parameters)) {
     perror("IOctl error");exit(0);
   }
   printf("Each buffer is %ld bytes\n",Parameters.dma_TransferSize);

  /* reset addon logic and begin data acq */
   printf("Start sampling\n");
   ioctl(fd, MCCARD_IOCSTART_SAMPLING);  

  

/* Read raw data from card */
for(sweep=1;sweep<=sample_time;sweep++) {  
  /*printf("<%d> Read 10 ms buffer\n",sweep);*/
   printf(".");

  numbytes=read(fd,channel_buf, Parameters.dma_TransferSize);  /* Grab 10 ms at time */
  if (numbytes==-1) {
    printf("Error reading from card\n");
    break;
  }
  count=0;bytes_processed=0;

 
  if (print_enabled) {
    do {
 
        for(i=0;i<32;i++,count++) {
           chh=channel_buf[count] >> 16;chl=channel_buf[count] & 0xFFFF;
           printf("%ld %ld ",chh,chl);
           bytes_processed=bytes_processed+4;
	}
	
        printf("\n");
     } while (bytes_processed < numbytes);
 }

}


/* Stop sampling on card */
  ioctl(fd, MCCARD_IOCSTOP); 

/* Get driver stats, pass a pointer to your struct MCCard_stats */
 ioctl(fd, MCCARD_IOQSTATS, &Stats);  

if (get_stats) {
  printf ("Transferred %ld DMA buffers\n",Stats.DMA_transfers);
  printf("  %ld Hardware and %ld Overrun errors\n",Stats.Hardware_errors,Stats.overruns);
  for(sweep=0;sweep < MAX_BUFFERS;sweep++) 
   printf("lag %d Frequency %ld\n",sweep,Stats.Bufferstats[sweep]);
}
 close(fd);
 exit(0);    
}





