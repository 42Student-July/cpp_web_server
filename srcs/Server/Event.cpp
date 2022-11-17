#include "Event.hpp"
Event::Event(const int fd, const ServerContext& context, const EventType type)
    : Fd(fd), context_(context), type_(type) {}
Event::~Event() { Close(); }
void Event::SetEventStatus(EventStatus status) { status_ = status; }
int Event::GetEventStatus() const { return status_; }
int Event::GetEventType() const { return type_; }
const ServerContext& Event::GetContext() const { return context_; }

ssize_t Event::Write(const char* str, size_t size) const {
  ssize_t written_size = write(GetFd(), str, size);
  if (written_size == -1) throw std::runtime_error("write err");
  return written_size;
}
std::string Event::Read() const {
  char buf[kBufferSize];
  ssize_t read_size = read(GetFd(), buf, kBufferSize);
  if (read_size == -1) throw std::runtime_error("read err");
  buf[read_size] = '\0';
  std::string ret(buf);
  return ret;
}
