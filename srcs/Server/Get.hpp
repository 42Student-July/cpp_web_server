#ifndef SRCS_SERVER_GET_HPP_
#define SRCS_SERVER_GET_HPP_
#include <string>

#include "File.hpp"
#include "HttpMethod.hpp"
class Get : public HttpMethod {
 private:
  ResponseCode rescode_;
  std::string body_;

 public:
  Get();
  ~Get();
  void Run(const std::string &path, Socket *sock);
  void UpdateSocketData(Socket *sock);
};

#endif  // SRCS_SERVER_GET_HPP_
