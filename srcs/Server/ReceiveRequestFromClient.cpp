#include "ReceiveRequestFromClient.hpp"

#include "ErrorPage.hpp"
#include "ResponseToTheClient.hpp"
ReceiveRequestFromClient::ReceiveRequestFromClient(Socket *sock)
    : socket_(sock), cgi_(NULL), del_(false) {}
ReceiveRequestFromClient::~ReceiveRequestFromClient() {}
void ReceiveRequestFromClient::Do() {
  try {
    stat_ = request_.ReadHttpRequest(socket_->sock_fd, &socket_->pr,
                                     socket_->vec_context);
    if (IsReadComplete(stat_)) {
      socket_->server_context = request_.GetSelectedSercerContext();
      std::cout << "read complete" << std::endl;
      cgi_ = socket_->PrepareNextEventProcess();
    }
  } catch (ErrorResponse &e) {
    del_ = true;
    std::cout << "read err" << std::endl;
    HttpResponseTmp res = ErrorPage::GetErrorPage(
        e.GetErrResponseCode(), socket_->server_context.error_page);
    socket_->response_code = res.rescode;
    socket_->response_body = res.body;
  }
}

Event *ReceiveRequestFromClient::NextEvent() {
  if (cgi_ == NULL && (IsReadComplete(stat_) || del_)) {
    return new ResponseToTheClient(socket_);
  }
  return NULL;
}

std::pair<Event *, epoll_event> ReceiveRequestFromClient::PublishNewEvent() {
  epoll_event epo_ev;
  if (cgi_ != NULL) {
    if (socket_->pr.request_body.empty())
      epo_ev = Epoll::Create(
          socket_->cgi_res[socket_->cgi_res.size() - 1].cgi_fd, EPOLLIN);
    else
      epo_ev = Epoll::Create(
          socket_->cgi_res[socket_->cgi_res.size() - 1].cgi_fd, EPOLLOUT);
  }
  return std::make_pair(cgi_, epo_ev);
}
void ReceiveRequestFromClient::Handle(Epoll *epoll) {
  if (IsReadComplete(stat_) || del_) {
    epoll->Mod(socket_->sock_fd, EPOLLOUT);
  }
}

EventState ReceiveRequestFromClient::State() {
  if (cgi_ != NULL) return kDel;
  if (stat_ == kReadNoRequest) return kReadDisconnect;
  if (IsReadComplete(stat_)) return kReadFinished;
  if (del_) return kErr;
  return kReadAgain;
}
EventType ReceiveRequestFromClient::Type() const { return kConn; }
Socket *ReceiveRequestFromClient::GetSocket() const { return socket_; }
void ReceiveRequestFromClient::SetSocket(Socket *socket) { socket_ = socket; }

bool ReceiveRequestFromClient::IsReadAgain(const ReadStat &st) {
  return (st == kWaitRequest || st == kWaitBody || st == kWaitHeader);
}

bool ReceiveRequestFromClient::IsReadComplete(const ReadStat &st) {
  return (st == kReadComplete);
}
