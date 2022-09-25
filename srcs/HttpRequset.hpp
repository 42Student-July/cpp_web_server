#ifndef SRCS_HTTPREQUSET_HPP_
#define SRCS_HTTPREQUSET_HPP_
#include <iostream>
#include <string>
#include <vector>

enum method { ERROR = -1, GET, DELETE, POST };

class HttpRequest {
 private:
  method method_;
  std::string version_;
  int status_code_;
  std::string request_path_;
  std::string request_header_;
  std::string request_body_;

 public:
  HttpRequest();
  explicit HttpRequest(std::vector<std::string> *rec);
  ~HttpRequest();
  void PerseHttpRequest(std::vector<std::string> *rec);
  void SetStatusCode(const int &s);
  method GetMethod();
  std::string GetVersion();
  int GetStatusCode();
  std::string GetPath();
  std::string GetRequestHeader();
  std::string GetRequestBody();
};

#endif  // SRCS_HTTPREQUSET_HPP_
