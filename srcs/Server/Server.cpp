#include "Server.hpp"

#include <vector>

// Server::Server(){}
Server::Server(const string p) : listen_(p) { epoll_.Init(listen_.GetFd()); }
Server::~Server() {}

void Server::Run() {
  SetNonBlocking(listen_.GetFd().GetFd());
  while (true) {
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
}

void Server::AcceptNewConnections() {
  Fd connfd(listen_.AcceptFd());
  if (connfd.GetFd() == -1) return;
  cout << "connfd :" << connfd.GetFd() << endl;
  epoll_event ev = Epoll::Create(connfd);
  epoll_.Add(&ev);
}
void Server::ReceiveRequest(epoll_event *ev) {
  int status = 0;
  Fd fd(ev->data.fd);
  if ((status = ReadRequest(fd)) == kNotDoneYet) {
    (void)status;
    return;
    // else if (status == 0){
  }

  epoll_.ModOutput(ev);
  requests_.erase(fd.GetFd());
}
void Server::SendResponse(epoll_event *ev) {
  int status = 0;
  Fd fd(ev->data.fd);
  response_[ev->data.fd];

  HttpResponse httpResponse;
  httpResponse.SetHttpResponse200();
  response_[ev->data.fd] = httpResponse.GetResponse();
  if ((status = WriteToClientFd(fd)) == kNotDoneYet) {
    (void)status;
    return;
  }
  response_.erase(ev->data.fd);
  epoll_.Del(ev);
  fd.Close();
}
int Server::WriteToClientFd(const Fd &connfd) {
  RioFileDescriptor rio(connfd);
  const int fd = connfd.GetFd();
  size_t response_size = response_[fd].size();
  for (size_t i = 0; i < response_size; i++) {
    ssize_t written_size =
        rio.WriteNbyte(response_[fd][0].c_str(), response_[fd][0].size());
    if (written_size == kNotDoneYet) {
      return kNotDoneYet;
    }
    response_[fd].erase(response_[fd].begin());
  }
  return 0;
}
int Server::ReadRequest(const Fd &fd) {
  RioFileDescriptor rio(fd);
  char buf[kMaxline];
  ssize_t read_size = 0;
  // while ((read_size = rio.ReadLineByteEach(buf, kMaxline)) > 0){
  //   requests_[fd.GetFd()].push_back(buf);
  // }
  read_size = rio.ReadLineByteEach(buf, kMaxline);
  requests_[fd.GetFd()].push_back(buf);
  for (size_t i = 0; i < requests_[fd.GetFd()].size(); i++) {
    cout << "requests_[fd.GetFd()][i]" << requests_[fd.GetFd()][i]
         << std::flush;
  }
  return read_size;
}

void Server::SetNonBlocking(const int &fd) {
  int flag = fcntl(fd, F_GETFL, 0);
  fcntl(fd, F_SETFL, flag | O_NONBLOCK);
}
