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

#include "Cgi.hpp"
#include "Connecting.hpp"
#include "Epoll.hpp"
#include "Event.hpp"
#include "HttpResponse.hpp"
#include "Listen.hpp"
#include "ListenEvent.hpp"
#include "Parser.hpp"
#include "ReceiveHttpRequest.hpp"
#include "ServerContext.hpp"
class Server {
 private:
  static const int kNotDoneYet = -1;
  std::map<int, Event *> events_;
  Epoll epoll_;
  ReceiveHttpRequest receive_request_;
  std::map<int, std::vector<std::string> > response_;
  void ExecEvents(epoll_event *ev);
  void AcceptNewConnections(epoll_event *ev);
  void ConnectingEvent(epoll_event *ev);
  void AddMonitorToRequest(epoll_event *fd);
  void ReceiveRequest(epoll_event *ev);
  void SendResponse(epoll_event *ev);
  int WriteToClientFd(const int connfd);
  Server();
  void DelEvent(const Event *sock, epoll_event *ev);
  void InitListenEvent(const std::vector<ServerContext> &contexts);
  static void CgiRun(epoll_event *ev);
  static void CgiEvent(epoll_event *ev);
  void AddEventToMonitored(Event *sock, uint32_t event_flag);

 public:
  explicit Server(const std::vector<ServerContext> &contexts);
  ~Server();
  void Run();
};

#endif  // SRCS_SERVER_SERVER_HPP_
