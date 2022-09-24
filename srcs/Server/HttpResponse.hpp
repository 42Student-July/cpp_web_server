#ifndef SRCS_SERVER_HTTPRESPONSE_HPP_
#define SRCS_SERVER_HTTPRESPONSE_HPP_
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
class HttpResponse {
 private:
 public:
  HttpResponse();
  ~HttpResponse();

  static std::string response();
  static std::vector<std::string> make_response200(int body_length);
};

#endif  // SRCS_SERVER_HTTPRESPONSE_HPP_
