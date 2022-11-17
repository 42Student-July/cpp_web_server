#include "Connecting.hpp"
Connecting::Connecting(const int fd, const ServerContext& context)
    : Event(fd, context, CONNECTING) {}
Connecting::~Connecting() {}

parsed_request Connecting::GetParsedRequest() const { return pr_; }
void Connecting::SetParsedRequest(const parsed_request& pr) { pr_ = pr; }

read_stat Connecting::ReadRequest() {
  return (hr_.ReadHttpRequest(GetFd(), &pr_));
}
