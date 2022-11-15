#ifndef SRCS_SERVER_SOCKET_HPP_
#define SRCS_SERVER_SOCKET_HPP_
#include "Fd.hpp"
#include "ServerContext.hpp"
#include "ReceiveHttpRequest.hpp"
enum SockType { LISTEN, CONNECTING };
enum SockStatus { READ, WRITE };
class Socket : public Fd {
 private:
  ServerContext context_;
  SockStatus status_;
  const SockType type_;
  ReceiveHttpRequest hr_;
  parsed_request pr_;
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
  read_stat ReadRequest();
};

#endif  // SRCS_SERVER_SOCKET_HPP_
