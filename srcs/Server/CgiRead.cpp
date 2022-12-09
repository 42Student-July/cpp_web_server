#include "CgiRead.hpp"
CgiRead::CgiRead(Socket *socket) : socket_(socket) {}

CgiRead::~CgiRead() {}
void CgiRead::Do() {
  if ((socket_->cgi_res.read_size = read(
           socket_->cgi_res.cgi_fd, socket_->cgi_res.buf, kBuffSize)) == -1)
    std::cout << "cgi read err" << std::endl;
  // int flag;
  // if(waitpid(socket_->cgi_res.process_id,&flag,WNOHANG) == -1){
  //   socket_->response_code = kKk500internalServerError;
  // }
  // 子プロが終了してたらflag立てる
}
Event *CgiRead::NextEvent() { return NULL; }
std::pair<Event *, epoll_event> CgiRead::PublishNewEvent() {
  return std::make_pair(static_cast<Event *>(NULL), epoll_event());
}
void CgiRead::Handle(Epoll *epoll) {
  if (socket_->cgi_res.read_size > 0) {
    epoll->Mod(socket_->sock_fd, EPOLLOUT);
  }
}
EventState CgiRead::State() { return kRead; }
Socket *CgiRead::GetSocket() const { return socket_; }
EventType CgiRead::Type() const { return kCgiReadd; }
