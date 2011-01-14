/* utils/resplitraw2CR.C: part of meabench, an MEA recording and analysis tool
** Copyright (C) 2000-2002  Daniel Wagenaar (wagenaar@caltech.edu)
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

// resplitraw.C

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include <stdio.h>
#include <stdlib.h>

#include <common/Types.H>
#include <common/Config.H>
#include <base/Variance.H>
#include <common/ChannelNrs.H>

#include <map>

void usage() {
  fprintf(stderr,"Usage: resplitraw2CR window_ms windows_per_file outnamebase\n"
	  "\n"
	  "Splits a very long raw file into manageable pieces.\n"
	  "Also, writes a list of suspected stimulus channels to stdout.\n"
	  "outnamebase must contain a %%i to receive the CR and a %%%%i (!) to receive\n"
	  "the file number.\n"
	  "The input is read using read(), not fread(), so very long input is OK.\n"
	  "Each window is written to a file identified by its (auto id'd) stim channel.\n");
  exit(1);
}

class OneFile {
public:
  OneFile() { fh=0; *myfn=0; }
  OneFile(char const *basefn, int hw, int maxw0) {
    int c,r; hw2cr(hw,c,r);
    int cr=c*10 + r + 11;
    sprintf(myfn,basefn,cr);
    fh=0;
    num=0;
    nowwin=0;
    maxwin=maxw0;
  }
  ~OneFile() {
    fprintf(stderr,"Fileset %s being closed with %i file%s, last window is %i\n",
	    myfn, num, num==1?"":"s", nowwin);
    closeensure(true);
  }
  void closeensure(bool always=false) {
    if (fh) {
      if (always || nowwin>=maxwin) {
	fclose(fh);
	fh=0;
      }
    }
  }
  void openensure() {
//    fprintf(stderr,"openensure\n");
    if (!fh) {
      if (!myfn[0]) {
	fprintf(stderr,"resplitraw2CR: BUG: openensure called on dummy OneFile\n");
	exit(6);
      }
      char buf[1000];
      nowwin=0;
      sprintf(buf,myfn,++num);
//      fprintf(stderr,"openensure, fn=%s\n",buf);
      fh=fopen(buf,"wb");
      if (!fh) {
	perror("resplitraw2CR: Cannot create output file");
	exit(3);
      }
    }
  }
  void writeone(Sample *src, unsigned int nsams) {
//    fprintf(stderr,"Writeone: src=%p ns=%i fh=%p myfn=%s num=%i maxwin=%i nowwin=%i\n",
//	      src,nsams,fh,myfn,num,maxwin,nowwin);
    closeensure();
    openensure();
//    fprintf(stderr,"(File %s: %i, win %i)\n",myfn,num,nowwin);
    if (fwrite(src,sizeof(Sample),nsams,fh) != nsams) {
      perror("resplitraw2CR: write problem");
      exit(2);
    }
    nowwin++;
  }
private:
  FILE *fh;
  char myfn[1000];
  int num;
  int maxwin;
  int nowwin;
};

int main(int argc, char **argv) {
  if (argc!=4)
    usage();
  int win_sams = FREQKHZ*atoi(argv[1]);
  int nwins = atoi(argv[2]);
  char const *namebase = argv[3];

  map<int, OneFile *> outfiles;

  Variance<float> v[NCHANS];
  Sample *buf = new Sample[win_sams];
  bool stop=false;
  int winno=0;
  
  while (1) {
    char *bufp=(char*)buf;
    int remain = sizeof(Sample)*win_sams;
    while (remain) {
      int n=read(0,bufp,remain);
      if (n<0) {
	perror("resplitraw2CR: read error");
	exit(2);
      } else if (n==0) {
	fprintf(stderr,"EOF\n");
	stop=true;
	break;
      }
      bufp+=n;
      remain-=n;
    }
    if (stop)
      break;

    // OK, I've got a window full.
    for (int c=0; c<NCHANS; c++)
      v[c].reset(buf[0][c]);
    for (int n=0; n<win_sams; n++)
      for (int c=0; c<NCHANS; c++)
	v[c].addexample(buf[n][c]);
    int cmax=0;
    float max=0;
    for (int c=0; c<NCHANS; c++) {
      float var=v[c].var();
      if (var>max) {
	max=var; cmax=c;
      }
    }

    // So cmax is the relevant hardware channel.
    map<int, OneFile *>::iterator i=outfiles.find(cmax);
    OneFile *p;
    if (i==outfiles.end())
      outfiles[cmax] = p = new OneFile(namebase,cmax,nwins);
    else
      p = (*i).second;

    fprintf(stderr,"win=%i hw=%i\n",winno,cmax);
    
    printf("%i %i\n",winno,cmax);

    p->writeone(buf,win_sams);

    winno++;
  }

  for (map<int,OneFile *>::iterator i=outfiles.begin(); i!=outfiles.end(); ++i)
    if ((*i).second)
      delete (*i).second;
  
  return 0;
}
