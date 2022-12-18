#include "Server.hpp"
Server::Server(const ContextMap &contexts) { InitListen(contexts); }
Server::~Server() {
  for (std::map<int, Event *>::iterator it = event_map_.begin();
       it != event_map_.end(); it++) {
    delete it->second->GetSocket();
    it->second->SetSocket(NULL);
    delete it->second;
  }
}
void Server::InitListen(const ContextMap &contexts) {
  ContextMap::const_iterator it = contexts.begin();
  for (; it != contexts.end(); it++) {
    Listen listen(it->first.first, it->first.second);
    const int fd = listen.GenerateConnectableFd();
    Event *event = new ListenToClient(fd, it->second);
    AddEventToMonitored(fd, event, EPOLLIN);
  }
}
void Server::AddEventToMonitored(const int fd, Event *event,
                                 uint32_t event_flag) {
  event_map_.insert(std::make_pair(fd, event));
  epoll_event new_ev = Epoll::Create(fd, event_flag);
  epoll_.Add(fd, &new_ev);
}
void Server::AddEventToMonitored(const int fd, Event *event,
                                 epoll_event *new_ev) {
  event_map_[fd] = event;
  epoll_.Add(fd, new_ev);
}
void Server::Run() {
  signal(SIGPIPE, SIG_IGN);
  while (true) {
    int ready = epoll_.Wait();
    std::cout << ready << std::endl;
    EventExec(ready);
  }
}
void Server::EventExec(int ready) {
  for (int i = 0; i < ready; i++) {
    try {
      // usleep(5000);
      epoll_event epoll = epoll_.Find(i);
      Event *event = event_map_[epoll.data.fd];
      event->Do();
      event->Handle(&epoll_);
      RegisterNewEvent(event);
      NextEvent(event, &epoll);
    } catch (EpollErr &e) {
      std::cout << e.Msg() << std::endl;
      if (event_map_.find(e.GetFd()) != event_map_.end()) {
        delete event_map_[e.GetFd()];
        event_map_.erase(e.GetFd());
      }
    } catch (std::runtime_error &e) {
      std::cerr << e.what() << std::endl;
    }
  }
}

void Server::RegisterNewEvent(Event *event) {
  // ここの返り値はもう少し考える
  std::pair<Event *, epoll_event> new_event = event->PublishNewEvent();
  if (new_event.first == NULL) return;
  AddEventToMonitored(new_event.second.data.fd, new_event.first,
                      &(new_event.second));
}

void Server::NextEvent(Event *event, epoll_event *epoll) {
  Event *next_event = event->NextEvent();
  if (next_event != NULL) {
    delete event_map_[epoll->data.fd];
    event_map_.erase(epoll->data.fd);
    event_map_[epoll->data.fd] = next_event;
  } else if (Event::IsDelete(event->State())) {
    epoll_.Del(epoll->data.fd, epoll);
    delete event_map_[epoll->data.fd];
    event_map_.erase(epoll->data.fd);
  }
}
// void Server::CheckTimeOut() {
//   std::map<int, EventData>::iterator it = event_map_.begin();
//   while (it != event_map_.end()) {
//     if (it->second.time.TimeOver()) {
//       try {
//         int del_fd = it->first;
//         delete it->second.event->GetSocket();
//         it->second.event->SetSocket(NULL);
//         delete it->second.event;
//         event_map_.erase(it);
//         epoll_event ev = epoll_.FindFromFd(del_fd);
//         epoll_.Del(del_fd, &ev);
//       } catch (EpollErr &e) {
//         std::cout << e.Msg() << std::endl;
//       }
//     }
//   }
// }
