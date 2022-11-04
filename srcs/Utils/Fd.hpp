#ifndef SRCS_UTILS_FD_HPP_
#define SRCS_UTILS_FD_HPP_
#include <unistd.h>

#include <stdexcept>
#include <string>
class Fd {
 private:
  int val_;

 public:
  explicit Fd(const int fd);
  ~Fd();
  int GetFd() const;
  void Reset(const int fd);
  void Close() const;
};

#endif  // SRCS_UTILS_FD_HPP_
