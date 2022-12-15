#ifndef SRCS_SERVER_RESPONSETOTHECLIENT_HPP_
#define SRCS_SERVER_RESPONSETOTHECLIENT_HPP_
#include <string>
#include <utility>

#include "Event.hpp"
#include "Sender.hpp"
#include "Socket.hpp"
class ResponseToTheClient : public Event {
 private:
  Socket *socket_;
  std::string response_;
  Sender sender_;
  EventState state_;

 public:
  explicit ResponseToTheClient(Socket *sock);
  ~ResponseToTheClient();
  void Do();
  Event *NextEvent();
  EventState State();
  Socket *GetSocket() const;
  EventType Type() const;
  std::pair<Event *, epoll_event> PublishNewEvent();
  void Handle(Epoll *epoll);
  void SetSocket(Socket *socket);
};

#endif  // SRCS_SERVER_RESPONSETOTHECLIENT_HPP_
