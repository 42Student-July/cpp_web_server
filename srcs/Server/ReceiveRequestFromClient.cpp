#include "ReceiveRequestFromClient.hpp"

#include "ResponseToTheClient.hpp"
ReceiveRequestFromClient::ReceiveRequestFromClient(Socket *sock)
    : socket_(sock), cgi_(NULL) {}
ReceiveRequestFromClient::~ReceiveRequestFromClient() {}
void ReceiveRequestFromClient::Do() {
  stat_ = request_.ReadHttpRequest(socket_->sock_fd, &socket_->pr,
                                   socket_->vec_context);
  if (IsReadErr(stat_)) {
    std::cout << "read error" << std::endl;
    std::cout << stat_ << std::endl;
    socket_->response_code = kKk400BadRequest;
  } else if (IsReadComplete(stat_)) {
    std::cout << "read complete" << std::endl;
    stat_ = kReadComplete;
    cgi_ = socket_->PrepareNextEventProcess();
  }
  std::cout << socket_->pr.request_path << std::endl;
}

Event *ReceiveRequestFromClient::NextEvent() {
  // if (cgi_ != NULL) {
  //   return new CgiResponse(socket_);
  // }
  if (cgi_ == NULL && IsReadFinished(stat_)) {
    // std::cout << "retu response" << std::endl;
    return new ResponseToTheClient(socket_);
  }
  return NULL;
}

std::pair<Event *, epoll_event> ReceiveRequestFromClient::PublishNewEvent() {
  epoll_event epo_ev;
  if (cgi_ != NULL) {
    if (socket_->pr.request_body.empty())
      epo_ev = Epoll::Create(socket_->cgi_res.cgi_fd, EPOLLIN);
    else
      epo_ev = Epoll::Create(socket_->cgi_res.cgi_fd, EPOLLOUT);
  }
  return std::make_pair(cgi_, epo_ev);
}
void ReceiveRequestFromClient::Handle(Epoll *epoll) {
  // cgiなら in out 一旦外す
  if (cgi_ != NULL) {
    epoll->Mod(socket_->sock_fd, 0);
  } else if (IsReadFinished(stat_)) {
    epoll->Mod(socket_->sock_fd, EPOLLOUT);
  }
}

EventState ReceiveRequestFromClient::State() {
  if (stat_ == kUnread) return kRead;
  if (IsReadComplete(stat_)) return kReadFinished;
  if (IsReadErr(stat_)) return kErr;
  return kReadAgain;
}
EventType ReceiveRequestFromClient::Type() const { return kConn; }
Socket *ReceiveRequestFromClient::GetSocket() const { return socket_; }
bool ReceiveRequestFromClient::IsReadErr(const ReadStat &st) {
  return (st == kErrorRequest || st == kErrorBody || st == kErrorHeader ||
          st == kReadError);
}
bool ReceiveRequestFromClient::IsReadAgain(const ReadStat &st) {
  return (st == kWaitRequest || st == kWaitBody || st == kWaitHeader);
}
bool ReceiveRequestFromClient::IsReadFinished(const ReadStat &st) {
  return IsReadComplete(st) || IsReadErr(st);
}
bool ReceiveRequestFromClient::IsReadComplete(const ReadStat &st) {
  return (st == kReadComplete);
}
