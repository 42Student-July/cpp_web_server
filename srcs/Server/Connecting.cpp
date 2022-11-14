#include "Connecting.hpp"
Connecting::Connecting(const int fd, const ServerContext& context)
    : Event(fd, context, CONNECTING) {}
Connecting::~Connecting() {}

parsed_request Connecting::GetParsedRequest() const { return parsed_request_; }
void Connecting::SetParsedRequest(const parsed_request& pr) {
  parsed_request_ = pr;
}
