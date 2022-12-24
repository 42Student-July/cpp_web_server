#include "Delete.hpp"

#include "File.hpp"
Delete::Delete() {}
Delete::~Delete() {}
void Delete::Run(const std::string &path, Socket *sock) {
  std::string file_path = path;
  if (path[path.size() - 1] == '/') file_path + sock->location_context.index;
  File f(file_path);
  if (f.IsDir()) {
    rescode_ = kKk403Forbidden;
    return;
  }
  if (f.DelFile() == -1) {
    rescode_ = kKk403Forbidden;
  } else {
    rescode_ = kKk204NoContent;
  }
}
void Delete::UpdateSocketData(Socket *sock) { sock->response_code = rescode_; }
