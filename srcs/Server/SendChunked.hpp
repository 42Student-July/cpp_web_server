#ifndef SRCS_SERVER_SENDCHUNKED_HPP_
#define SRCS_SERVER_SENDCHUNKED_HPP_
#include <unistd.h>

#include <iostream>
#include <sstream>
#include <string>
class SendChunked {
 private:
  size_t sent_byte_;
  bool sent_last_chunked_;
  std::string last_chunk_;
  std::string send_chunk_;
  bool err_;
  static ssize_t WriteAndSubStr(int fd, std::string *str);

 public:
  SendChunked();
  ~SendChunked();
  void Send(int fd, const std::string &str);
  void SendLastChunk(int fd);
  bool SentLastChunk() const;
  bool WriteErr() const;
  size_t SentByte() const;
};

#endif  // SRCS_SERVER_SENDCHUNKED_HPP_
