#ifndef SRCS_LISTENFD_HPP_
#define SRCS_LISTENFD_HPP_
#include <netdb.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include <iostream>
#include <string>

#include "./utils/Fd.hpp"
#include "./utils/Result.hpp"
#include "Epoll.hpp"
#define string std::string
#define endl std::endl
#define cout std::cout
class ListenFd {
 private:
  typedef struct addrinfo addrinfo;
  typedef struct sockaddr SA;
  typedef struct sockaddr_storage sockaddr_storage;
  static const int listen_max = 1024;
  static const int max_line = 8192;
  Fd listen_;
  addrinfo *list_top_;
  static void init_hint(addrinfo *hint);
  static int MakeSocket(addrinfo *addr);
  void SetSocketOption() const;
  bool CanBind(addrinfo *addr) const;
  ListenFd();

 public:
  explicit ListenFd(string port);
  ~ListenFd();
  void GenerateConnectableFd();
  Fd GetFd() const;
  int AcceptFd() const;
  bool IsConnecting(const epoll_event &event) const;
};
#endif  // SRCS_LISTENFD_HPP_
