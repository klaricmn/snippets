#include "Timing.h"

#include <cstddef>
#include <sys/time.h>

sst::Timing::Timing() {
  init();
}

void sst::Timing::init() {
  _isRunning = false;
  _startingTime = 0;
  _elapsedTime = 0;
}

void sst::Timing::start() {
  if(false == _isRunning) {
    _isRunning = true;
    _startingTime = getCurrentTime();
  }
}

void sst::Timing::split() {
  stop();
  start();
}

void sst::Timing::stop() {
  if(true == _isRunning) { 
    _elapsedTime += getSplitElapsedTime();
    _isRunning = false;
  }
}

void sst::Timing::reset() {
  init();
}

double sst::Timing::getTotalElapsedTime() const {
  if(false == _isRunning)
    return _elapsedTime;
  else
    return _elapsedTime + getSplitElapsedTime(); 
}

double sst::Timing::getSplitElapsedTime() const {
  return getCurrentTime() - _startingTime;
}

double sst::Timing::getCurrentTime() const {
  struct timeval tp;
  int rtn;
  rtn=gettimeofday(&tp, NULL);
  return (double)tp.tv_sec+(1.e-6)*tp.tv_usec;
}
