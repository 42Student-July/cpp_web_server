
#include "Epoll.hpp"
Epoll::Epoll() : Fd(-1) { Init(); }

void Epoll::Init() {
  Reset(epoll_create1(EPOLL_CLOEXEC));
  if (GetFd() < 0) throw std::runtime_error("epoll create1 err ");
}

Epoll::~Epoll() { Close(); }

int Epoll::Wait() {
  int num_fd = epoll_wait(GetFd(), events_, MAX_EVENT, 1000);
  // if (num_fd < 0) std::cout << "wait err " << std::endl;
  return num_fd;
}

epoll_event Epoll::Find(const int& n) const { return events_[n]; }
epoll_event Epoll::FindFromFd(const int& fd) const {
  std::map<int, epoll_event>::const_iterator it = epoll_map_.find(fd);
  if (it == epoll_map_.end()) throw EpollErr("find from fd err", fd);
  return it->second;
}
epoll_event Epoll::Create(const int conn_fd, uint32_t flag) {
  epoll_event new_event;
  memset(&new_event, 0, sizeof(epoll_event));
  new_event.events = flag;
  new_event.data.fd = conn_fd;
  return new_event;
}

void Epoll::Mod(const int fd, uint32_t flags) {
  epoll_event ev = FindFromFd(fd);
  ev.events = flags;
  epoll_map_[fd] = ev;
  if ((epoll_ctl(GetFd(), EPOLL_CTL_MOD, fd, &ev)) < 0)
    throw EpollErr("epoll ctl mod err", fd);
}
void Epoll::Add(const int fd, epoll_event* ev) {
  epoll_map_.insert(std::make_pair(fd, *ev));
  if ((epoll_ctl(GetFd(), EPOLL_CTL_ADD, fd, ev)) < 0)
    throw EpollErr("epoll ctl add err", fd);
}
void Epoll::Del(const int fd, epoll_event* ev) {
  epoll_map_.erase(fd);
  if ((epoll_ctl(GetFd(), EPOLL_CTL_DEL, fd, ev)) < 0)
    throw EpollErr("epoll ctl del err", fd);
}

EpollErr::EpollErr(const std::string msg, int fd) : fd_(fd), msg_(msg) {}
EpollErr::~EpollErr() {}
int EpollErr::GetFd() const { return fd_; }
std::string EpollErr::Msg() const { return msg_; }
