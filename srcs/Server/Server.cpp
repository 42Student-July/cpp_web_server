#include "Server.hpp"
Server::Server(const std::vector<ServerContext> &contexts) {
  InitListenSocket(contexts);
  AddListenSocketsToEvents();
}
Server::~Server() {
  for (; sockets_.begin() != sockets_.end();) {
    DelSocket(sockets_.begin()->second);
  }
}
void Server::InitListenSocket(const std::vector<ServerContext> &contexts) {
  for (size_t i = 0; i < contexts.size(); i++) {
    Listen listen(contexts[i].GetHost(), contexts[i].GetPort());
    const int fd = listen.GenerateConnectableFd();
    sockets_.insert(std::make_pair(fd, new ListenSocket(fd, contexts[i])));
  }
}
void Server::AddListenSocketsToEvents() {
  for (std::map<int, Socket *>::iterator it = sockets_.begin();
       it != sockets_.end(); it++) {
    epoll_event event = Epoll::Create(it->second->GetFd(), EPOLLIN | EPOLLET);
    epoll_.Add(&event);
  }
}
void Server::DelSocket(const Socket *sock) {
  sockets_.erase(sock->GetFd());
  delete sock;
}

void Server::Run() {
  while (true) {
    int num_fd = epoll_.Wait();
    for (int i = 0; i < num_fd; i++) {
      epoll_event ev = epoll_.FindEvent(i);
      IOEvents(&ev);
    }
  }
}
void Server::IOEvents(epoll_event *ev) {
  switch (sockets_[ev->data.fd]->GetSockType()) {
    case LISTEN:
      AcceptNewConnections(ev);
      break;
    case CONNECTING:
      ConnectingEvent(ev);
      break;
  }
}

void Server::ConnectingEvent(epoll_event *ev) {
  if ((ev->events & EPOLLIN) != 0u) {
    ReceiveRequest(ev);
  } else if ((ev->events & EPOLLOUT) != 0u) {
    SendResponse(ev);
  }
}
void Server::AcceptNewConnections(epoll_event *ev) {
  ListenSocket *sock = dynamic_cast<ListenSocket *>(sockets_[ev->data.fd]);
  int conn_fd = sock->Accept();
  Socket *connsock = new ConnectingSocket(conn_fd, sock->GetContext());
  sockets_.insert(std::make_pair(conn_fd, connsock));
  epoll_event new_ev = Epoll::Create(conn_fd, EPOLLIN | EPOLLET);
  epoll_.Add(&new_ev);
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
