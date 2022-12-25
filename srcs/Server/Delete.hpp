#ifndef SRCS_SERVER_DELETE_HPP_
#define SRCS_SERVER_DELETE_HPP_
#include <string>

#include "HttpMethod.hpp"
class Delete : public HttpMethod {
 private:
  ResponseCode rescode_;
  std::string body_;
  void SetErrorPage(const ResponseCode error_code, Socket *sock);
  void SetResponseBody(const std::string &body);
  void SetResponseCode(const ResponseCode &rescode);

 public:
  Delete();
  ~Delete();
  void Run(const std::string &path, Socket *sock);
  void UpdateSocketData(Socket *sock);
};
#endif  // SRCS_SERVER_DELETE_HPP_
