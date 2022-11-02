#ifndef SRCS_SERVER_LISTENFD_HPP_
#define SRCS_SERVER_LISTENFD_HPP_
#include <netdb.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include <exception>
#include <iostream>
#include <string>

#include "Epoll.hpp"
#include "Result.hpp"
class ListenFd {
 private:
  typedef struct addrinfo addrinfo;
  typedef struct sockaddr SA;
  typedef struct sockaddr_storage sockaddr_storage;
  static const int listen_max = 1024;
  static const int max_line = 8192;
  int listen_fd_;
  addrinfo *list_top_;
  static void init_hint(addrinfo *hint);
  static int MakeSocket(addrinfo *addr);
  int SetupSocket();
  void SetSocketOption() const;
  bool CanBind(addrinfo *addr) const;
  ListenFd();

 public:
  explicit ListenFd(std::string port);
  ~ListenFd();
  void GenerateConnectableFd();
  int GetFd() const;
  int AcceptFd() const;
  bool IsNewConnection(const epoll_event &event) const;
};
#endif  // SRCS_SERVER_LISTENFD_HPP_
