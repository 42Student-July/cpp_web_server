#include "CgiRead.hpp"

#include "CgiResponse.hpp"
#include "ResponseToTheClient.hpp"
CgiRead::CgiRead(Socket *socket)
    : created_next_event_(false), socket_(socket), del_(false) {}

CgiRead::~CgiRead() { std::cout << "destructor cgi read" << std::endl; }
void CgiRead::Do() {
  try {
    if (socket_->CgiReadAndStoreToBuf() == -1) return;
    cgi_parser_.Parse(socket_->cgi_res.buf);
    cgi_parser_.UpdateData(socket_);
    std::cout << "cgi read ok" << std::endl;
  } catch (ErrorResponse &e) {
    std::cout << e.Msg() << std::endl;
    socket_->response_code = e.GetErrResponseCode();
  }
}
Event *CgiRead::NextEvent() { return NULL; }

std::pair<Event *, epoll_event> CgiRead::PublishNewEvent() {
  if (created_next_event_) {
    return std::make_pair(static_cast<Event *>(NULL), epoll_event());
  }
  created_next_event_ = true;
  if (socket_->response_code != kKkNotSet) {
    return std::make_pair(new ResponseToTheClient(socket_),
                          Epoll::Create(socket_->sock_fd, EPOLLOUT));
  }
  switch (cgi_parser_.GetResponseType()) {
    case kDocumentResponse:
      return CreateClientEvent();
    case kClientRedirResponse:
      return CreateClientEvent();
    case kClientRediredocResponse:
      return CreateClientEvent();
    case kLocalRedirResponse:
      return CreateLocalRedirEvent();
    default:
      return std::make_pair(static_cast<Event *>(NULL), epoll_event());
  }
  return std::make_pair(static_cast<Event *>(NULL), epoll_event());
}
std::pair<Event *, epoll_event> CgiRead::CreateLocalRedirEvent() {
  Event *new_ev = socket_->PrepareNextEventProcess();
  epoll_event new_epo = Epoll::Create(socket_->sock_fd, EPOLLOUT);
  del_ = true;
  if (new_ev == NULL) {
    new_ev = new ResponseToTheClient(socket_);
  } else {
    std::cout << "localredirevent cgi" << std::endl;
    new_epo = Epoll::Create(socket_->cgi_res.cgi_fd, EPOLLIN);
  }
  return std::make_pair(new_ev, new_epo);
}
std::pair<Event *, epoll_event> CgiRead::CreateClientEvent() const {
  return std::make_pair(new CgiResponse(socket_),
                        Epoll::Create(socket_->sock_fd, EPOLLOUT));
}
void CgiRead::Handle(Epoll *epoll) {
  static_cast<void>(epoll);
  // if(created_next_event_ == false)
  //   return ;
  // if (socket_->cgi_res.read_size >= 0) {
  //   epoll->Mod(socket_->sock_fd, EPOLLOUT);
  // }
}
EventState CgiRead::State() {
  if (socket_->cgi_res.read_size <= 0) {
    return kDel;
  }
  return kRead;
}
Socket *CgiRead::GetSocket() const { return socket_; }
EventType CgiRead::Type() const { return kCgiReadd; }
void CgiRead::SetSocket(Socket *socket) { socket_ = socket; }
