#ifndef SRCS_SERVER_GET_HPP_
#define SRCS_SERVER_GET_HPP_
#include <string>

#include "File.hpp"
#include "HttpMethod.hpp"
#include "HttpResponse.hpp"
class Get : public HttpMethod {
 private:
  ResponseCode rescode_;
  // https://tex2e.github.io/rfc-translater/html/rfc9112.html#6--Message-Body
  std::string body_;
  HttpResponse::HttpHeaders headers_;

 public:
  Get();
  ~Get();
  void Run(const std::string &path, Socket *sock);
  void UpdateSocketData(Socket *sock);

  void Redirect(ResponseCode rescode, const std::string &path);
  void SetResponseCode(const ResponseCode &rescode);
  void SetResponseBody(const std::string &body);
  void SetErrorPage(const ResponseCode error_code, Socket *sock);
  void AddResponseHeader(const std::string &key, const std::string &value);
};

#endif  // SRCS_SERVER_GET_HPP_
