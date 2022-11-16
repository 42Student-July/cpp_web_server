#ifndef SRCS_SERVER_EVENT_HPP_
#define SRCS_SERVER_EVENT_HPP_

#include <string>

#include "Fd.hpp"
#include "ServerContext.hpp"
enum EventType { LISTEN, CONNECTING, CGI };
enum EventStatus { READ, WRITE, WAIT, DEL };
class Event : public Fd {
 private:
  ServerContext context_;
  EventStatus status_;
  const EventType type_;
  Event& operator=(const Event& sock);
  Event();
  Event(const Event& sock);
  static const int Kbuffer_size_ = 8192;

 public:
  Event(const int fd, const ServerContext& context, const EventType type);
  virtual ~Event();
  void SetEventStatus(EventStatus status);
  int GetEventStatus() const;
  int GetEventType() const;
  const ServerContext& GetContext() const;
  std::string Read() const;
  ssize_t Write(const char* str, const size_t size) const;
};

#endif  // SRCS_SERVER_EVENT_HPP_
