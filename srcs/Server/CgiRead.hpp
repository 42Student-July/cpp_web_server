#ifndef SRCS_SERVER_CGIREAD_HPP_
#define SRCS_SERVER_CGIREAD_HPP_
#include <utility>

#include "Event.hpp"
#include "Socket.hpp"
class CgiRead : public Event {
 private:
  static const int kBuffSize = 2048;
  Socket *socket_;

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
};

#endif  // SRCS_SERVER_CGIREAD_HPP_
