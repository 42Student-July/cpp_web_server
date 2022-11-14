#ifndef SRCS_SERVER_SERVER_HPP_
#define SRCS_SERVER_SERVER_HPP_
#include <fcntl.h>
#include <stdio.h>
#include <sys/select.h>

#include <iostream>
#include <map>
#include <string>
#include <utility>
#include <vector>

#include "ConnectingSocket.hpp"
#include "Epoll.hpp"
#include "HttpResponse.hpp"
#include "Listen.hpp"
#include "ListenSocket.hpp"
#include "ParseRequestMessage.hpp"
#include "Parser.hpp"
#include "Result.hpp"
#include "ServerContext.hpp"
#include "Socket.hpp"
class Server {
 private:
  static const int kNotDoneYet = -1;
  std::map<int, Socket *> sockets_;
  Epoll epoll_;
  // ReceiveHttpRequest receive_request_;
  std::map<int, std::vector<std::string> > response_;

  void IOEvents(epoll_event *ev);
  void AcceptNewConnections(epoll_event *ev);
  void ConnectingEvent(epoll_event *ev);
  void AddMonitorToRequest(epoll_event *fd);
  static void ReceiveRequest(epoll_event *ev);
  void SendResponse(epoll_event *ev);
  // int ReadRequest(const Fd &fd);
  int WriteToClientFd(const int connfd);
  Server();
  void DelSocket(const Socket *sock);
  void InitListenSocket(const std::vector<ServerContext> &contexts);
  void AddListenSocketsToEvents();

 public:
  explicit Server(const std::vector<ServerContext> &contexts);
  ~Server();
  void Run();
};

#endif  // SRCS_SERVER_SERVER_HPP_
