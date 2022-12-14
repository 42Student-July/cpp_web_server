#include "CgiRead.hpp"

#include "CgiResponse.hpp"
#include "ResponseToTheClient.hpp"
CgiRead::CgiRead(Socket *socket)
    : created_next_event_(false), socket_(socket) {}

CgiRead::~CgiRead() {}
void CgiRead::Do() {
  try {
    socket_->CgiReadAndWaitPid();
    cgi_parser_.Parse(socket_->cgi_res.buf);
    cgi_parser_.UpdateData(socket_);
  } catch (...) {
    socket_->response_code = kKk500internalServerError;
  }
}
Event *CgiRead::NextEvent() { return NULL; }

std::pair<Event *, epoll_event> CgiRead::PublishNewEvent() {
  ResponseType res = cgi_parser_.GetResponseType();
  Event *new_ev = NULL;
  epoll_event new_epo = Epoll::Create(socket_->sock_fd, EPOLLOUT);
  if (res == kToBeDetermined || created_next_event_)
    return std::make_pair(new_ev, new_epo);
  if (socket_->response_code != kKkNotSet) {
    created_next_event_ = true;
    new_ev = new ResponseToTheClient(socket_);
  } else if (res == kDocumentResponse || res == kClientRedirResponse ||
             res == kClientRediredocResponse) {
    created_next_event_ = true;
    new_ev = new CgiResponse(socket_);
  } else if (res == kLocalRedirResponse) {
    created_next_event_ = true;
    if (!socket_->CgiFinished()) {
      socket_->response_code = kKk500internalServerError;
      new_ev = new ResponseToTheClient(socket_);
    } else {
      Event *cgi = socket_->PrepareNextEventProcess();
      if (cgi == NULL) {
        new_ev = new ResponseToTheClient(socket_);
      } else {
        new_ev = new CgiRead(socket_);
        new_epo = Epoll::Create(socket_->sock_fd, EPOLLIN);
      }
    }
  }
  return std::make_pair(new_ev, new_epo);
}
void CgiRead::Handle(Epoll *epoll) {
  if (socket_->cgi_res.read_size > 0 || socket_->CgiFinished()) {
    epoll->Mod(socket_->sock_fd, EPOLLOUT);
  }
}
EventState CgiRead::State() {
  if (socket_->CgiFinished()) return kDel;
  return kRead;
}
Socket *CgiRead::GetSocket() const { return socket_; }
EventType CgiRead::Type() const { return kCgiReadd; }
