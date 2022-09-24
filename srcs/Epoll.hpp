#ifndef SRCS_EPOLL_HPP_
#define SRCS_EPOLL_HPP_
#include <string.h>
#include <sys/epoll.h>

#include <iostream>
#include <vector>

#include "./utils/Fd.hpp"
#define MAX_EVENT 100
class Epoll {
 private:
  // static const int max_event = 100;
  Fd epoll_;
  epoll_event events_[MAX_EVENT];

 public:
  Epoll();
  ~Epoll();
  void Create();
  void Init(const Fd &connfd);
  epoll_event FindEvent(const int &n) const;
  static epoll_event Create(const Fd &connfd);
  int Wait();
  void Del(epoll_event *ev);
  void Add(epoll_event *ev);
  void ModOutput(epoll_event *ev);
};

#endif  // SRCS_EPOLL_HPP_
