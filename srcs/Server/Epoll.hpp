#ifndef SRCS_SERVER_EPOLL_HPP_
#define SRCS_SERVER_EPOLL_HPP_
#include <string.h>
#include <sys/epoll.h>
#include <unistd.h>

#include <iostream>
#include <vector>

#define MAX_EVENT 100
class Epoll {
 private:
  // static const int max_event = 100;
  int epoll_fd_;
  epoll_event events_[MAX_EVENT];

 public:
  Epoll();
  ~Epoll();
  void Create();
  void Init(const int connfd);
  epoll_event FindEvent(const int &n) const;
  static epoll_event Create(const int connfd);
  int Wait();
  void Del(epoll_event *ev) const;
  void Add(epoll_event *ev) const;
  void Mod(epoll_event *ev, uint32_t flags) const;
};

#endif  // SRCS_SERVER_EPOLL_HPP_
