#include "Get.hpp"

#include "File.hpp"
#include "fcntl.h"
Get::Get() {}
Get::~Get() {}
void Get::Run(const std::string &path, Socket *sock) {
  // outindex_
  File f(path);
  if (f.IsFile()) {
    if (!f.CanRead()) {
      body_ = f.ReadFileLines();
      rescode_ = kKk200Ok;
    } else {
      rescode_ = kKk403Forbidden;
    }
  } else if (f.IsDir() && sock->location_context.auto_index == "on") {
    // directry 挿入
    body_ = "autoindex";
  } else {
    rescode_ = kKk404NotFound;
  }
}
void Get::UpdateSocketData(Socket *sock) {
  sock->response_body = body_;
  sock->response_code = rescode_;
}
