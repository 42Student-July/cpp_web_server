#include "ResponseToTheClient.hpp"

#include "HttpProcessor.hpp"
ResponseToTheClient::ResponseToTheClient(Socket *sock)
    : socket_(sock), state_(kWrite) {}
ResponseToTheClient::~ResponseToTheClient() { delete socket_; }
void ResponseToTheClient::Do() {
  std::cout << "response" << std::endl;
  if (state_ == kWrite) {
    HttpResponse res;
    HttpProcessor::ProcessHttpRequest(socket_->pr,
                                      socket_->server_context.locations, &res);
    sender_.Init(res.GetRawResponse());
  }
  sender_.Send(socket_->sock_fd);
  if (!sender_.HasMoreToSend()) {
    std::cout << "!HasMoreToSend static response del" << std::endl;
    state_ = kDel;
  } else {
    state_ = kWriteAgain;
  }
}
Event *ResponseToTheClient::NextEvent() { return NULL; }

std::pair<Event *, epoll_event> ResponseToTheClient::PublishNewEvent() {
  return std::make_pair(static_cast<Event *>(NULL), epoll_event());
}
void ResponseToTheClient::Handle(Epoll *epoll) { static_cast<void>(epoll); }

EventState ResponseToTheClient::State() { return state_; }
Socket *ResponseToTheClient::GetSocket() const { return socket_; }
EventType ResponseToTheClient::Type() const { return kConn; }
