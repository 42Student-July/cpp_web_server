#ifndef SRCS_SERVER_LISTENEVENT_HPP_
#define SRCS_SERVER_LISTENEVENT_HPP_
#include "Event.hpp"
#include "Fd.hpp"
class ListenEvent : public Event {
 private:
  ListenEvent();

 public:
  ListenEvent(const int fd, const ServerContext& context);
  ~ListenEvent();
  int Accept();
};

#endif  // SRCS_SERVER_LISTENEVENT_HPP_
