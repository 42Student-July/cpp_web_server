#include "CgiWrite.hpp"

#include "CgiRead.hpp"
#include "Epoll.hpp"
CgiWrite::CgiWrite(Socket *sock) : socket_(sock) {
  sender_.Init(socket_->pr.request_body);
}
CgiWrite::~CgiWrite() {}
void CgiWrite::Do() { sender_.Send(socket_->cgi_res.cgi_fd); }
Event *CgiWrite::NextEvent() {
  if (sender_.HasMoreToSend()) return NULL;
  return new CgiRead(socket_);
}
std::pair<Event *, epoll_event> CgiWrite::PublishNewEvent() {
  return std::make_pair(static_cast<Event *>(NULL), Epoll::Create(-1, 0));
}
void CgiWrite::Handle(Epoll *epoll) {
  if (!sender_.HasMoreToSend()) {
    epoll->Mod(socket_->cgi_res.cgi_fd, EPOLLIN);
  }
}
EventState CgiWrite::State() {
  if (!sender_.HasMoreToSend()) return kDel;
  return kWrite;
}
Socket *CgiWrite::GetSocket() const { return socket_; }
EventType CgiWrite::Type() const { return kCgiWrite; }
void CgiWrite::SetSocket(Socket *socket) { socket_ = socket; }
