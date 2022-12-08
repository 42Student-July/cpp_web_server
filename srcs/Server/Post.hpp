#ifndef SRCS_SERVER_POST_HPP_
#define SRCS_SERVER_POST_HPP_
#include <string>

#include "Method.hpp"
class Post : public HttpMethod {
 private:
  ResponseCode rescode_;

 public:
  Post();
  ~Post();
  void Run(const std::string &path, Socket *sock);
  void UpdateSocketData(Socket *sock);
};
#endif  // SRCS_SERVER_POST_HPP_
