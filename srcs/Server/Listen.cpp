#include "Listen.hpp"
Listen::Listen() {}
Listen::Listen(const std::string &host, const std::string &port) : port_(port) {
  if (host.empty())
    host_ = "localhost";
  else
    host_ = host;
  InitSockAddr();
}
void Listen::InitSockAddr() {
  memset(&hint_, 0, sizeof(addrinfo));
  hint_.ai_family = AF_INET;
  hint_.ai_socktype = SOCK_STREAM;
  int res = getaddrinfo(host_.c_str(), port_.c_str(), &hint_, &address_);
  if (res != 0) throw std::runtime_error("getaddrinfo err");
}
Listen::~Listen() {}

int Listen::GenerateConnectableFd() {
  int listen_fd = 0;
  addrinfo *current = NULL;
  for (current = address_; current != NULL; current = current->ai_next) {
    listen_fd =
        socket(current->ai_family, current->ai_socktype, current->ai_protocol);
    if (listen_fd == -1) continue;
    if (bind(listen_fd, current->ai_addr, address_->ai_addrlen) != 0) break;
    close(listen_fd);
  }
  freeaddrinfo(address_);
  if (current == NULL) throw std::runtime_error("socket bind err");
  if (listen(listen_fd, kListenMax) == -1) {
    throw std::runtime_error("listen err");
  }
  return listen_fd;
}

// int Listen::AcceptFd() const {
//   sockaddr_storage client_addr;
//   char hostname[max_line], client_port[max_line];
//   socklen_t client_len = sizeof(sockaddr_storage);
//   int connfd = accept4(listen_fd_, reinterpret_cast<SA *>(&client_addr),
//                        &client_len, SOCK_NONBLOCK);
//   getnameinfo(reinterpret_cast<SA *>(&client_addr), client_len, hostname,
//               max_line, client_port, max_line, 0);
//   std::cout << "accepted connection from (" << hostname << ", " <<
//   client_port
//             << ")" << std::endl;
//   return connfd;
//}
