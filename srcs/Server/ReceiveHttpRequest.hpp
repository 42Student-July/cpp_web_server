#ifndef SRCS_SERVER_RECEIVEHTTPREQUEST_HPP_
#define SRCS_SERVER_RECEIVEHTTPREQUEST_HPP_
#include <unistd.h>

#include <iostream>
#include <map>
#include <string>
#include <utility>
#include <vector>

#define NL "\r\n"
#define NLNL "\r\n\r\n"
#define BUFFER_SIZE 8192

typedef std::vector<std::pair<std::string, std::string> > HEADER;

enum method { ERROR, CONNECT, DELETE, GET, HEAD, OPTIONS, POST, PUT, TRACE };

enum read_stat {
  UNREAD,
  WAIT_REQUEST,
  ERROR_REQUEST,
  WAIT_HEADER,
  ERROR_HEADER,
  WAIT_BODY,
  ERROR_BODY,
  READ_COMPLETE,
  READ_ERROR
};

struct parsed_request {
  method m;
  std::string version;
  int status_code;
  std::string request_path;
  HEADER request_header;
  std::string request_body;
};

struct httprequest_data {
  read_stat s;
  std::string buf;
  std::string request_line;
  std::string request_header;
  std::string message_body;
  struct parsed_request pr;
};

class ReceiveHttpRequest {
 private:
  httprequest_data fd_data_;

 public:
  ReceiveHttpRequest();
  ReceiveHttpRequest(ReceiveHttpRequest const &other);
  ReceiveHttpRequest &operator=(ReceiveHttpRequest const &other);
  ~ReceiveHttpRequest();
  read_stat ReadHttpRequest(const int &fd, parsed_request *pr);
  void ShowParsedRequest(const int &fd);
  std::string GetBuf();
};

#endif  // SRCS_SERVER_RECEIVEHTTPREQUEST_HPP_
