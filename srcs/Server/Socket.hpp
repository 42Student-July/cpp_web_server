#ifndef SRCS_SERVER_SOCKET_HPP_
#define SRCS_SERVER_SOCKET_HPP_
#include <signal.h>
#include <string.h>
#include <sys/epoll.h>
#include <sys/wait.h>
#include <unistd.h>

#include <iostream>
#include <string>
#include <utility>
#include <vector>

#include "Event.hpp"
#include "Fd.hpp"
#include "HttpResponse.hpp"
#include "Path.hpp"
#include "ReceiveHttpRequest.hpp"
#include "ResponseCode.hpp"
#include "Sender.hpp"
#include "ServerContext.hpp"
#include "Timer.hpp"
class Event;
enum ResponseType {
  kToBeDetermined,
  kDocumentResponse,
  kLocalRedirResponse,
  kClientRedirResponse,
  kClientRediredocResponse,
  kErrType
};

struct CgiRes {
  // explicit CgiRes(int fd) : cgi_fd(fd) {}
  int buff_size;
  int written_size;
  int read_size;
  pid_t process_id;
  int pid_exit_status;
  int cgi_fd;
  ResponseType type;
  // char buf[2048];
  std::string buf;
};

class Socket {
 public:
  static const int kBuffSize = 2048;
  std::vector<ServerContext> vec_context;
  ServerContext server_context;
  LocationContext location_context;
  std::string response;
  ParsedRequest pr;

  int sock_fd;
  std::vector<std::pair<std::string, std::string> > response_headder;
  ResponseCode response_code;
  bool can_write;
  std::string response_body;
  std::vector<CgiRes> cgi_res;

  HttpResponse::HttpHeaders headers;

  std::string full_path;
  Socket(int fd, const std::vector<ServerContext>& context);
  ~Socket();
  int CgiReadAndStoreToBuf(size_t pos);
  bool CgiFinished(size_t pos);
  Event* PrepareNextEventProcess();
  void SetErrorPage(const ResponseCode error_code);
};
class ErrorResponse {
 private:
  std::string err_msg_;
  ResponseCode rescode_;

 public:
  ErrorResponse(const std::string& msg, ResponseCode rescode);
  ~ErrorResponse();
  ResponseCode GetErrResponseCode() const;
  std::string Msg() const throw();
};
#endif  // SRCS_SERVER_SOCKET_HPP_
