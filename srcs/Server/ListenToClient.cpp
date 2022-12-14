#include "ListenToClient.hpp"

#include <sys/socket.h>

#include "Epoll.hpp"
#include "ReceiveRequestFromClient.hpp"
ListenToClient::ListenToClient(const int fd,
                               const std::vector<ServerContext>& context)
    : listen_fd_(fd), context_(context), conn_fd_(-1), socket_(NULL) {}
ListenToClient::~ListenToClient() {}
// int ListenEvent::Accept() {
//   int conn = accept(GetFd(), NULL, NULL);
//   if (conn == -1) throw std::runtime_error("accept");
//   std::cout << "new connecting: " << conn << std::endl;
//   return conn;
// }
void ListenToClient::Do() {
  conn_fd_ = accept(listen_fd_, NULL, NULL);
  if (conn_fd_ == -1) throw std::runtime_error("accept");
  std::cout << "new connecting: " << conn_fd_ << std::endl;
}
Event* ListenToClient::NextEvent() { return NULL; }
EventState ListenToClient::State() { return kRead; }
Socket* ListenToClient::GetSocket() const { return socket_; }
EventType ListenToClient::Type() const { return kListen; }

std::pair<Event*, epoll_event> ListenToClient::PublishNewEvent() {
  socket_ = new Socket(conn_fd_, context_);
  return std::make_pair(new ReceiveRequestFromClient(socket_),
                        Epoll::Create(conn_fd_, EPOLLIN));  // recive request
}
void ListenToClient::Handle(Epoll* epoll) { static_cast<void>(epoll); }
int ListenToClient::NewEventFd() const { return conn_fd_; }
