#include "ConnectingSocket.hpp"
ConnectingSocket::ConnectingSocket(const int fd, const ServerContext& context)
    : Socket(fd, context, CONNECTING) {}
ConnectingSocket::~ConnectingSocket() {}

ssize_t ConnectingSocket::Write(const char* str, size_t size) {
  ssize_t written_size = write(GetFd(), str, size);
  if (written_size == -1) throw std::runtime_error("write err");
  return written_size;
}
std::string ConnectingSocket::Read() {
  char buf[Kbuffer_size_];
  ssize_t read_size = read(GetFd(), buf, Kbuffer_size_);
  if (read_size == -1) throw std::runtime_error("read err");
  buf[read_size] = '\0';
  std::string ret(buf);
  return ret;
}
