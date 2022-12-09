#ifndef SRCS_SERVER_RECEIVEHTTPREQUEST_HPP_
#define SRCS_SERVER_RECEIVEHTTPREQUEST_HPP_
#include <unistd.h>

#include <algorithm>
#include <iostream>
#include <map>
#include <string>
#include <utility>
#include <vector>

#include "ServerContext.hpp"

#define NL "\r\n"
#define NLNL "\r\n\r\n"
#define BUFFER_SIZE 8192

typedef std::vector<std::pair<std::string, std::string> > Header;

enum ReadStat {
  kUnread,
  kWaitRequest,
  kErrorRequest,
  kWaitHeader,
  kErrorHeader,
  kWaitBody,
  kErrorBody,
  kReadComplete,
  kReadError
};

struct ParsedRequest {
  Method m;
  std::string version;
  int status_code;
  std::string request_path;
  Header request_header;
  std::string request_body;
  std::string query_string;
};

struct HttpRequestData {
  ReadStat s;
  std::string buf;
  std::string request_line;
  std::string request_header;
  std::string message_body;
  bool is_chunked;
  struct ParsedRequest pr;
};
class ServerContext;
class ReceiveHttpRequest {
 private:
  HttpRequestData fd_data_;
  ServerContext *sc_;

  class SearchValueByKey {
    std::string Key_;

   public:
    explicit SearchValueByKey(std::string key) : Key_(key) {}
    bool operator()(const std::pair<std::string, std::string> pair) const {
      return Key_ == pair.first;
    }
  };

 public:
  ReceiveHttpRequest();
  ReceiveHttpRequest(ReceiveHttpRequest const &other);
  ReceiveHttpRequest &operator=(ReceiveHttpRequest const &other);
  ~ReceiveHttpRequest();
  ReadStat ReadHttpRequest(const int &fd, ParsedRequest *pr,
                           std::vector<ServerContext> sc);
  void ShowParsedRequest(const int &fd);
  std::string GetBuf();
  ParsedRequest GetParsedRequest() const;
  ServerContext &SelectServerContext(
      std::vector<ServerContext> *contexts) const;
  std::string &GetValueByKey(const std::string &key) const;
};

#endif  // SRCS_SERVER_RECEIVEHTTPREQUEST_HPP_
