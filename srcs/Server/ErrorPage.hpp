#ifndef SRCS_SERVER_ERRORPAGE_HPP_
#define SRCS_SERVER_ERRORPAGE_HPP_

#include <map>
#include <string>

#include "ServerContext.hpp"
#include "Socket.hpp"

// 仮の構造体
// Socketクラスのメンバ変数の rescode_ と body_ に相当する
// 返り値をまとめやすくするために使用する
struct HttpResponseTmp {
  ResponseCode rescode;
  std::string body;
};

class ErrorPage {
 public:
  typedef std::map<long, std::string> ErrorPageMap;

  ~ErrorPage();

  static HttpResponseTmp GetErrorPage(const ResponseCode error_code,
                                      ErrorPageMap error_pages);

 private:
  ErrorPage(ErrorPage const &other);
  ErrorPage &operator=(ErrorPage const &other);
  ErrorPage();

  static std::string GetErrorPagePath(const ResponseCode error_code,
                                      ErrorPageMap error_pages);
  static HttpResponseTmp DefaultErrorPage(const ResponseCode error_code);
  static HttpResponseTmp CustomErrorPage(const ResponseCode error_code,
                                         const std::string &path);
};

#endif  // SRCS_SERVER_ERRORPAGE_HPP_
