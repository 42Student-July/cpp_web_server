#include "ListenFd.hpp"
// ListenFd::ListenFd(){}
ListenFd::ListenFd(string port) : listen_(-1) {
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
  listen_.Reset(-1);
}

void ListenFd::GenerateConnectableFd() {
  addrinfo *current = NULL;
  for (current = list_top_; current != NULL; current = current->ai_next) {
    listen_.Reset(MakeSocket(current));
    if (listen_.GetFd() < 0) continue;
    SetSocketOption();
    if (CanBind(current)) break;
  }
  if (current == NULL) cout << "err1" << endl;
  if (listen(listen_.GetFd(), listen_max) < 0) cout << "err2" << endl;
}

int ListenFd::MakeSocket(addrinfo *addr) {
  return socket(addr->ai_family, addr->ai_socktype, addr->ai_protocol);
}

void ListenFd::SetSocketOption() const {
  int optval = 1;
  setsockopt(listen_.GetFd(), SOL_SOCKET, SO_REUSEADDR, (const void *)&optval,
             sizeof(int));
}

bool ListenFd::CanBind(addrinfo *addr) const {
  return bind(listen_.GetFd(), addr->ai_addr, addr->ai_addrlen) == 0;
}
Fd ListenFd::GetFd() const { return listen_; }
int ListenFd::AcceptFd() const {
  sockaddr_storage client_addr;
  char hostname[max_line], client_port[max_line];
  socklen_t client_len = sizeof(sockaddr_storage);
  int connfd = accept4(listen_.GetFd(), reinterpret_cast<SA *>(&client_addr),
                       &client_len, SOCK_NONBLOCK);
  getnameinfo(reinterpret_cast<SA *>(&client_addr), client_len, hostname,
              max_line, client_port, max_line, 0);
  cout << "accepted connection from (" << hostname << ", " << client_port << ")"
       << endl;
  return connfd;
}

bool ListenFd::IsNewConnection(const epoll_event &event) const {
  return event.data.fd == GetFd().GetFd();
}
