#ifndef SRCS_SERVER_HTTPMETHOD_HPP_
#define SRCS_SERVER_HTTPMETHOD_HPP_
#include <string>

#include "ReceiveHttpRequest.hpp"
#include "Socket.hpp"
class HttpMethod {
 public:
  virtual ~HttpMethod();
  virtual void Run(const std::string &path, Socket *sock) = 0;
  virtual void UpdateSocketData(Socket *sock) = 0;
  static HttpMethod *Build(const Method &m);
};

#endif  // SRCS_SERVER_HTTPMETHOD_HPP_
