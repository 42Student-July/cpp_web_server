#include "CgiResponse.hpp"
CgiResponse::CgiResponse(Socket *sock) : socket_(sock) {}
CgiResponse::~CgiResponse() {}
void CgiResponse::Do() {}
Event *CgiResponse::NextEvent() { return NULL; }
std::pair<Event *, epoll_event> CgiResponse::PublishNewEvent() {
  return std::make_pair(static_cast<Event *>(NULL), epoll_event());
}
void CgiResponse::Handle(Epoll *epoll) { static_cast<void>(epoll); }
EventState CgiResponse::State() { return kWrite; }
Socket *CgiResponse::GetSocket() const { return socket_; }
EventType CgiResponse::Type() const { return kCgiWrite; }
