#ifndef SRCS_SERVER_EVENT_HPP_
#define SRCS_SERVER_EVENT_HPP_
#include <sys/epoll.h>

#include <iostream>
#include <string>
#include <utility>

#include "Epoll.hpp"
#include "Fd.hpp"
#include "ServerContext.hpp"
#include "Socket.hpp"
enum EventType { kListen, kConn, kCgi };
enum EventState {
  kRead,
  kReadAgain,
  kReadFinished,
  kWrite,
  kWriteAgain,
  kWriteFinished,
  kCgiWrite,
  kCgiRead,
  kCgiReadAgain,
  kCgiReadFinished,
  kWait,
  kDel,
  kErr,
  kCgiErr
};
class Event {
 public:
  virtual ~Event();
  virtual void Do() = 0;
  virtual Event *NextEvent() = 0;
  virtual std::pair<Event *, epoll_event> PublishNewEvent() = 0;
  virtual void Handle(Epoll *epoll) = 0;
  virtual EventState State() = 0;
  virtual Socket *GetSocket() const = 0;

  virtual EventType Type() const = 0;
  static bool IsNotDelete(const EventState &state);
  static bool IsFinished(const EventState &state);
  static bool IsDelete(const EventState &st);
};

#endif  // SRCS_SERVER_EVENT_HPP_
