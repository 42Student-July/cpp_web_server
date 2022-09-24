#ifndef SRCS_UTILS_FD_HPP_
#define SRCS_UTILS_FD_HPP_
#include <unistd.h>
class Fd {
 private:
  int fd_;
  // Fd();
  // Fd(const Fd& fd);
  // Fd operator=(const Fd& fd);

 public:
  explicit Fd(const int fd);
  ~Fd();
  int GetFd() const;
  void Reset(int fd);
  void Close();
};
#endif  // SRCS_UTILS_FD_HPP_
