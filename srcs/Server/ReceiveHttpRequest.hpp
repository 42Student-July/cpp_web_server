#ifndef SRCS_SERVER_RECEIVEHTTPREQUEST_HPP_
#define SRCS_SERVER_RECEIVEHTTPREQUEST_HPP_
#include <unistd.h>

#include <algorithm>
#include <iostream>
#include <map>
#include <string>
#include <utility>
#include <vector>

#define NL "\r\n"
#define NLNL "\r\n\r\n"
#define BUFFER_SIZE 8192

typedef std::vector<std::pair<std::string, std::string> > Header;

enum Method {
  kError,
  kConnect,
  kDelete,
  kGet,
  kHead,
  kOptions,
  kPost,
  kPut,
  kTrace
};

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

class ReceiveHttpRequest {
 private:
  HttpRequestData fd_data_;

 public:
  ReceiveHttpRequest();
  ReceiveHttpRequest(ReceiveHttpRequest const &other);
  ReceiveHttpRequest &operator=(ReceiveHttpRequest const &other);
  ~ReceiveHttpRequest();
  ReadStat ReadHttpRequest(const int &fd, ParsedRequest *pr);
  void ShowParsedRequest(const int &fd);
  std::string GetBuf();
  ParsedRequest GetParsedRequest() const;
};

#endif  // SRCS_SERVER_RECEIVEHTTPREQUEST_HPP_
