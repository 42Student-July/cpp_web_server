#ifndef SRCS_SERVER_CONNECTING_HPP_
#define SRCS_SERVER_CONNECTING_HPP_

#include <string>

#include "Event.hpp"
#include "ReceiveHttpRequest.hpp"
class Connecting : public Event {
 private:
  ReceiveHttpRequest hr_;
  parsed_request pr_;

 public:
  Connecting(const int fd, const ServerContext& context);
  ~Connecting();
  parsed_request GetParsedRequest() const;
  void SetParsedRequest(const parsed_request& pr);
  read_stat ReadRequest();
};
#endif  // SRCS_SERVER_CONNECTING_HPP_
