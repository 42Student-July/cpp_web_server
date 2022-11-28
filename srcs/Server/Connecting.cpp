#include "Connecting.hpp"
Connecting::Connecting(const int fd, const ServerContext& context)
    : Event(fd, context, kConnecting) {}
Connecting::~Connecting() {}

ParsedRequest Connecting::GetParsedRequest() const { return pr_; }
void Connecting::SetParsedRequest(const ParsedRequest& pr) { pr_ = pr; }

void Connecting::ReadRequest() {
  ReadStat stat = hr_.ReadHttpRequest(GetFd(), &pr_);
  if (stat == kReadComplete) {
    SetEventStatus(kWrite);
  }
}
