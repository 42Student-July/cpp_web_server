#include "Connecting.hpp"
Connecting::Connecting(const int fd, const ServerContext& context)
    : Event(fd, context, kConnecting) {
  SetEventStatus(kRead);
}
Connecting::~Connecting() {}

ParsedRequest Connecting::GetParsedRequest() const { return pr_; }
void Connecting::SetParsedRequest(const ParsedRequest& pr) { pr_ = pr; }

void Connecting::ReadRequest() {
  ReadStat stat = hr_.ReadHttpRequest(GetFd(), &pr_);
  if (stat == kReadComplete) {
    SetEventStatus(kWrite);
  }
}

void Connecting::SetSender(const std::string& response) {
  sender_.Init(response);
}

const Sender& Connecting::GetSender() const { return sender_; }

void Connecting::SendResponse() {
  sender_.Send(GetFd());
  if (!sender_.HasMoreToSend()) {
    SetEventStatus(kDel);
  } else {
    SetEventStatus(kAginWrite);
  }
}
