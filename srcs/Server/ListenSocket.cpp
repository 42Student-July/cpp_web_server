#include "ListenSocket.hpp"

#include <sys/socket.h>
ListenSocket::ListenSocket(const int fd, const ServerContext& context)
    : Socket(fd, context, LISTEN) {}
ListenSocket::~ListenSocket() {}
int ListenSocket::Accept() {
  int conn = accept(GetFd(), NULL, NULL);
  if (conn == -1) throw std::runtime_error("accept");
  return conn;
}
