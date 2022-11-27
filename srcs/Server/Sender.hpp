#ifndef SRCS_SERVER_SENDER_HPP_
#define SRCS_SERVER_SENDER_HPP_

#include <string>

class Sender {
 public:
  Sender();
  void Init(std::string const &response);
  void Send(int fd);
  ~Sender();

 private:
  Sender(Sender const &other);
  Sender &operator=(Sender const &other);

  std::string buf;
  size_t sended_bytes;
  size_t total_bytes;

  bool HasMoreToSend() const;
};

#endif  // SRCS_SERVER_SENDER_HPP_
