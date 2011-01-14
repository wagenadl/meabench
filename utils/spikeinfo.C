/* utils/spikeinfo.C: part of meabench, an MEA recording and analysis tool
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

#include <stdio.h>
#include <stdlib.h>
#include <common/Types.H>
#include <common/Config.H>
#include <base/Sprintf.H>
#include <base/Description.H>
#include <base/dbx.H>

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <time.h>

#include <string>

void usage() {
  fprintf(stderr,"Usage: spikeinfo filename\n");
  exit(1);
}

void error(char const *x) {
  string msg = "spikeinfo: ";
  msg+=x;
  perror(msg.c_str());
  exit(2);
}

int main(int argc, char **argv) {
  if (argc!=2)
    usage();

  string base = argv[1];

  struct stat s;
  
  if (stat(base.c_str(),&s))
    error("Base file not found");

  time_t start_inode = s.st_ctime;
  timeref_t length = s.st_size;
  time_t end_file = s.st_mtime;

  int ext;
  for (ext=1; ; ext++) {
    string fn = Sprintf("%s-%i",base.c_str(),ext);
    if (stat(fn.c_str(),&s))
      break;
    end_file = s.st_mtime;
    length += s.st_size;
  }

  string fn = base;
  if (ext>1)
    fn += Sprintf("-%i",ext-1);
  FILE *fd = fopen(fn.c_str(),"rb");
  if (!fd)
    error("Can't open last file in set");
  if (fseek(fd,-SpikeinfoDISKSIZE,SEEK_END))
    error("Can't seek to last spikeinfo");
  Spikeinfo si;
  if (!fread(&si,SpikeinfoDISKSIZE,1,fd))
    error("Can't read last spikeinfo");
  fclose(fd);
  int secs = si.time /FREQKHZ/1000;
  int mins = secs/60; secs-=mins*60;
  int hrs = mins/60; mins-=hrs*60;

  string end_desc = "[Unknown]";
  string spikes_desc = "[Unknown]";
  string fn_desc = "[Unknown]";
  
  try {
    ReadDescription desc(base + ".desc");
    end_desc = desc["Date"];
    fn_desc = desc["Data filename"];
    spikes_desc = desc["Number of spikes"];
  } catch (Error const &e) {
    e.report();
  }

  struct tm stt,ent;
  localtime_r(&start_inode,&stt);
  localtime_r(&end_file,&ent);

  int nspikes = length/SpikeinfoDISKSIZE;
  string nspk = "";
  if (nspikes>1000*1000)
    nspk = Sprintf("%i,%03i,%03i",nspikes/1000000,(nspikes/1000)%1000,nspikes%1000);
  else if (nspikes>1000)
    nspk = Sprintf("%i,%03i",nspikes/1000,nspikes%1000);
  else
    nspk = Sprintf("%i",nspikes);
    

  printf("Filename:           %s\n",base.c_str());
  printf("Filename (desc)     %s\n",fn_desc.c_str());
  printf("# files:            %i\n",ext);
  printf("# spikes (file):    %s\n",nspk.c_str());
  printf("# spikes (desc):    %s\n",spikes_desc.c_str());
  printf("Start time (inode): %s",asctime(&stt));
  printf("End time (file):    %s",asctime(&ent));
  printf("End time (desc):    %s\n",end_desc.c_str());
  printf("Duration (file):    %i:%02i:%02i\n",hrs,mins,secs);

  return 0;
}

