#include "Post.hpp"

#include "File.hpp"
Post::Post() {}
Post::~Post() {}
void Post::Run(const std::string &path, Socket *sock) {
  if (!sock->location_context.IsUploadPath(path)) {
    rescode_ = kKk403Forbidden;
    return;
  }
  File f(path);
  if (f.Replace(sock->pr.request_body) == -1) {
    rescode_ = kKk403Forbidden;
  } else {
    rescode_ = kKk201Created;
  }
}
void Post::UpdateSocketData(Socket *sock) { sock->response_code = rescode_; }
