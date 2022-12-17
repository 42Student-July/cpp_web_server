#include "Socket.hpp"

#include "Cgi.hpp"
#include "CgiRead.hpp"
#include "CgiResponse.hpp"
#include "CgiWrite.hpp"
#include "HttpMethod.hpp"
#include "PrepareNextEventFromRequestAndConfig.hpp"
Socket::Socket(const int fd, const std::vector<ServerContext>& context)
    : vec_context(context), sock_fd(fd), response_code(kKkNotSet) {
  server_context = context[0];
}
Socket::~Socket() {
  for (size_t i = 0; i < cgi_res.size(); i++) {
    if (!CgiFinished(i)) {
      kill(cgi_res[i].process_id, SIGTERM);
    }
    close(cgi_res[i].cgi_fd);
  }
  close(sock_fd);
}
int Socket::CgiReadAndStoreToBuf(size_t pos) {
  char read_buf[kBuffSize];
  if ((cgi_res[pos].read_size =
           read(cgi_res[pos].cgi_fd, read_buf, kBuffSize)) == -1) {
    std::cerr << "cgi read err" << std::endl;
    return -1;
  }
  // std::cout << "read size :" << cgi_res.read_size << std::endl;
  read_buf[cgi_res[pos].read_size] = '\0';
  cgi_res[pos].buf = read_buf;
  // std::cout << cgi_res.buf << std::endl;
  return cgi_res[pos].read_size;
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
      CgiRes cres;
      c.Run(pre.GetFullPath(), this, &cres);
      std::cout << "cgi run" << std::endl;
      cgi_res.push_back(cres);
      if (pr.request_body.empty()) {
        return new CgiRead(this, cgi_res.size() - 1);
      }
      return new CgiWrite(this, cgi_res.size() - 1);
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

bool Socket::CgiFinished(size_t pos) {
  return waitpid(cgi_res[pos].process_id, &(cgi_res[pos].pid_exit_status),
                 WNOHANG) > 0;
}

ErrorResponse::ErrorResponse(const std::string& errmsg, ResponseCode rescode)
    : err_msg_(errmsg), rescode_(rescode) {}
ErrorResponse::~ErrorResponse() {}
ResponseCode ErrorResponse::GetErrResponseCode() const { return rescode_; }
std::string ErrorResponse::Msg() const throw() { return err_msg_; }
