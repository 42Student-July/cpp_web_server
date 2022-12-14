#ifndef SRCS_SERVER_HTTPRESPONSE_HPP_
#define SRCS_SERVER_HTTPRESPONSE_HPP_

#include <map>
#include <string>
#include <utility>
#include <vector>

namespace http {
std::string GetStatusMessage(const int error_code);
}  // namespace http

class HttpResponse {
 public:
  typedef std::map<std::string, std::string> HttpHeaders;

  HttpResponse();
  HttpResponse(HttpResponse const &other);
  HttpResponse &operator=(HttpResponse const &other);
  ~HttpResponse();

  void SetStatusCode(int status);
  void SetBody(std::string const &body);
  void SetHeader(std::string const &key, std::string const &value);
  void SetHeader(std::string const &key, int value);

  int GetStatusCode() const;
  std::string const &GetBody() const;
  std::string GetStatusLine() const;
  std::string GetRawResponse() const;
  void SetHttpResponse200();
  std::vector<std::string> GetResponseHeaders() const;
  void SetHttpResponse(int status_code, std::string const &body,
                       const HttpHeaders &headers);

 private:
  static const char kCRLF[3];
  int statusCode_;
  std::map<std::string, std::string> headers_;
  std::string version_;
  std::string body_;

  std::string GetStatusMessage() const;
};

#endif  // SRCS_SERVER_HTTPRESPONSE_HPP_
