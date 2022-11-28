#ifndef SRCS_UTILS_TIMER_HPP_
#define SRCS_UTILS_TIMER_HPP_
#include <sys/time.h>

#include <iostream>
class Timer {
 private:
  size_t out_time_;
  timeval start_;

 public:
  explicit Timer(const size_t out_time);
  ~Timer();
  bool TimeOver() const;
};

#endif  // SRCS_UTILS_TIMER_HPP_
