#include "Epoll.hpp"
Epoll::Epoll() : epoll_(-1) {}

void Epoll::Init(const Fd& listen) {
  epoll_.Reset(epoll_create1(EPOLL_CLOEXEC));
  if (epoll_.GetFd() < 0) std::cout << "init epoll epollctl err " << std::endl;
  epoll_event event = Create(listen);
  Add(&event);
}

Epoll::~Epoll() { epoll_.Reset(-1); }

int Epoll::Wait() {
  int num_fd = epoll_wait(epoll_.GetFd(), events_, MAX_EVENT, -1);
  if (num_fd < 0) std::cout << "wait err " << std::endl;
  return num_fd;
}

epoll_event Epoll::FindEvent(const int& n) const { return events_[n]; }

epoll_event Epoll::Create(const Fd& connfd) {
  epoll_event new_event;
  memset(&new_event, 0, sizeof(epoll_event));
  new_event.events = EPOLLIN | EPOLLET;
  new_event.data.fd = connfd.GetFd();
  return new_event;
}

void Epoll::ModOutput(epoll_event* ev) {
  ev->events = EPOLLOUT | EPOLLET;
  if ((epoll_ctl(epoll_.GetFd(), EPOLL_CTL_MOD, ev->data.fd, ev)) < 0)
    std::cout << "mod err " << std::endl;
}
void Epoll::Add(epoll_event* ev) {
  if ((epoll_ctl(epoll_.GetFd(), EPOLL_CTL_ADD, ev->data.fd, ev)) < 0)
    std::cout << "add err " << std::endl;
}
void Epoll::Del(epoll_event* ev) {
  if ((epoll_ctl(epoll_.GetFd(), EPOLL_CTL_DEL, ev->data.fd, ev)) < 0)
    std::cout << "del err " << std::endl;
}
