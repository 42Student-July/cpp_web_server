#ifndef SRCS_SERVER_HTTPRESPONSE_HPP_
#define SRCS_SERVER_HTTPRESPONSE_HPP_
#include <string>
#include <utility>
#include <vector>

class HttpResponse {
 public:
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
  std::string GetRequestLine() const;
  std::vector<std::string> GetResponse() const;
  void SetHttpResponse200();

 private:
  static const char kCRLF[3];
  int statusCode_;
  std::vector<std::pair<std::string, std::string> > headers_;
  std::string version_;
  std::string body_;

  std::string GetStatusMessage() const;
  std::vector<std::string> GetResponseHeader() const;
};

#endif  // SRCS_SERVER_HTTPRESPONSE_HPP_
