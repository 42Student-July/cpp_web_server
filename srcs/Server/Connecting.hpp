#ifndef SRCS_SERVER_CONNECTING_HPP_
#define SRCS_SERVER_CONNECTING_HPP_

#include <string>

#include "Event.hpp"
#include "Path.hpp"
#include "ReceiveHttpRequest.hpp"
#include "Sender.hpp"

class Connecting : public Event {
 private:
  ReceiveHttpRequest hr_;
  ParsedRequest pr_;
  Path p_;
  Sender sender_;

 public:
  Connecting(const int fd, const ServerContext& context);
  ~Connecting();
  ParsedRequest GetParsedRequest() const;
  void SetParsedRequest(const ParsedRequest& pr);
  void SetSender(const std::string& response);
  const Sender& GetSender() const;
  ReadStat ReadRequest();
};
#endif  // SRCS_SERVER_CONNECTING_HPP_
