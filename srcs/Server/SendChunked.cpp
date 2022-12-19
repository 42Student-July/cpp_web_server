#include "SendChunked.hpp"
static const char *crlf = "\r\n";
SendChunked::SendChunked() : sent_byte_(0), sent_last_chunked_(false) {
  last_chunk_ = "0\r\n\r\n";
}
SendChunked::~SendChunked() {}
void SendChunked::Send(int fd, const std::string &str) {
  try {
    if (!send_chunk_.empty()) {
      WriteAndSubStr(fd, &send_chunk_);
    } else {
      size_t length = 0;
      if (sent_byte_ + 15 < str.size()) {
        length = 15;
      } else {
        length = str.size() - sent_byte_;
      }
      std::stringstream ss;
      ss << std::hex << length;
      send_chunk_ = ss.str() + crlf + str.substr(sent_byte_, length) + crlf;
      sent_byte_ += length;
      WriteAndSubStr(fd, &send_chunk_);
    }
  } catch (std::runtime_error &e) {
    std::cerr << e.what() << std::endl;
    sent_last_chunked_ = true;
  }
}

void SendChunked::SendLastChunk(int fd) {
  try {
    WriteAndSubStr(fd, &last_chunk_);
    sent_last_chunked_ = true;
  } catch (std::runtime_error &e) {
    std::cerr << e.what() << std::endl;
    sent_last_chunked_ = true;
  }
}
#include <cerrno>
ssize_t SendChunked::WriteAndSubStr(int fd, std::string *str) {
  // std::cout << *str << std::endl;
  ssize_t wrriten = write(fd, str->c_str(), str->size());
  // if(wrriten <= 0){
  //   std::cerr <<"fd :" <<fd << std::endl;
  //   std::cerr << "wrriten :" << wrriten << std::endl;
  //   std::cerr << "errno :" << errno << std::endl;
  // }
  if (wrriten <= 0) throw std::runtime_error("write err");
  if (static_cast<size_t>(wrriten) == str->size()) {
    str->clear();
    return wrriten;
  }
  *str = str->substr(wrriten);
  return wrriten;
}
bool SendChunked::SentLastChunk() const { return sent_last_chunked_; }

size_t SendChunked::SentByte() const { return sent_byte_; }
