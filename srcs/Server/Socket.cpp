#include "Socket.hpp"

#include "Cgi.hpp"
#include "CgiRead.hpp"
#include "CgiResponse.hpp"
#include "CgiWrite.hpp"
#include "HttpMethod.hpp"
#include "PrepareNextEventFromRequestAndConfig.hpp"
Socket::Socket(const int fd, const std::vector<ServerContext>& context)
    : vec_context(context),
      sock_fd(fd),
      response_code(kKkNotSet),
      can_write(true) {
  server_context = context[0];
}
Socket::~Socket() {
  if (!CgiFinished()) {
    kill(cgi_res.process_id, SIGTERM);
  }
}
void Socket::CgiReadAndWaitPid() {
  if ((cgi_res.read_size = read(cgi_res.cgi_fd, cgi_res.buf, kBuffSize)) == -1)
    std::cout << "cgi read err" << std::endl;  // throw surkamo
  int pid_status =
      waitpid(cgi_res.process_id, &(cgi_res.pid_exit_status), WNOHANG);
  if (pid_status == -1) {
    throw std::runtime_error("wait_pid error");
  }
  if (pid_status > 0) {
    if (WEXITSTATUS(cgi_res.pid_exit_status) != 0)
      throw std::runtime_error("WEXITITSTATUS ERR");
  }
  cgi_res.buf[cgi_res.read_size] = '\0';
}

Event* Socket::PrepareNextEventProcess() {
  // paht 探索 index くっつけるcgi 実行,イベントに追加 get(auto indexも) post
  // redirectも delete処理　 通常の処理実行、cgi実行
  PrepareNextEventFromRequestAndConfig pre(server_context, pr);
  std::cout << "prepare" << std::endl;
  if (!pre.RequestMethodAllowed()) {
    response_code = kKk405MethodNotAllowed;
  } else if (pre.IsRequestCgi()) {
    try {
      Cgi c(this);
      c.Run();
      if (pr.request_body.empty()) return new CgiRead(this);
      return new CgiWrite(this);
    } catch (...) {
      response_code = kKk500internalServerError;
    }
  } else {
    HttpMethod* m = HttpMethod::Build(pr.m);
    if (m == NULL) {
      response_code = kKk501NotImplemented;
      return NULL;
    }
    m->Run(pre.GetFullPath(), this);  // auto index
    m->UpdateSocketData(this);
    delete m;
  }
  return NULL;
}

bool Socket::CgiFinished() {
  return waitpid(cgi_res.process_id, &(cgi_res.pid_exit_status), WNOHANG) > 0;
}
