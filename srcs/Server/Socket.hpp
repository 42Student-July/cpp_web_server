#ifndef SRCS_SERVER_SOCKET_HPP_
#define SRCS_SERVER_SOCKET_HPP_
#include "Fd.hpp"
#include "ServerContext.hpp"
enum SockType { LISTEN, CONNECTING };
enum SockStatus { READ, WRITE, DELETE };
class Socket : public Fd {
 private:
  ServerContext context_;
  SockStatus status_;
  const SockType type_;
  Socket& operator=(const Socket& sock);
  Socket();
  Socket(const Socket& sock);

 public:
  Socket(const int fd, const ServerContext& context, const SockType type);
  virtual ~Socket();
  void SetSockStatus(SockStatus status);
  int GetSockStatus() const;
  int GetSockType() const;
  const ServerContext& GetContext() const;
};

#endif  // SRCS_SERVER_SOCKET_HPP_
