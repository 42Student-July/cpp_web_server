#include "Socket.hpp"
Socket::Socket(const int fd, const ServerContext& context)
    : server_context(context),
      sock_fd(fd),
      response_code(kKkNotSet),
      can_write(true) {}
Socket::~Socket() {}
