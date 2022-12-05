#ifndef SRCS_SERVER_LISTENTOCLIENT_HPP_
#define SRCS_SERVER_LISTENTOCLIENT_HPP_
#include <iostream>
#include <utility>
#include <vector>

#include "Event.hpp"
#include "ServerContext.hpp"
#include "Socket.hpp"
class ListenToClient : public Event {
 private:
  int listen_fd_;
  std::vector<ServerContext> context_;
  int conn_fd_;
  Socket *socket_;

 public:
  ListenToClient(const int listen_fd,
                 const std::vector<ServerContext> &context);
  ~ListenToClient();
  void Do();
  Event *NextEvent();
  EventState State();
  Socket *GetSocket() const;
  EventType Type() const;
  std::pair<Event *, epoll_event> PublishNewEvent();
  void Handle(Epoll *epoll);
  int NewEventFd() const;
};

#endif  // SRCS_SERVER_LISTENTOCLIENT_HPP_
