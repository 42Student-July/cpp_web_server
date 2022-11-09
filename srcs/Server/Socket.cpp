#include "Socket.hpp"
Socket::Socket(const int fd, const ServerContext& context, const SockType type)
    : Fd(fd), context_(context), type_(type) {}
Socket::~Socket() { Close(); }
void Socket::SetSockStatus(SockStatus status) { status_ = status; }
int Socket::GetSockStatus() const { return status_; }
int Socket::GetSockType() const { return type_; }
const ServerContext& Socket::GetContext() const { return context_; }
