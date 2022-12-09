#ifndef SRCS_SERVER_CGIRESPONSE_HPP_
#define SRCS_SERVER_CGIRESPONSE_HPP_
#include <utility>

#include "Event.hpp"
#include "Socket.hpp"
class CgiResponse : public Event {
 private:
  Socket *socket_;

 public:
  explicit CgiResponse(Socket *sock);
  ~CgiResponse();
  void Do();
  Event *NextEvent();
  std::pair<Event *, epoll_event> PublishNewEvent();
  void Handle(Epoll *epoll);
  EventState State();
  Socket *GetSocket() const;
  EventType Type() const;
};

#endif  // SRCS_SERVER_CGIRESPONSE_HPP_
