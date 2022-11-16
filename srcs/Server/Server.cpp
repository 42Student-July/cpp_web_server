#include "Server.hpp"
Server::Server(const std::vector<ServerContext> &contexts) {
  InitListenEvent(contexts);
}
Server::~Server() {
  for (; events_.begin() != events_.end();) {
    DelEvent(events_.begin()->second, NULL);
  }
}
void Server::InitListenEvent(const std::vector<ServerContext> &contexts) {
  for (size_t i = 0; i < contexts.size(); i++) {
    Listen listen(contexts[i].GetHost(), contexts[i].GetPort());
    const int fd = listen.GenerateConnectableFd();
    Event *sock = new ListenEvent(fd, contexts[i]);
    AddEventToMonitored(sock, EPOLLIN);
  }
}
void Server::DelEvent(const Event *sock, epoll_event *ev) {
  if (ev != 0) epoll_.Del(ev);
  events_.erase(sock->GetFd());
  delete sock;
}

void Server::Run() {
  while (true) {
    int num_event = epoll_.Wait();
    for (int i = 0; i < num_event; i++) {
      epoll_event ev = epoll_.FindEvent(i);
      ExecEvents(&ev);
      if (events_[ev.data.fd]->GetEventStatus() == DEL)
        DelEvent(events_[ev.data.fd], &ev);
    }
  }
}

void Server::ExecEvents(epoll_event *ev) {
  switch (events_[ev->data.fd]->GetEventType()) {
    case LISTEN:
      AcceptNewConnections(ev);
      break;
    case CONNECTING:
      ConnectingEvent(ev);
      break;
    case CGI:
      CgiEvent(ev);
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
  ListenEvent *sock = dynamic_cast<ListenEvent *>(events_[ev->data.fd]);
  int conn_fd = sock->Accept();
  Event *connsock = new Connecting(conn_fd, sock->GetContext());
  AddEventToMonitored(connsock, EPOLLIN);
}

// [FIXME]
void Server::ReceiveRequest(epoll_event *ev) {
  Connecting *connecting_event =
      dynamic_cast<Connecting *>(this->events_[ev->data.fd]);
  parsed_request pr = connecting_event->GetParsedRequest();
  read_stat st =
      receive_request_.ReadHttpRequest(connecting_event->GetFd(), &pr);
  (void)st;
  connecting_event->SetParsedRequest(pr);
}
void Server::AddEventToMonitored(Event *sock, uint32_t event_flag) {
  events_.insert(std::make_pair(sock->GetFd(), sock));
  epoll_event new_ev = Epoll::Create(sock->GetFd(), event_flag);
  epoll_.Add(&new_ev);
}

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

void Server::CgiRun(epoll_event *ev) {
  // Event *sock = new Cgi(fd[1]);
  // Events_.insert(std::make_pair(sock->GetFd(),sock));
  // epoll_event new_ev = Epoll::Create(sock->GetFd(), EPOLLIN);
  // epoll_.Mod(ev, 0);
  (void)ev;
}
void Server::CgiEvent(epoll_event *ev) {
  (void)ev;
  // Cgi *sock = dynamic_cast<Cgi *>(events_[ev->data.fd]);
  // sock->SetChunked(sock->Read());
  // 無限にプリント　→　時間でタイムアウト
  // 無限ループ　→　read byteが0だったらキルする
  // cgi読み込み終了したらcgi呼び出したEventのepoll writeに変更
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
