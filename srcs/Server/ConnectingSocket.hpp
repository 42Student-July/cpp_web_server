#ifndef SRCS_SERVER_CONNECTINGSOCKET_HPP_
#define SRCS_SERVER_CONNECTINGSOCKET_HPP_

#include <string>

#include "Socket.hpp"
class ConnectingSocket : public Socket {
 private:
  static const int Kbuffer_size_ = 8192;
  parsed_request parsed_request_;

 public:
  ConnectingSocket(const int fd, const ServerContext& context);
  ~ConnectingSocket();
  ssize_t Write(const char* str, size_t size);
  std::string Read();
  parsed_request GetParsedRequest() const;
  void SetParsedRequest(const parsed_request& pr);
};
#endif  // SRCS_SERVER_CONNECTINGSOCKET_HPP_
