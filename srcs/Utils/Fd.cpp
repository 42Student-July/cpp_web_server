#include "Fd.hpp"
Fd::Fd(const int fd) : fd_(fd) {}

Fd::~Fd() {
  // close(fd_);
}

int Fd::GetFd() const { return fd_; }

void Fd::Reset(const int fd) {
  close(fd_);
  fd_ = fd;
}
void Fd::Close() { Reset(-1); }
