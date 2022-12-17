#ifndef SRCS_SERVER_GET_HPP_
#define SRCS_SERVER_GET_HPP_
#include <string>

#include "File.hpp"
#include "HttpMethod.hpp"
#include "HttpResponse.hpp"
#include "Method.hpp"
class Get : public HttpMethod {
 private:
  ResponseCode rescode_;
  // https://tex2e.github.io/rfc-translater/html/rfc9112.html#6--Message-Body
  std::string body_;

 public:
  Get();
  ~Get();
  void Run(const std::string &path, Socket *sock);
  void UpdateSocketData(Socket *sock);

  void SetResponseCode(const ResponseCode &rescode);
  void SetResponseBody(const std::string &body);
  void SetErrorPage(const ResponseCode error_code, Socket *sock);
};

#endif  // SRCS_SERVER_GET_HPP_
