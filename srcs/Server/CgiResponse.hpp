#ifndef SRCS_SERVER_CGIRESPONSE_HPP_
#define SRCS_SERVER_CGIRESPONSE_HPP_
#include <string>
#include <utility>

#include "Event.hpp"
#include "SendChunked.hpp"
#include "Sender.hpp"
#include "Socket.hpp"
class CgiResponse : public Event {
 private:
  Socket *socket_;
  Sender sender_;
  SendChunked chunked_;
  std::string response_state_and_header_;
  std::string MakeResponseStatusCode();
  std::string MakeResponseHeader();

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
