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
    for (int i = 0; i < epoll_.Wait(); i++) {
      epoll_event ev = epoll_.FindEvent(i);
      ExecEvents(&ev);
      if (events_[ev.data.fd]->GetEventStatus() == kDel)
        DelEvent(events_[ev.data.fd], &ev);
    }
  }
}

void Server::ExecEvents(epoll_event *ev) {
  switch (events_[ev->data.fd]->GetEventType()) {
    case kListen:
      AcceptNewConnections(ev);
      break;
    case kConnecting:
      ConnectingEvent(ev);
      break;
    case kCgi:
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
void Server::ReceiveRequest(epoll_event *epo_ev) {
  Connecting *conn = dynamic_cast<Connecting *>(events_[epo_ev->data.fd]);
  conn->ReadRequest();
  if (conn->GetEventStatus() == kWrite) {
    epoll_.Mod(epo_ev, EPOLLOUT);
  }
  if (conn->GetEventStatus() == kCgi) {
    // CgiRun(epo_ev);
    epoll_.Mod(epo_ev, 0);
  }
}
void Server::AddEventToMonitored(Event *sock, uint32_t event_flag) {
  events_.insert(std::make_pair(sock->GetFd(), sock));
  epoll_event new_ev = Epoll::Create(sock->GetFd(), event_flag);
  epoll_.Add(&new_ev);
}

void Server::SendResponse(epoll_event *ev) {
  Connecting *conn = dynamic_cast<Connecting *>(events_[ev->data.fd]);
  if (conn->GetEventStatus() == kWrite) {
    HttpResponse response;
    HttpProcessor::ProcessHttpRequest(conn->GetParsedRequest(),
                                      conn->GetContext().locations, &response);
    conn->SetSender(response.GetRawResponse());
  }
  conn->SendResponse();
}

// void Server::CgiRun(epoll_event *epo_ev) {
//   Connecting *conn = dynamic_cast<Connecting *>(events_[epo_ev->data.fd]);
//   Event *sock =
//       new Cgi(conn->GetContext(), conn->GetParsedRequest(), epo_ev->data.fd);
//   // Events_.insert(std::make_pair(sock->GetFd(),sock));
//   // epoll_event new_ev = Epoll::Create(sock->GetFd(), EPOLLIN);
//   // epoll_.Mod(ev, 0);
//   (void)ev;
//   // Cgi *cgi = new Cgi(event->GetContext(), event->GetParsedRequest(),
//   kGet);
//   // cgi->Run();
//   // std::cout << cgi->GetChunked() << std::endl;
//   // delete cgi;
//   // event->SetEventStatus(kDel);
// }
void Server::CgiEvent(epoll_event *ev) {
  (void)ev;
  // Cgi *sock = dynamic_cast<Cgi *>(events_[ev->data.fd]);
  // sock->SetChunked(sock->Read());
  // 無限にプリント　→　時間でタイムアウト
  // 無限ループ　→　read byteが0だったらキルする
  // cgi読み込み終了したらcgi呼び出したEventのepoll writeに変更
}
