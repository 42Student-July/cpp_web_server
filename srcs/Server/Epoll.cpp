#include "Epoll.hpp"
Epoll::Epoll() : Fd(-1) { Init(); }

void Epoll::Init() {
  Reset(epoll_create1(EPOLL_CLOEXEC));
  if (GetFd() < 0) throw std::runtime_error("epoll create1 err ");
}

Epoll::~Epoll() { Close(); }

int Epoll::Wait() {
  int num_fd = epoll_wait(GetFd(), events_, MAX_EVENT, -1);
  if (num_fd < 0) std::cout << "wait err " << std::endl;
  return num_fd;
}

epoll_event Epoll::FindEvent(const int& n) const { return events_[n]; }

epoll_event Epoll::Create(const int conn_fd, uint32_t flag) {
  epoll_event new_event;
  memset(&new_event, 0, sizeof(epoll_event));
  new_event.events = flag;
  new_event.data.fd = conn_fd;
  return new_event;
}

void Epoll::Mod(epoll_event* ev, uint32_t flags) const {
  ev->events = flags;
  if ((epoll_ctl(GetFd(), EPOLL_CTL_MOD, ev->data.fd, ev)) < 0)
    throw std::runtime_error("epoll ctl mod err ");
}
void Epoll::Add(epoll_event* ev) const {
  if ((epoll_ctl(GetFd(), EPOLL_CTL_ADD, ev->data.fd, ev)) < 0)
    throw std::runtime_error("epoll ctl add err ");
}
void Epoll::Del(epoll_event* ev) const {
  if ((epoll_ctl(GetFd(), EPOLL_CTL_DEL, ev->data.fd, ev)) < 0)
    throw std::runtime_error("epoll ctl del err");
}
