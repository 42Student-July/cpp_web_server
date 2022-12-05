#include "ResponseToTheClient.hpp"

#include "HttpProcessor.hpp"
ResponseToTheClient::ResponseToTheClient(Socket *sock)
    : socket_(sock), state_(kWrite) {}
ResponseToTheClient::~ResponseToTheClient() {
  close(socket_->sock_fd);
  delete socket_;
}
void ResponseToTheClient::Do() {
  std::cout << "response" << std::endl;
  if (state_ == kWrite) {
    HttpResponse res;
    HttpProcessor::ProcessHttpRequest(socket_->pr,
                                      socket_->server_context.locations, &res);
    sender_.Init(res.GetRawResponse());
  }
  sender_.Send(socket_->sock_fd);
  if (!sender_.HasMoreToSend())
    state_ = kDel;
  else
    state_ = kWriteAgain;
}
Event *ResponseToTheClient::NextEvent() { return NULL; }

std::pair<Event *, epoll_event> ResponseToTheClient::PublishNewEvent() {
  return std::make_pair(static_cast<Event *>(NULL), epoll_event());
}
void ResponseToTheClient::Handle(Epoll *epoll) {
  // if cgi read中かつ 返す文字列がないなら epoout外して　cgiのhandleで操作
  static_cast<void>(epoll);
  // if(socket_->can_write_)
  // else
  // epoll->events = 0;
}

EventState ResponseToTheClient::State() { return state_; }
Socket *ResponseToTheClient::GetSocket() const { return socket_; }
EventType ResponseToTheClient::Type() const { return kConn; }
