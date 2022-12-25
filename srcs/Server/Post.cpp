#include "Post.hpp"

#include "ErrorPage.hpp"
#include "File.hpp"
Post::Post() {}
Post::~Post() {}
void Post::SetErrorPage(const ResponseCode error_code, Socket *sock) {
  HttpResponseTmp response;
  response =
      ErrorPage::GetErrorPage(error_code, sock->server_context.error_page);
  SetResponseCode(response.rescode);
  SetResponseBody(response.body);
}
void Post::SetResponseBody(const std::string &body) { body_ = body; }
void Post::SetResponseCode(const ResponseCode &rescode) { rescode_ = rescode; }
void Post::Run(const std::string &path, Socket *sock) {
  std::string file_path = path;
  if (path[path.size() - 1] == '/') file_path + sock->location_context.index;
  File f(file_path);
  if (f.IsDir()) {
    SetErrorPage(kKk403Forbidden, sock);
    return;
  }
  if (f.Replace(sock->pr.request_body) == -1) {
    SetErrorPage(kKk403Forbidden, sock);
  } else {
    rescode_ = kKk201Created;
  }
}
void Post::UpdateSocketData(Socket *sock) {
  sock->response_body = body_;
  sock->response_code = rescode_;
}
