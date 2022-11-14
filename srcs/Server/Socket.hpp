#ifndef SRCS_SERVER_SOCKET_HPP_
#define SRCS_SERVER_SOCKET_HPP_
#include "Fd.hpp"
#include "ServerContext.hpp"

#define NL "\r\n"
#define NLNL "\r\n\r\n"
#define BUFFER_SIZE 8192
typedef std::vector<std::pair<std::string, std::string> > HEADER;

enum SockType { LISTEN, CONNECTING };
enum SockStatus { READ, WRITE };
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

class Socket : public Fd {
 private:
  ServerContext context_;
  SockStatus status_;
  const SockType type_;
  Socket& operator=(const Socket& sock);
  Socket();
  Socket(const Socket& sock);
  parsed_request pr_;
  httprequest_data hd_;

 public:
  Socket(const int fd, const ServerContext& context, const SockType type);
  virtual ~Socket();
  void SetSockStatus(SockStatus status);
  int GetSockStatus() const;
  int GetSockType() const;
  const ServerContext& GetContext() const;
  read_stat ReadHttpRequest(int fd);
  parsed_request GetParsedRequest();
};

#endif  // SRCS_SERVER_SOCKET_HPP_
