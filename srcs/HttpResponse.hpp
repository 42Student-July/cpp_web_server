#ifndef SRCS_HTTPRESPONSE_HPP_
#define SRCS_HTTPRESPONSE_HPP_
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#define string std::string
class HttpResponse {
 private:
 public:
  HttpResponse();
  ~HttpResponse();

  static string response();
  static std::vector<string> make_response200(int body_length);
};

#endif  // SRCS_HTTPRESPONSE_HPP_
