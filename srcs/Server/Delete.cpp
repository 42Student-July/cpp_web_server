#include "Delete.hpp"

#include "File.hpp"
Delete::Delete() {}
Delete::~Delete() {}
void Delete::Run(const std::string &path, Socket *sock) {
  File f(path);
  (void)sock;
  if (f.DelFile() == -1) {
    rescode_ = kKk403Forbidden;
  } else {
    rescode_ = kKk204NoContent;
  }
}
void Delete::UpdateSocketData(Socket *sock) { sock->response_code = rescode_; }
