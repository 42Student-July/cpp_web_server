#include "Socket.hpp"

#include "Cgi.hpp"
#include "CgiRead.hpp"
#include "CgiResponse.hpp"
#include "CgiWrite.hpp"
#include "HttpMethod.hpp"
#include "PrepareNextEventFromRequestAndConfig.hpp"
Socket::Socket(const int fd, const std::vector<ServerContext>& context)
    : vec_context(context), sock_fd(fd), response_code(kKkNotSet), cgi_res(-1) {
  server_context = context[0];
}
Socket::~Socket() {
  if (cgi_res.cgi_fd != -1) {
    if (!CgiFinished()) {
      kill(cgi_res.process_id, SIGTERM);
    }
    if (cgi_res.cgi_fd != 0) {
      close(cgi_res.cgi_fd);
    }
  }
  close(sock_fd);
}
int Socket::CgiReadAndStoreToBuf() {
  if ((cgi_res.read_size = read(cgi_res.cgi_fd, cgi_res.buf, kBuffSize)) ==
      -1) {
    std::cerr << "cgi read err" << std::endl;
    return -1;
  }
  // std::cout << "read size :" << cgi_res.read_size << std::endl;
  cgi_res.buf[cgi_res.read_size] = '\0';
  // std::cout << cgi_res.buf << std::endl;
  return cgi_res.read_size;
}

Event* Socket::PrepareNextEventProcess() {
  // paht 探索 index くっつけるcgi 実行,イベントに追加 get(auto indexも) post
  // redirectも delete処理　 通常の処理実行、cgi実行
  try {
    PrepareNextEventFromRequestAndConfig pre(server_context, pr);
    pre.UpdateData(this);
    if (!pre.RequestMethodAllowed()) {
      throw ErrorResponse("method not allowed", kKk405MethodNotAllowed);
    }
    if (pre.IsRequestCgi()) {
      Cgi c;
      c.Run(pre.GetFullPath(), this);
      std::cout << "cgi run" << std::endl;
      if (pr.request_body.empty()) {
        return new CgiRead(this);
      }
      return new CgiWrite(this);
    }
    HttpMethod* m = HttpMethod::Build(pr.m);
    std::cout << "method run" << std::endl;
    m->Run(pre.GetFullPath(), this);  // auto index
    m->UpdateSocketData(this);
    delete m;
  } catch (ErrorResponse& e) {
    std::cout << e.Msg() << std::endl;
    response_code = e.GetErrResponseCode();
  }
  return NULL;
}

bool Socket::CgiFinished() {
  return waitpid(cgi_res.process_id, &(cgi_res.pid_exit_status), WNOHANG) > 0;
}

ErrorResponse::ErrorResponse(const std::string& errmsg, ResponseCode rescode)
    : err_msg_(errmsg), rescode_(rescode) {}
ErrorResponse::~ErrorResponse() {}
ResponseCode ErrorResponse::GetErrResponseCode() const { return rescode_; }
std::string ErrorResponse::Msg() const throw() { return err_msg_; }
