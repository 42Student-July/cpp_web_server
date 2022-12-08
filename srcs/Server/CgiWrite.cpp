#include "CgiWrite.hpp"

#include "CgiRead.hpp"
#include "Epoll.hpp"
CgiWrite::CgiWrite(Socket *sock) : socket_(sock) {}
CgiWrite::~CgiWrite() {}
void CgiWrite::Do() {}
Event *CgiWrite::NextEvent() {
  return NULL;  // static_cast<Event *>(new CgiRead());
}
std::pair<Event *, epoll_event> CgiWrite::PublishNewEvent() {
  return std::make_pair(static_cast<Event *>(NULL), Epoll::Create(-1, 0));
}
void CgiWrite::Handle(Epoll *epoll) { (void)epoll; }
EventState CgiWrite::State() { return kWrite; }
Socket *CgiWrite::GetSocket() const { return socket_; }
EventType CgiWrite::Type() const { return kCgiWrite; }
