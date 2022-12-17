#ifndef SRCS_SERVER_SOCKET_HPP_
#define SRCS_SERVER_SOCKET_HPP_
#include <signal.h>
#include <string.h>
#include <sys/epoll.h>
#include <sys/wait.h>
#include <unistd.h>

#include <iostream>
#include <string>
#include <vector>

#include "Fd.hpp"
#include "ServerContext.hpp"
#include "Timer.hpp"
// #include "Event.hpp"
#include "HttpResponse.hpp"
#include "Path.hpp"
#include "ReceiveHttpRequest.hpp"
#include "Sender.hpp"

enum ResponseCode {
  kKk200Ok = 200,
  kKk201Created = 201,
  kKk204NoContent = 204,
  kKk400BadRequest = 400,
  kKk401Unauthorized = 401,
  kKk403Forbidden = 403,
  kKk404NotFound = 404,
  kKk405MethodNotAllowed = 405,
  kKk413RequestEntityTooLarge = 413,
  kKk429TooManyRequest = 429,
  kKk500internalServerError = 500,
  kKk501NotImplemented = 501,
  kKk503ServerUnavailable = 503,
  kKkNotSet = 0
};
struct CgiRes {
  int buff_size;
  int written_size;
  int read_size;
  pid_t process_id;
  int cgi_fd;
  char buf[2048];
};

class Socket {
 private:
 public:
  std::vector<ServerContext> vec_context;
  ServerContext server_context;
  LocationContext location_context;
  std::string reesponse;
  ParsedRequest pr;
  // Timer timer_;
  // epoll_event epo_ev;
  int sock_fd;
  // Socket *two_way;
  ResponseCode response_code;
  bool can_write;
  std::string response_body;
  CgiRes cgi_res;
  std::string full_path;
  Socket(int fd, const std::vector<ServerContext>& context);
  ~Socket();
};

#endif  // SRCS_SERVER_SOCKET_HPP_
