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
// #include "Connecting.hpp"
#include "Epoll.hpp"
#include "Event.hpp"
#include "HttpProcessor.hpp"
#include "HttpResponse.hpp"
#include "Listen.hpp"
#include "ListenToClient.hpp"
#include "Parser.hpp"
#include "ReceiveHttpRequest.hpp"
#include "ServerContext.hpp"
#include "Socket.hpp"
class Server {
 private:
  // static const int kNotDoneYet = -1;
  std::map<int, Event *> event_map_;
  Epoll epoll_;
  // ReceiveHttpRequest receive_request_;
  // std::map<int, std::string> response_;
  Server();
  void InitListen(const ContextMap &contexts);
  void GenerateCgi(epoll_event *ev);
  void ReadFromCgi(epoll_event *ev);
  void AddEventToMonitored(const int fd, Event *event, uint32_t event_flag);
  void AddEventToMonitored(const int fd, Event *event, epoll_event *new_ev);
  void NextEvent(Event *event, epoll_event *epoll);
  void RegisterNewEvent(Event *event);
  void DeleteEvent(Event *event, epoll_event *epoll);

 public:
  explicit Server(const ContextMap &contexts);
  ~Server();
  void Run();
};

#endif  // SRCS_SERVER_SERVER_HPP_
