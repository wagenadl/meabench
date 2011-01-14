// MedianFlt.H

#ifndef MEDIANFLT_H

#define MEDIANFLT_H

class MedianFlt {
public:
  MedianFlt(int N) {
    one = new short[N];
    two = new short[N];
    for (int n=0; n<N; n++)
      one[n]=two[n]=0;
  }
  ~MedianFlt() {
    delete [] one;
    delete [] two;
  }
  short median(short x, int n) {
    short y=one[n];
    short z=two[n];
    one[n]=x;
    if (x<y) {
      if (z<x) // z<x<y
	return x;
      else if (z<y) // x<=z<y
	return z;
      else // x<y<=z
	return y;
    } else {
      if (z<y) // z<y<=x
	return y;
      else if (z<x) // y<=z<x
	return z;
      else // y<=x<=z
	return x;
    }
  }
  void swap() { swap(one,two); }
private:
  void swap(short *a, short *b) {
    short *c=a;
    a=b;
    b=c;
  }
private:
  short *one;
  short *two;
};

#endif
