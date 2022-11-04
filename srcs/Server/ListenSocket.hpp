#ifndef SRCS_SERVER_LISTENSOCKET_HPP_
#define SRCS_SERVER_LISTENSOCKET_HPP_
#include "Fd.hpp"
#include "Socket.hpp"
class ListenSocket : public Socket {
 private:
  ListenSocket();

 public:
  ListenSocket(const int fd, const ServerContext& context);
  ~ListenSocket();
  int Accept();
};

#endif  // SRCS_SERVER_LISTENSOCKET_HPP_
