#ifndef SRCS_SERVER_HPP_
#define SRCS_SERVER_HPP_
#include <fcntl.h>
#include <stdio.h>
#include <sys/select.h>

#include <iostream>
#include <map>
#include <string>
#include <vector>

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
  static const int kNotDoneYet = -1;
  ListenFd listen_;
  Epoll epoll_;
  std::map<int, std::vector<string> > requests_;
  std::map<int, std::vector<string> > response_;
  void AcceptNewConnections();
  void ReceiveRequest(epoll_event *ev);
  void SendResponse(epoll_event *ev);
  int ReadRequest(const Fd &fd);
  int WriteToClientFd(const Fd &connfd);
  static void SetNonBlocking(const int &fd);
  Server();

 public:
  explicit Server(string port);
  ~Server();
  void Run();
};
#endif  //  SRCS_SERVER_HPP_
