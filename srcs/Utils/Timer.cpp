#include "Timer.hpp"

Timer::Timer(const size_t out_time) : out_time_(out_time) {
  gettimeofday(&start_, NULL);
}
Timer::~Timer() {}
bool Timer::TimeOver() const {
  timeval now;
  gettimeofday(&now, NULL);
  return out_time_ <= static_cast<size_t>(now.tv_sec - start_.tv_sec);
}
