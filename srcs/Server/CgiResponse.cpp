#include "CgiResponse.hpp"

#include "CgiParser.hpp"
CgiResponse::CgiResponse(Socket *sock, size_t cgi_pos)
    : socket_(sock), cgi_pos_(cgi_pos) {
  if (socket_->cgi_res[cgi_pos_].type != kClientRedirResponse) {
    response_state_and_header_ = MakeResponseStatusCode();
  } else {
    response_state_and_header_ = "HTTP/1.1 302 Found\r\n";
  }
  response_state_and_header_ += MakeResponseHeader();
  if (socket_->cgi_res[cgi_pos_].type != kClientRedirResponse) {
    response_state_and_header_ +=
        "Transfer-Encoding: chunked\r\nConnection: keep-alive\r\n\r\n";
  } else {
    response_state_and_header_ += "Connection: close\r\n";
  }
  sender_.Init(response_state_and_header_);
}

std::string CgiResponse::MakeResponseStatusCode() {
  CgiParser::HeaderPair p =
      CgiParser::FindByKey(socket_->response_headder, "STATUS");
  if (p.first.empty()) {
    return "HTTP/1.1 200 OK\r\n";
  }
  return "HTTP/1.1 " + p.second + "\r\n";
}
std::string CgiResponse::MakeResponseHeader() {
  std::string res;
  for (CgiParser::HeaderVec::iterator it = socket_->response_headder.begin();
       it != socket_->response_headder.end(); it++) {
    if (it->first == "STATUS") continue;
    res += CgiParser::HeaderPairToStr(*it);
  }
  return res;
}

CgiResponse::~CgiResponse() {
  std::cout << "cgi response destructor" << std::endl;
  delete socket_;
}

void CgiResponse::Do() {
  if (sender_.HasMoreToSend()) {
    // std::cout << "send header" << std::endl;
    sender_.Send(socket_->sock_fd);
  } else if (socket_->cgi_res[cgi_pos_].type != kClientRedirResponse) {
    if (chunked_.SentByte() == socket_->response_body.size() &&
        socket_->cgi_res[cgi_pos_].read_size == 0) {
      chunked_.SendLastChunk(socket_->sock_fd);
    } else {
      if (chunked_.SentByte() >= socket_->response_body.size()) return;
      chunked_.Send(socket_->sock_fd, socket_->response_body);
    }
  }
}
Event *CgiResponse::NextEvent() { return NULL; }
std::pair<Event *, epoll_event> CgiResponse::PublishNewEvent() {
  return std::make_pair(static_cast<Event *>(NULL), epoll_event());
}
void CgiResponse::Handle(Epoll *epoll) {
  static_cast<void>(epoll);
  // if (chunked_.SentByte() >= socket_->response_body.size()) {
  //    epoll->Mod(socket_->sock_fd, 0);
  // }
}
EventState CgiResponse::State() {
  if ((chunked_.SentLastChunk() && socket_->cgi_res[cgi_pos_].read_size == 0) ||
      socket_->cgi_res[cgi_pos_].type == kClientRedirResponse ||
      ((sender_.ErrorOccured() || chunked_.WriteErr()) &&
       socket_->cgi_res[cgi_pos_].read_size == 0)) {
    return kDel;
  }
  return kWrite;
}
Socket *CgiResponse::GetSocket() const { return socket_; }
EventType CgiResponse::Type() const { return kCgiWrite; }
void CgiResponse::SetSocket(Socket *socket) { socket_ = socket; }
