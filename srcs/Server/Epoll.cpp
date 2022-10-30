#include "Epoll.hpp"
Epoll::Epoll() : epoll_fd_(-1) {}

void Epoll::Init(const int listen) {
  epoll_fd_ = epoll_create1(EPOLL_CLOEXEC);
  if (epoll_fd_ < 0) std::cout << "init epoll epollctl err " << std::endl;
  epoll_event event = Create(listen);
  Add(&event);
}

Epoll::~Epoll() { close(epoll_fd_); }

int Epoll::Wait() {
  int num_fd = epoll_wait(epoll_fd_, events_, MAX_EVENT, -1);
  if (num_fd < 0) std::cout << "wait err " << std::endl;
  return num_fd;
}

epoll_event Epoll::FindEvent(const int& n) const { return events_[n]; }

epoll_event Epoll::Create(const int connfd) {
  epoll_event new_event;
  memset(&new_event, 0, sizeof(epoll_event));
  new_event.events = EPOLLIN | EPOLLET;
  new_event.data.fd = connfd;
  return new_event;
}

void Epoll::Mod(epoll_event* ev, uint32_t flags) const {
  ev->events = flags;
  if ((epoll_ctl(epoll_fd_, EPOLL_CTL_MOD, ev->data.fd, ev)) < 0)
    std::cout << "mod err " << std::endl;
}
void Epoll::Add(epoll_event* ev) const {
  if ((epoll_ctl(epoll_fd_, EPOLL_CTL_ADD, ev->data.fd, ev)) < 0)
    std::cout << "add err " << std::endl;
}
void Epoll::Del(epoll_event* ev) const {
  if ((epoll_ctl(epoll_fd_, EPOLL_CTL_DEL, ev->data.fd, ev)) < 0)
    std::cout << "del err " << std::endl;
}
