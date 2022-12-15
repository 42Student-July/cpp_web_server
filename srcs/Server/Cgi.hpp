#ifndef SRCS_SERVER_CGI_HPP_
#define SRCS_SERVER_CGI_HPP_
#include <signal.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <unistd.h>

#include <exception>
#include <map>
#include <string>
#include <vector>

#include "Event.hpp"
#include "ReceiveHttpRequest.hpp"
#include "ServerContext.hpp"
#include "Socket.hpp"
#include "Timer.hpp"
#include "Utils.hpp"
class Cgi {
 private:
  int fd_[2];
  std::string path_info_;  // request parser
  std::vector<std::string> argv_;
  std::map<std::string, std::string> env_map_;
  void SockPair();
  void SetSockopt();
  void SetEnv(Socket *socket);
  void StoreStrIfNotEmpty(const std::string &str);
  void ParseArgv(Socket *socket);
  void Fork(Socket *socket);

 public:
  Cgi();
  ~Cgi();
  void Run(const std::string &full_path, Socket *socket);
};
#endif  // SRCS_SERVER_CGI_HPP_
