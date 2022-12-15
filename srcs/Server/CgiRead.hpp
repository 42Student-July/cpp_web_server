#ifndef SRCS_SERVER_CGIREAD_HPP_
#define SRCS_SERVER_CGIREAD_HPP_
#include <utility>

#include "CgiParser.hpp"
#include "Event.hpp"
#include "Socket.hpp"
class CgiRead : public Event {
 private:
  bool created_next_event_;
  Socket *socket_;
  CgiParser cgi_parser_;
  bool del_;
  std::pair<Event *, epoll_event> CreateLocalRedirEvent();
  std::pair<Event *, epoll_event> CreateClientEvent();

 public:
  explicit CgiRead(Socket *socket);
  ~CgiRead();
  void Do();
  Event *NextEvent();
  std::pair<Event *, epoll_event> PublishNewEvent();
  void Handle(Epoll *epoll);
  EventState State();
  Socket *GetSocket() const;
  EventType Type() const;
  void SetSocket(Socket *socket);
};

#endif  // SRCS_SERVER_CGIREAD_HPP_
