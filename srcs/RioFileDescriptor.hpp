#ifndef SRCS_RIOFILEDESCRIPTOR_HPP_
#define SRCS_RIOFILEDESCRIPTOR_HPP_
#include <string.h>
#include <unistd.h>

#include <algorithm>
#include <iostream>

#include "./utils/Fd.hpp"
#include "./utils/Result.hpp"
#define RIO_BUFFER_SIZE 8192
class RioFileDescriptor {
 private:
  static const int eof = 1;
  Fd fd_;
  int buffer_remaining_;
  char *buf_pos_ptr_;
  char buf_[RIO_BUFFER_SIZE];
  ssize_t ReadFdIntoBuf();
  void CopyBufToUsrBuf(char *usr_buf, ssize_t size);
  ssize_t RioRead(char *usr_buf, ssize_t n);
  RioFileDescriptor();

 public:
  ~RioFileDescriptor();
  explicit RioFileDescriptor(const Fd &fd);
  explicit RioFileDescriptor(const int &fd);
  ssize_t ReadLineByteEach(char *usr_buf, size_t max_len);
  ssize_t WriteNbyte(const char *usr_buf, size_t n);
};
#endif  // SRCS_RIOFILEDESCRIPTOR_HPP_
