#ifndef SRCS_SERVER_RECEIVEREQUESTFROMCLIENT_HPP_
#define SRCS_SERVER_RECEIVEREQUESTFROMCLIENT_HPP_
#include <utility>

#include "Event.hpp"
#include "ReceiveHttpRequest.hpp"
#include "Socket.hpp"
class ReceiveRequestFromClient : public Event {
 private:
  Socket *socket_;
  ReceiveHttpRequest request_;
  ReadStat stat_;
  Event *cgi_;
  static bool IsReadErr(const ReadStat &st);
  static bool IsReadAgain(const ReadStat &st);
  static bool IsReadFinished(const ReadStat &st);
  static bool IsReadComplete(const ReadStat &st);

 public:
  explicit ReceiveRequestFromClient(Socket *sock);
  ~ReceiveRequestFromClient();

  void Do();
  Event *NextEvent();
  EventState State();
  Socket *GetSocket() const;
  EventType Type() const;
  std::pair<Event *, epoll_event> PublishNewEvent();
  void Handle(Epoll *epoll);
  void SetSocket(Socket *socket);
};

#endif  // SRCS_SERVER_RECEIVEREQUESTFROMCLIENT_HPP_
