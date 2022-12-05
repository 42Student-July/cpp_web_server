#ifndef SRCS_SERVER_EPOLL_HPP_
#define SRCS_SERVER_EPOLL_HPP_
#include <string.h>
#include <sys/epoll.h>
#include <unistd.h>

#include <iostream>
#include <map>
#include <utility>
#include <vector>

#include "Fd.hpp"
#define MAX_EVENT 100
class Epoll : public Fd {
 private:
  epoll_event events_[MAX_EVENT];
  std::map<int, epoll_event> epoll_map_;

 public:
  Epoll();
  ~Epoll();
  void Create();
  void Init();
  epoll_event Find(const int &n) const;
  epoll_event FindFromFd(const int &fd) const;
  static epoll_event Create(const int connfd, uint32_t flags);
  int Wait();
  void Del(const int fd, epoll_event *ev);
  void Add(const int fd, epoll_event *ev);
  void Mod(const int fd, uint32_t flags);
};

#endif  // SRCS_SERVER_EPOLL_HPP_
