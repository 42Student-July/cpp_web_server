#include "ListenEvent.hpp"

#include <sys/socket.h>
ListenEvent::ListenEvent(const int fd, const ServerContext& context)
    : Event(fd, context, kListen) {}
ListenEvent::~ListenEvent() {}
int ListenEvent::Accept() {
  int conn = accept(GetFd(), NULL, NULL);
  if (conn == -1) throw std::runtime_error("accept");
  std::cout << "new connecting: " << conn << std::endl;
  return conn;
}
