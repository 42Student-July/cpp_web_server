#ifndef SRCS_SERVER_CONNECTING_HPP_
#define SRCS_SERVER_CONNECTING_HPP_

#include <string>

#include "Event.hpp"
#include "ReceiveHttpRequest.hpp"
class Connecting : public Event {
 private:
  ReceiveHttpRequest hr_;
  ParsedRequest pr_;

 public:
  Connecting(const int fd, const ServerContext& context);
  ~Connecting();
  ParsedRequest GetParsedRequest() const;
  void SetParsedRequest(const ParsedRequest& pr);
  read_stat ReadRequest();
};
#endif  // SRCS_SERVER_CONNECTING_HPP_
