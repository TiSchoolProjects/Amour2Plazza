#pragma once

#include "pthread.h"

class IMutex {
public:
  virtual ~IMutex() = default;
  virtual void lock() = 0;
  virtual void unlock() = 0;
  virtual bool trylock() = 0;
};

class Mutex : public IMutex {
public:
  Mutex();
  virtual void lock() override;
  virtual void unlock() override;
  virtual bool trylock() override;

private:
  pthread_mutex_t _mutex;
};
