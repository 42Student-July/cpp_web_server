#include "CgiRead.hpp"
CgiRead::CgiRead(Socket *socket) : socket_(socket) {}
CgiRead::CgiRead(Event *cgi_write) { (void)cgi_write; }
CgiRead::~CgiRead() {}
void CgiRead::Do() {}
Event *CgiRead::NextEvent() { return NULL; }
std::pair<Event *, epoll_event> CgiRead::PublishNewEvent() {
  return std::make_pair(static_cast<Event *>(NULL), epoll_event());
}
void CgiRead::Handle(Epoll *epoll) { (void)epoll; }
EventState CgiRead::State() { return kCgiReadAgain; }
Socket *CgiRead::GetSocket() const { return socket_; }
EventType CgiRead::Type() const { return kCgiReadd; }
