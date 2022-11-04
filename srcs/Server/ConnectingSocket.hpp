#ifndef SRCS_SERVER_CONNECTINGSOCKET_HPP_
#define SRCS_SERVER_CONNECTINGSOCKET_HPP_

#include <string>

#include "Socket.hpp"
class ConnectingSocket : public Socket {
 private:
  static const int Kbuffer_size_ = 8192;

 public:
  ConnectingSocket(const int fd, const ServerContext& context);
  ~ConnectingSocket();
  ssize_t Write(const char* str, size_t size);
  std::string Read();
};

#endif  // SRCS_SERVER_CONNECTINGSOCKET_HPP_
