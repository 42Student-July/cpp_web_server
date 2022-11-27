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

std::string Sender::GetBuf() const { return buf; }

void Sender::SetSendedBytes(size_t bytes) { sended_bytes = bytes; }

int Sender::GetSendedBytes() const { return sended_bytes; }

int Sender::GetTotalBytes() const { return total_bytes; }

bool Sender::HasMoreToSend() const { return sended_bytes < total_bytes; }
