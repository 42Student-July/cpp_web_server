#include "ReceveRequestFromClient.hpp"

#include "ResponseToTheClient.hpp"
ReceveRequestFromClient::ReceveRequestFromClient(Socket *sock)
    : socket_(sock) {}
ReceveRequestFromClient::~ReceveRequestFromClient() {}
void ReceveRequestFromClient::Do() {
  std::vector<ServerContext> sc;  //エラー回避のための仮置き
  stat_ = request_.ReadHttpRequest(socket_->sock_fd, &socket_->pr, sc);
  if (IsReadErr(stat_)) {
    socket_->response_code = kKk400BadRequest;
  } else if (IsReadComplete(stat_)) {
    stat_ = kReadComplete;
    // paht 探索 cgi 実行,イベントに追加 get post delete処理
    // path 探索　index くっつける
    // 通常の処理実行、cgi実行
    // delete + cgi なら消す
    // 優先度 delete > cgi > post get
  }
}
Event *ReceveRequestFromClient::NextEvent() {
  if (IsReadFinished(stat_)) {
    // std::cout << "retu response" << std::endl;
    return new ResponseToTheClient(socket_);
  }
  // std::cout << "retu NULL" << std::endl;
  return NULL;
}

std::pair<Event *, epoll_event> ReceveRequestFromClient::PublishNewEvent() {
  // if cgiなら作る
  epoll_event epo_ev;
  return std::make_pair(static_cast<Event *>(NULL), epo_ev);
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
