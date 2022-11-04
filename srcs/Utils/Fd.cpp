#include "Fd.hpp"
Fd::Fd(const int fd) : val_(fd) {}
Fd::~Fd() {}
int Fd::GetFd() const { return val_; }
void Fd::Close() const {
  int res = close(val_);
  if (res == -1) throw std::runtime_error("close err");
}
void Fd::Reset(const int fd) {
  close(val_);
  val_ = fd;
}
