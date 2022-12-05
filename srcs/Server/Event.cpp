#include "Event.hpp"
Event::~Event() {}
bool Event::IsNotDelete(const EventState &st) {
  return (st == kRead || st == kWrite || st == kReadAgain ||
          st == kWriteAgain || st == kCgiReadAgain);
}
bool Event::IsFinished(const EventState &st) {
  return (st == kCgiReadFinished || st == kWriteFinished);
}
bool Event::IsDelete(const EventState &st) { return (st == kDel); }
