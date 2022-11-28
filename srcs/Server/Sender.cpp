#include "Sender.hpp"

#include <unistd.h>

#include <stdexcept>

Sender::Sender() : sended_bytes_(0), total_bytes_(0) {}

Sender::Sender(Sender const &other) { *this = other; }

Sender &Sender::operator=(Sender const &other) {
  if (this != &other) {
    (void)other;
  }
  return *this;
}

Sender::~Sender() {}

void Sender::Init(std::string const &response) {
  buf_ = response;
  sended_bytes_ = 0;
  total_bytes_ = buf_.size();
}

std::string Sender::GetBuf() const { return buf_; }

void Sender::SetSendedBytes(size_t bytes) { sended_bytes_ = bytes; }

int Sender::GetSendedBytes() const { return sended_bytes_; }

int Sender::GetTotalBytes() const { return total_bytes_; }

bool Sender::HasMoreToSend() const { return sended_bytes_ < total_bytes_; }

void Sender::Send(int fd) {
  int bytes =
      write(fd, buf_.c_str() + sended_bytes_, total_bytes_ - sended_bytes_);
  if (bytes == -1) {
    throw std::runtime_error("send() failed");
  }
  sended_bytes_ += bytes;
}
