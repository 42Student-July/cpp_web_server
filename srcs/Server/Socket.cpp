#include "Socket.hpp"
Socket::Socket(const int fd, const std::vector<ServerContext>& context)
    : vec_context(context),
      sock_fd(fd),
      response_code(kKkNotSet),
      can_write(true) {
  server_context = context[0];
}
Socket::~Socket() {}
