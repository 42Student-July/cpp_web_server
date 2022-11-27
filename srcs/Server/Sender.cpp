#include "Sender.hpp"

Sender::Sender() : buf(), sended_bytes(0), total_bytes(0) {}

Sender::Sender(Sender const &other) { *this = other; }

Sender &Sender::operator=(Sender const &other) {
  if (this != &other) {
    (void)other;
  }
  return *this;
}

Sender::~Sender() {}

void Sender::Init(std::string const &response) {
  buf = response;
  sended_bytes = 0;
  total_bytes = buf.size();
}
