#include "CgiRead.hpp"

#include "CgiResponse.hpp"
#include "ErrorPage.hpp"
#include "ResponseToTheClient.hpp"
CgiRead::CgiRead(Socket *socket, size_t cgi_pos)
    : created_next_event_(false), socket_(socket), cgi_pos_(cgi_pos) {}

CgiRead::~CgiRead() { std::cout << "destructor cgi read" << std::endl; }
void CgiRead::Do() {
  try {
    if (socket_->CgiReadAndStoreToBuf(cgi_pos_) == -1) return;
    cgi_parser_.Parse(socket_->cgi_res[cgi_pos_].buf);
    cgi_parser_.UpdateData(socket_, cgi_pos_);
    // std::cout << "cgi read ok" << std::endl;
  } catch (ErrorResponse &e) {
    std::cout << e.Msg() << std::endl;
    HttpResponseTmp res = ErrorPage::GetErrorPage(
        e.GetErrResponseCode(), socket_->server_context.error_page);
    socket_->response_code = res.rescode;
    socket_->response_body = res.body;
  }
}
Event *CgiRead::NextEvent() { return NULL; }

std::pair<Event *, epoll_event> CgiRead::PublishNewEvent() {
  if (created_next_event_) {
    return std::make_pair(static_cast<Event *>(NULL), epoll_event());
  }
  if (socket_->response_code != kKkNotSet && !created_next_event_) {
    created_next_event_ = true;
    return std::make_pair(new ResponseToTheClient(socket_),
                          Epoll::Create(socket_->sock_fd, EPOLLOUT));
  }
  if (socket_->cgi_res[cgi_pos_].read_size <= 0 && !created_next_event_ &&
      cgi_parser_.GetResponseType() == kToBeDetermined) {
    created_next_event_ = true;
    HttpResponseTmp res = ErrorPage::GetErrorPage(
        kKk500internalServerError, socket_->server_context.error_page);
    socket_->response_code = res.rescode;
    socket_->response_body = res.body;
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
  created_next_event_ = true;
  Event *new_ev = socket_->PrepareNextEventProcess();
  epoll_event new_epo = Epoll::Create(socket_->sock_fd, EPOLLOUT);
  if (new_ev == NULL) {
    new_ev = new ResponseToTheClient(socket_);
  } else {
    new_epo = Epoll::Create(
        socket_->cgi_res[socket_->cgi_res.size() - 1].cgi_fd, EPOLLIN);
  }
  return std::make_pair(new_ev, new_epo);
}
std::pair<Event *, epoll_event> CgiRead::CreateClientEvent() {
  created_next_event_ = true;
  return std::make_pair(new CgiResponse(socket_, cgi_pos_),
                        Epoll::Create(socket_->sock_fd, EPOLLOUT));
}
void CgiRead::Handle(Epoll *epoll) {
  static_cast<void>(epoll);
  // if(created_next_event_ == false)
  //   return ;
  // if (socket_->cgi_res[cgi_pos_].read_size >= 0) {
  //   epoll->Mod(socket_->sock_fd, EPOLLOUT);
  // }
}
EventState CgiRead::State() {
  if (socket_->cgi_res[cgi_pos_].read_size == 0 ||
      socket_->response_code != kKkNotSet) {
    return kDel;
  }
  return kRead;
}
Socket *CgiRead::GetSocket() const { return socket_; }
EventType CgiRead::Type() const { return kCgiReadd; }
void CgiRead::SetSocket(Socket *socket) { socket_ = socket; }
