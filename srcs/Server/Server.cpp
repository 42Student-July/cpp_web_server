#include "Server.hpp"

#include <vector>

Server::Server(const std::string p) : listen_(p) {
  epoll_.Init(listen_.GetFd());
}
Server::~Server() {}

void Server::Run() {
  while (true) {
    IOEvents();
  }
}

void Server::IOEvents() {
  int num_fd = epoll_.Wait();
  for (int i = 0; i < num_fd; i++) {
    epoll_event event = epoll_.FindEvent(i);
    if (listen_.IsNewConnection(event)) {
      AcceptNewConnections();
    } else if ((event.events & EPOLLIN) != 0u) {
      ReceiveRequest(&event);
    } else if ((event.events & EPOLLOUT) != 0u) {
      SendResponse(&event);
    }
  }
}
void Server::AcceptNewConnections() {
  int conn = listen_.AcceptFd();
  if (conn == -1) return;
  std::cout << "connfd :" << conn << std::endl;
  epoll_event ev = Epoll::Create(conn);
  epoll_.Add(&ev);
}
void Server::ReceiveRequest(epoll_event *ev) { (void)ev; }

void Server::SendResponse(epoll_event *ev) {
  int status = 0;
  response_[ev->data.fd];
  HttpResponse httpResponse;
  httpResponse.SetHttpResponse200();
  response_[ev->data.fd] = httpResponse.GetResponse();
  if ((status = WriteToClientFd(ev->data.fd)) == kNotDoneYet) {
    (void)status;
    return;
  }
  response_.erase(ev->data.fd);
  epoll_.Del(ev);
  close(ev->data.fd);
}

int Server::WriteToClientFd(const int conn) {
  const int fd = conn;
  size_t response_size = response_[fd].size();
  for (size_t i = 0; i < response_size; i++) {
    ssize_t written_size =
        write(conn, response_[fd][0].c_str(), response_[fd][0].size());
    if (written_size == kNotDoneYet) {
      return kNotDoneYet;
    }
    response_[fd].erase(response_[fd].begin());
  }
  return 0;
}
