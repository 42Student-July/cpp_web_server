#ifndef SRCS_SERVER_POST_HPP_
#define SRCS_SERVER_POST_HPP_
#include <string>

#include "HttpMethod.hpp"
class Post : public HttpMethod {
 private:
  ResponseCode rescode_;
  std::string body_;
  void SetErrorPage(const ResponseCode error_code, Socket *sock);
  void SetResponseBody(const std::string &body);
  void SetResponseCode(const ResponseCode &rescode);

 public:
  Post();
  ~Post();
  void Run(const std::string &path, Socket *sock);
  void UpdateSocketData(Socket *sock);
};
#endif  // SRCS_SERVER_POST_HPP_
