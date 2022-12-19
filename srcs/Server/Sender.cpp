#include "Sender.hpp"

#include <unistd.h>

#include <iostream>
#include <stdexcept>
Sender::Sender() : sended_bytes_(0), total_bytes_(0), err_(false) {}

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

bool Sender::ErrorOccured() const { return err_; }
void Sender::Send(int fd) {
  try {
    int bytes =
        write(fd, buf_.c_str() + sended_bytes_, total_bytes_ - sended_bytes_);
    if (bytes <= 0) {
      throw std::runtime_error("send() failed");
    }
    sended_bytes_ += bytes;
  } catch (std::runtime_error &e) {
    std::cout << e.what() << std::endl;
    err_ = true;
  }
}
