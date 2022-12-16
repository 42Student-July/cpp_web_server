#include "Listen.hpp"

Listen::Listen() {}
Listen::Listen(const std::string &host, const std::string &port)
    : host_(host), port_(port) {
  InitSockAddr();
}
void Listen::InitSockAddr() {
  memset(&addr_, 0, sizeof(struct sockaddr_in));
  addr_.sin_family = AF_INET;
  addr_.sin_port = htons((unsigned short)utils::Atoi(port_));  // to big endian
}
Listen::~Listen() {}

int Listen::GenerateConnectableFd() {
  int listen_fd = socket(addr_.sin_family, SOCK_STREAM, 0);
  if (listen_fd == -1) throw std::runtime_error("socket  err");
  if (!host_.empty()) {
    addr_.sin_addr.s_addr = inet_addr(host_.c_str());
  }
  int optval = 1;
  setsockopt(listen_fd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(int));
  if (bind(listen_fd, (const sockaddr *)&addr_, sizeof(addr_)) == -1) {
    throw std::runtime_error("bind err");
  }
  if (listen(listen_fd, kListenMax) == -1) {
    throw std::runtime_error("listen err");
  }
  return listen_fd;
}
