#ifndef SRCS_SERVER_SOCKET_HPP_
#define SRCS_SERVER_SOCKET_HPP_
#include <signal.h>
#include <string.h>
#include <sys/epoll.h>
#include <sys/wait.h>
#include <unistd.h>

#include <iostream>
#include <string>

#include "Fd.hpp"
#include "ServerContext.hpp"
#include "Timer.hpp"
// #include "Event.hpp"
#include "HttpResponse.hpp"
#include "Path.hpp"
#include "ReceiveHttpRequest.hpp"
#include "Sender.hpp"

enum ResponseCode {
  kKk200Ok,
  kKk204NoContent,
  kKk400BadRequest,
  kKk401Unauthorized,
  kKk403Forbidden,
  kKk404NotFound,
  kKk405MethodNotAllowed,
  kKk413RequestEntityTooLarge,
  kKk429TooManyRequest,
  kKk500internalServerRequestsError,
  kKk501NotImplemented,
  kKk503ServerUnavailable,
  kKkNotSet
};
// struct CgiRes {pid_t process_id; std::string chunked;};

class Socket {
 private:
 public:
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
  Socket(int fd, const ServerContext& context);
  ~Socket();
};

#endif  // SRCS_SERVER_SOCKET_HPP_
