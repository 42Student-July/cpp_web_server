#ifndef SRCS_SERVER_LISTEN_HPP_
#define SRCS_SERVER_LISTEN_HPP_
#include <arpa/inet.h>
#include <netdb.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include <exception>
#include <iostream>
#include <stdexcept>
#include <string>

#include "Epoll.hpp"
#include "Fd.hpp"
#include "ServerContext.hpp"
#include "Utils.hpp"
class Listen {
 private:
  static const int kListenMax = 1024;
  struct sockaddr_in addr_;
  std::string host_;
  std::string port_;
  void InitSockAddr();
  Listen();

 public:
  Listen(const std::string &host, const std::string &port);
  ~Listen();
  int GenerateConnectableFd();
};
#endif  // SRCS_SERVER_LISTEN_HPP_
