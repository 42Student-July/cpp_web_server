#ifndef SRCS_SERVER_RECEVEREQUESTFROMCLIENT_HPP_
#define SRCS_SERVER_RECEVEREQUESTFROMCLIENT_HPP_
#include <utility>

#include "Event.hpp"
#include "ReceiveHttpRequest.hpp"
#include "Socket.hpp"
class ReceveRequestFromClient : public Event {
 private:
  Socket *socket_;
  ReceiveHttpRequest request_;
  ReadStat stat_;
  Event *cgi_;
  // EventState event_state_;
  static bool IsReadErr(const ReadStat &st);
  static bool IsReadAgain(const ReadStat &st);
  static bool IsReadFinished(const ReadStat &st);
  static bool IsReadComplete(const ReadStat &st);
  void ExecMethodOrCgi();

 public:
  explicit ReceveRequestFromClient(Socket *sock);
  ~ReceveRequestFromClient();

  void Do();
  Event *NextEvent();
  EventState State();
  Socket *GetSocket() const;
  EventType Type() const;
  std::pair<Event *, epoll_event> PublishNewEvent();
  void Handle(Epoll *epoll);
};

#endif  // SRCS_SERVER_RECEVEREQUESTFROMCLIENT_HPP_
