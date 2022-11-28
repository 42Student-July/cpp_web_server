#ifndef SRCS_SERVER_CGI_HPP_
#define SRCS_SERVER_CGI_HPP_
#include <signal.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

#include <exception>
#include <map>
#include <string>
#include <vector>

#include "Connecting.hpp"
#include "Event.hpp"
#include "ReceiveHttpRequest.hpp"
#include "ServerContext.hpp"
#include "Timer.hpp"
#include "Utils.hpp"
// enum CgiType{
// GET,
// POST
// };
class Cgi : public Event {
 private:
  static const size_t kKTimeOut = 3;
  Timer timer_;
  int pipe_out_[2];
  int pipe_in_[2];
  pid_t child_process_;
  std::string chunked_;
  Method method_;

  // CgiType type_;
  std::string pass_;
  std::string path_info_;
  std::vector<std::string> argv_;
  std::map<std::string, std::string> env_map_;
  std::string query_;
  pid_t chilid_process_;
  char **argv_ptr_;
  char **env_ptr_;
  void SetEnv();
  void PipeIn();
  void PipeOut();
  void Fork();
  void StoreStrIfNotEmpty(const std::string &str);
  void ParseArgv();
  void ArgvToCharPtr();
  void EnvMapToCharPtr();
  void Dup2();
  static void DelPtr(char **ptr);

 public:
  Cgi(const ServerContext &context, const ParsedRequest &pr, Method m);
  ~Cgi();
  void Run();
  int GetOutFd() const;
  int GetInFd() const;
  void End();
  std::string GetChunked() const;
  void SetChunked(const std::string &str);
  bool TimeOver() const;
};
#endif  // SRCS_SERVER_CGI_HPP_
