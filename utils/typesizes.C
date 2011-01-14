#include <common/Types.H>
#include <stdio.h>
#include <stddef.h>

struct foo {
  int a;
  int b;
};

struct bar {
  foo x;
  int c;
};

struct tst {
  Spikeinfo s;
  short t;
};

int main(int, char **) {
  printf("MEABench type sizes\n");
  printf("sizeof(raw_t) = %i\n",int(sizeof(raw_t)));
  printf("sizeof(real_t) = %i\n",int(sizeof(real_t)));
  printf("sizeof(timeref_t) = %i\n",int(sizeof(timeref_t)));
  printf("sizeof(Sample) = %i\n",int(sizeof(Sample)));
  printf("sizeof(Spikeinfo) = %i\n",int(sizeof(Spikeinfo)));

  Spikeinfo s;
  printf("offsetof(Spikeinfo.time) = %i\n",int(offsetof(Spikeinfo,time)));
  printf("offsetof(Spikeinfo.channel) = %i\n",int(offsetof(Spikeinfo,channel)));
  printf("offsetof(Spikeinfo.height) = %i\n",int(offsetof(Spikeinfo,height)));
  printf("offsetof(Spikeinfo.width) = %i\n",int(offsetof(Spikeinfo,width)));
  printf("offsetof(Spikeinfo.context) = %i\n",int(offsetof(Spikeinfo,context)));
  printf("offsetof(Spikeinfo.threshold) = %i\n",int(offsetof(Spikeinfo,threshold)));
  
  printf("sizeof(foo) = %i\n",int(sizeof(foo)));
  printf("sizeof(bar) = %i\n",int(sizeof(bar)));
  printf("sizeof(tst) = %i\n",int(sizeof(tst)));
  return 0;
}

