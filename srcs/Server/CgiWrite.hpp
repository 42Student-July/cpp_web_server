#ifndef SRCS_SERVER_CGIWRITE_HPP_
#define SRCS_SERVER_CGIWRITE_HPP_
#include <utility>

#include "Epoll.hpp"
#include "Event.hpp"
#include "Sender.hpp"
#include "Socket.hpp"
class CgiWrite : public Event {
 private:
  Socket *socket_;
  Sender sender_;

 public:
  explicit CgiWrite(Socket *socket);
  ~CgiWrite();
  void Do();
  Event *NextEvent();
  std::pair<Event *, epoll_event> PublishNewEvent();
  void Handle(Epoll *epoll);
  EventState State();
  Socket *GetSocket() const;
  EventType Type() const;
};
#endif  // SRCS_SERVER_CGIWRITE_HPP_
