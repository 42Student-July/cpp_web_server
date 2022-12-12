#ifndef SRCS_SERVER_DELETE_HPP_
#define SRCS_SERVER_DELETE_HPP_
#include <string>

#include "Method.hpp"
class Delete : public HttpMethod {
 private:
  ResponseCode rescode_;

 public:
  Delete();
  ~Delete();
  void Run(const std::string &path, Socket *sock);
  void UpdateSocketData(Socket *sock);
};
#endif  // SRCS_SERVER_DELETE_HPP_
