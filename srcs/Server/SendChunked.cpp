#include "SendChunked.hpp"
static const char *crlf = "\r\n";
SendChunked::SendChunked()
    : sent_byte_(0),
      sent_last_chunked_(false),
      err_(false),
      size_other_than_body_(0) {
  last_chunk_ = "0\r\n\r\n";
}
SendChunked::~SendChunked() {}
void SendChunked::Send(int fd, const std::string &str) {
  try {
    if (!send_chunk_.empty()) {
      sent_byte_ += WriteAndSubStr(fd, &send_chunk_);
    } else {
      std::stringstream ss;
      std::string send_str = str.substr(sent_byte_ - size_other_than_body_);
      ss << std::hex << send_str.size();
      send_chunk_ = ss.str() + crlf + send_str + crlf;
      sent_byte_ += WriteAndSubStr(fd, &send_chunk_);
      size_other_than_body_ += ss.str().size() + 4;
    }
  } catch (std::runtime_error &e) {
    std::cerr << e.what() << std::endl;
    err_ = true;
  }
}

void SendChunked::SendLastChunk(int fd) {
  try {
    WriteAndSubStr(fd, &last_chunk_);
    sent_last_chunked_ = true;
  } catch (std::runtime_error &e) {
    std::cerr << e.what() << std::endl;
    err_ = true;
    sent_last_chunked_ = true;
  }
}
ssize_t SendChunked::WriteAndSubStr(int fd, std::string *str) {
  // std::cout << *str << std::endl;
  ssize_t wrriten = write(fd, str->c_str(), str->size());
  // if(wrriten <= 0){
  //   std::cerr <<"fd :" <<fd << std::endl;
  //   std::cerr << "wrriten :" << wrriten << std::endl;
  //   std::cerr << "errno :" << errno << std::endl;
  // }
  if (wrriten == -1) throw std::runtime_error("write err");
  if (static_cast<size_t>(wrriten) == str->size()) {
    str->clear();
    return wrriten;
  }
  *str = str->substr(wrriten);
  return wrriten;
}
bool SendChunked::SentLastChunk() const { return sent_last_chunked_; }

size_t SendChunked::SentByte() const {
  size_t res = sent_byte_ - size_other_than_body_;
  if (res > sent_byte_) return 0;
  return sent_byte_ - size_other_than_body_;
}
bool SendChunked::WriteErr() const { return err_; }
