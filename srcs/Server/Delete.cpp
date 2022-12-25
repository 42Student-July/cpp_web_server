#include "Delete.hpp"

#include "ErrorPage.hpp"
#include "File.hpp"
Delete::Delete() {}
Delete::~Delete() {}
void Delete::SetErrorPage(const ResponseCode error_code, Socket *sock) {
  HttpResponseTmp response;
  response =
      ErrorPage::GetErrorPage(error_code, sock->server_context.error_page);
  SetResponseCode(response.rescode);
  SetResponseBody(response.body);
}
void Delete::SetResponseBody(const std::string &body) { body_ = body; }
void Delete::SetResponseCode(const ResponseCode &rescode) {
  rescode_ = rescode;
}
void Delete::Run(const std::string &path, Socket *sock) {
  std::string file_path = path;
  if (path[path.size() - 1] == '/') file_path + sock->location_context.index;
  File f(file_path);
  if (!f.IsExist()) {
    SetErrorPage(kKk404NotFound, sock);
    return;
  }
  if (f.IsDir()) {
    SetErrorPage(kKk403Forbidden, sock);
    return;
  }
  if (f.DelFile() == -1) {
    SetErrorPage(kKk403Forbidden, sock);
  } else {
    rescode_ = kKk200Ok;
  }
}
void Delete::UpdateSocketData(Socket *sock) { sock->response_code = rescode_; }
