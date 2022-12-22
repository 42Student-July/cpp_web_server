#include "Get.hpp"

#include <vector>

#include "ErrorPage.hpp"
#include "File.hpp"
#include "Utils.hpp"
#include "fcntl.h"

namespace {
std::string CreateHttpAutoIndexHtml(const std::string &request_path,
                                    const std::string &full_path) {
  std::string body;

  body += "<html>\r\n";
  body += "<head><title>Index of ";
  body += request_path;
  body += "</title></head>\r\n";
  body += "<body>\r\n";
  body += "<h1>Index of ";
  body += request_path;
  body += "</h1><hr><pre><a href=\"../\">../</a>\r\n";

  File file(full_path);
  std::vector<FileInfo> file_list(file.GetFileListInDir());
  for (std::vector<FileInfo>::iterator it = file_list.begin();
       it != file_list.end(); ++it) {
    FileInfo &file_info = *it;
    body += "<a href=\"";
    body += file_info.name;
    body += "\">";
    body += file_info.name;
    body += "\t";
    body += file_info.timestamp;
    body += "\t";
    if (file_info.name[file_info.name.size() - 1] == '/') {
      body += "-";
    } else {
      body += utils::UIntToString(file_info.size);
    }
    body += "</a>\r\n";
    std::cout << file_info.name;
    std::cout << file_info.timestamp;
    std::cout << file_info.size << std::endl;
  }
  body += "</pre><hr></body></html>";
  return body;
}

}  // namespace

Get::Get() {}
Get::~Get() {}

void Get::SetErrorPage(const ResponseCode error_code, Socket *sock) {
  HttpResponseTmp response;
  response =
      ErrorPage::GetErrorPage(error_code, sock->server_context.error_page);
  SetResponseCode(response.rescode);
  SetResponseBody(response.body);
}

void Get::Redirect(ResponseCode rescode, const std::string &path) {
  AddResponseHeader("Location", path);
  SetResponseCode(rescode);
}

void Get::Run(const std::string &path, Socket *sock) {
  // redirect が存在する場合

  if (sock->location_context.redirect.first != -1) {
    Redirect(static_cast<ResponseCode>(sock->location_context.redirect.first),
             sock->location_context.redirect.second);
    return;
  }

  // outindex_
  File file(path);
  if (!file.IsExist()) {
    SetErrorPage(kKk404NotFound, sock);
    return;
  }

  // fileがfileの場合
  if (file.IsFile()) {
    if (!file.CanRead()) {
      return;
    }
    body_ = file.ReadFileLines();
    rescode_ = kKk200Ok;
    return;
  }

  // fileがdirectoryの場合
  if (file.IsDir()) {
    std::string index_path;
    if (sock->location_context.index.empty()) {
      index_path = "index.html";
    } else {
      // 一旦 indexには複数のindexが指定されていることは考えない
      index_path = sock->location_context.index.back();
    }
    File index_file(path + index_path);
    if (!index_file.IsExist()) {
      if (sock->location_context.auto_index == "on") {
        body_ = CreateHttpAutoIndexHtml(sock->pr.request_path, path);
        rescode_ = kKk200Ok;
        return;
      }
      SetErrorPage(kKk403Forbidden, sock);
      return;
    }
    if (!index_file.CanRead()) {
      SetErrorPage(kKk403Forbidden, sock);
      return;
    }
    body_ = index_file.ReadFileLines();
    rescode_ = kKk200Ok;
    return;
  }
}
void Get::UpdateSocketData(Socket *sock) {
  sock->response_body = body_;
  sock->response_code = rescode_;
  sock->headers = headers_;
}

void Get::SetResponseBody(const std::string &body) { body_ = body; }
void Get::SetResponseCode(const ResponseCode &rescode) { rescode_ = rescode; }
void Get::AddResponseHeader(const std::string &key, const std::string &value) {
  headers_[key] = value;
}
