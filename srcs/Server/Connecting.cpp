#include "Connecting.hpp"
Connecting::Connecting(const int fd, const ServerContext& context)
    : Event(fd, context, kConnecting) {}
Connecting::~Connecting() {}

ParsedRequest Connecting::GetParsedRequest() const { return pr_; }
void Connecting::SetParsedRequest(const ParsedRequest& pr) { pr_ = pr; }

ReadStat Connecting::ReadRequest() {
  return (hr_.ReadHttpRequest(GetFd(), &pr_));
}

void Connecting::SetSender(const std::string& response) {
  sender_.Init(response);
}

Sender Connecting::GetSender() const { return sender_; }
