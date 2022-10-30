#include "ListenFd.hpp"
// ListenFd::ListenFd(){}
ListenFd::ListenFd(std::string port) : listen_fd_(-1) {
  addrinfo hint;
  init_hint(&hint);
  getaddrinfo(NULL, port.c_str(), &hint, &list_top_);
  GenerateConnectableFd();
}
// helper
void ListenFd::init_hint(addrinfo *hint) {
  memset(hint, 0, sizeof(addrinfo));
  hint->ai_socktype = SOCK_STREAM;
  hint->ai_flags = AI_PASSIVE | AI_ADDRCONFIG;
  hint->ai_flags |= AI_NUMERICSERV;
  // 通信プロトコル TCP
  // service引数がポート番号であることを強制させる
  // passive 待ち受け状態のソケットとしてソケットアドレスを返す
  // addrconfigローカルホストの設定をみてipアドレスをあわせる
}

ListenFd::~ListenFd() {
  freeaddrinfo(list_top_);
  close(listen_fd_);
}

// int ListenFd::SetupSocket() {
// }

void ListenFd::GenerateConnectableFd() {
  addrinfo *current = NULL;
  for (current = list_top_; current != NULL; current = current->ai_next) {
    listen_fd_ = MakeSocket(current);
    if (listen_fd_ < 0) continue;
    SetSocketOption();
    if (CanBind(current)) break;
    close(listen_fd_);
  }
  if (current == NULL) std::cout << "err1" << std::endl;
  if (listen(listen_fd_, listen_max) < 0)
    throw std::runtime_error("listen error");
}

int ListenFd::MakeSocket(addrinfo *addr) {
  return socket(addr->ai_family, addr->ai_socktype, addr->ai_protocol);
}

void ListenFd::SetSocketOption() const {
  int optval = 1;
  setsockopt(listen_fd_, SOL_SOCKET, SO_REUSEADDR, (const void *)&optval,
             sizeof(int));
}

bool ListenFd::CanBind(addrinfo *addr) const {
  return bind(listen_fd_, addr->ai_addr, addr->ai_addrlen) == 0;
}
int ListenFd::GetFd() const { return listen_fd_; }
int ListenFd::AcceptFd() const {
  sockaddr_storage client_addr;
  char hostname[max_line], client_port[max_line];
  socklen_t client_len = sizeof(sockaddr_storage);
  int connfd = accept4(listen_fd_, reinterpret_cast<SA *>(&client_addr),
                       &client_len, SOCK_NONBLOCK);
  getnameinfo(reinterpret_cast<SA *>(&client_addr), client_len, hostname,
              max_line, client_port, max_line, 0);
  std::cout << "accepted connection from (" << hostname << ", " << client_port
            << ")" << std::endl;
  return connfd;
}

bool ListenFd::IsNewConnection(const epoll_event &event) const {
  return event.data.fd == listen_fd_;
}
