#ifndef SRCS_SERVER_HPP_
#define SRCS_SERVER_HPP_
#include <fcntl.h>
#include <stdio.h>
#include <sys/select.h>

#include <iostream>
#include <string>

#include "./utils/Fd.hpp"
#include "./utils/Result.hpp"
#include "Epoll.hpp"
#include "HttpResponse.hpp"
#include "ListenFd.hpp"
#include "ParseRequestMessage.hpp"
#include "RioFileDescriptor.hpp"
#define string std::string
#define cout std::cout
#define endl std::endl
// namespace ft{
class Server {
 private:
  static const int kMaxline = 8192;
  ListenFd listen_;
  Epoll epoll_;
  void CommunicatingWithClient(epoll_event* ep);
  void StartCommunicationWithClient();
  Server();

 public:
  explicit Server(string port);
  ~Server();
  void Run();
  static int Echo(const Fd& fd);
};
#endif  // SRCS_SERVER_HPP_
