#include "ReceveRequestFromClient.hpp"

#include "CgiRead.hpp"
#include "CgiWrite.hpp"
#include "Method.hpp"
#include "PrepareElementsFromRequestAndConfig.hpp"
#include "ResponseToTheClient.hpp"
ReceveRequestFromClient::ReceveRequestFromClient(Socket *sock)
    : socket_(sock), cgi_(NULL) {}
ReceveRequestFromClient::~ReceveRequestFromClient() {}
void ReceveRequestFromClient::Do() {
  stat_ = request_.ReadHttpRequest(socket_->sock_fd, &socket_->pr);
  if (IsReadErr(stat_)) {
    socket_->response_code = kKk400BadRequest;
  } else if (IsReadComplete(stat_)) {
    stat_ = kReadComplete;
    ExecMethod();
  }
}
void ReceveRequestFromClient::ExecMethod() {
  // paht 探索 index くっつけるcgi 実行,イベントに追加 get(auto indexも) post
  // redirectも delete処理　 通常の処理実行、cgi実行
  PrepareElementsFromRequestAndConfig pre(socket_->server_context, socket_->pr);
  if (!pre.RequestMethodAllowed()) {
    socket_->response_code = kKk405MethodNotAllowed;
  } else if (pre.IsRequestCgi()) {
    if (socket_->pr.request_body.empty())
      cgi_ = new CgiRead(socket_);
    else
      cgi_ = new CgiWrite(socket_);
  } else {
    HttpMethod *m = HttpMethod::Build(socket_->pr.m);
    if (m == NULL) {
      socket_->response_code = kKk501NotImplemented;
      return;
    }
    m->Run(pre.GetFullPath(), socket_);  // auto index
    m->UpdateSocketData(socket_);
  }
}
Event *ReceveRequestFromClient::NextEvent() {
  if (IsReadFinished(stat_)) {
    // std::cout << "retu response" << std::endl;
    // cgi response mo zissou
    return new ResponseToTheClient(socket_);
  }
  // std::cout << "retu NULL" << std::endl;
  return NULL;
}

std::pair<Event *, epoll_event> ReceveRequestFromClient::PublishNewEvent() {
  epoll_event epo_ev;
  if (cgi_ != NULL) {
    memset(&epo_ev, 0, sizeof(epoll_event));
    if (cgi_->Type() == kCgiWrite)
      epo_ev.events = EPOLLOUT;
    else
      epo_ev.events = EPOLLIN;
  }
  return std::make_pair(cgi_, epo_ev);
}
void ReceveRequestFromClient::Handle(Epoll *epoll) {
  if (IsReadFinished(stat_)) {
    epoll->Mod(socket_->sock_fd, EPOLLOUT);
  }
  // cgiなら in out 一旦外す
}

EventState ReceveRequestFromClient::State() {
  if (stat_ == kUnread) return kRead;
  if (IsReadComplete(stat_)) return kReadFinished;
  if (IsReadErr(stat_)) return kErr;
  return kReadAgain;
}
EventType ReceveRequestFromClient::Type() const { return kConn; }
Socket *ReceveRequestFromClient::GetSocket() const { return socket_; }
bool ReceveRequestFromClient::IsReadErr(const ReadStat &st) {
  return (st == kErrorRequest || st == kErrorBody || st == kErrorHeader ||
          st == kReadError);
}
bool ReceveRequestFromClient::IsReadAgain(const ReadStat &st) {
  return (st == kWaitRequest || st == kWaitBody || st == kWaitHeader);
}
bool ReceveRequestFromClient::IsReadFinished(const ReadStat &st) {
  return IsReadComplete(st) || IsReadErr(st);
}
bool ReceveRequestFromClient::IsReadComplete(const ReadStat &st) {
  return (st == kReadComplete);
}
