#ifndef SRCS_SERVER_SERVER_HPP_
#define SRCS_SERVER_SERVER_HPP_
#include <fcntl.h>
#include <stdio.h>
#include <sys/select.h>

#include <iostream>
#include <map>
#include <string>
#include <vector>

#include "Epoll.hpp"
#include "HttpResponse.hpp"
#include "ListenFd.hpp"
#include "ParseRequestMessage.hpp"
#include "Result.hpp"
// namespace ft{
class Server {
 private:
  enum { line, header, body, eof };
  static const int kMaxline = 8192;
  static const int kNotDoneYet = -1;

  ListenFd listen_;
  Epoll epoll_;
  std::map<int, std::vector<std::string> > response_;
  void ReadFromBuffer();
  void IOEvents();
  void AcceptNewConnections();
  void AddMonitorToRequest(epoll_event *fd);
  static void ReceiveRequest(epoll_event *ev);
  void SendResponse(epoll_event *ev);
  // int ReadRequest(const Fd &fd);
  int WriteToClientFd(const int connfd);
  Server();

 public:
  explicit Server(std::string port);
  ~Server();
  void Run();
};

#endif  // SRCS_SERVER_SERVER_HPP_
