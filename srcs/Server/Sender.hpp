#ifndef SRCS_SERVER_SENDER_HPP_
#define SRCS_SERVER_SENDER_HPP_

#include <string>

class Sender {
 public:
  Sender();
  void Init(std::string const &response);
  void Send(int fd);
  ~Sender();

  std::string GetBuf() const;
  void SetSendedBytes(size_t bytes);
  int GetSendedBytes() const;
  int GetTotalBytes() const;

  bool HasMoreToSend() const;

 private:
  Sender(Sender const &other);
  Sender &operator=(Sender const &other);

  std::string buf_;
  size_t sended_bytes_;
  size_t total_bytes_;
};

#endif  // SRCS_SERVER_SENDER_HPP_
