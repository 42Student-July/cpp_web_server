#include "Post.hpp"

#include "File.hpp"
Post::Post() {}
Post::~Post() {}
void Post::Run(const std::string &path, Socket *sock) {
  std::string file_path = path + sock->location_context.index;
  File f(file_path);
  if (f.IsDir()) {
    rescode_ = kKk403Forbidden;
    return;
  }
  if (f.Replace(sock->pr.request_body) == -1) {
    rescode_ = kKk403Forbidden;
  } else {
    rescode_ = kKk201Created;
  }
}
void Post::UpdateSocketData(Socket *sock) { sock->response_code = rescode_; }
