// Mutex.H

#ifndef MUTEX_H

#define MUTEX_H

#include <pthread.h>
#include "Error.H"

class Mutex {
public:
  Mutex()  {
    if (pthread_mutex_init(&mutex,0))
         throw SysErr("Mutex","Cannot create mutex");
  }
  ~Mutex() { }
  int lock() {
    return pthread_mutex_lock(&mutex);
  }
  int unlock() {
    return pthread_mutex_unlock(&mutex);
  }
private:
  friend class Cond;
  pthread_mutex_t mutex;
};

class MLock {
public:
  MLock(Mutex &m0) : m(m0) {
    if (m.lock())
      throw SysErr("MLock","Cannot lock mutex");
  }
  ~MLock() {
    if (m.unlock()) {
      SysErr e("MLock","Cannot lock mutex");
      e.report();
    }
  }
private:
  Mutex &m;
};

class Cond {
public:
  Cond() {
    int r=pthread_cond_init(&cond,0);
    if (r)
      throw SysErr("Cond","Cannot create cond");
  }
  ~Cond() { }
  void wait() {
    int q;
    { MLock ml(mutex);
      q=pthread_cond_wait(&cond,&mutex.mutex);
    }
    if (q)
      throw SysErr("Cond","Cannot wait for cond");
  }
  void signal() {
    int q;
    { MLock ml(mutex);
      q=pthread_cond_signal(&cond);
    }
    if (q)
      throw SysErr("Cond","Cannot signal cond");
  }
private:
  Mutex mutex;
  pthread_cond_t cond;
};

#endif
