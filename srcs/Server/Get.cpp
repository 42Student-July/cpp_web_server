#include "Get.hpp"

#include "File.hpp"
#include "Utils.hpp"
#include "fcntl.h"

namespace {
std::string CreateHttpAutoIndexHtml(const std::string &request_path,
                                    const std::string &full_path) {
  std::string body;

  body += "<html><head><title>Index of ";
  body += request_path;
  body += "</title></head><body bgcolor=\"white\"><h1>Index of ";
  body += request_path;
  body += "</h1><hr><pre><a href=\"../\">../</a>\n";

  File file(full_path);
  std::vector<FileInfo> file_list = file.GetFileListInDir();
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
    body += utils::UIntToString(file_info.size);
    body += "</a>\n";
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
void Get::Run(const std::string &path, Socket *sock) {
  // outindex_
  File file(path);
  if (!file.IsExist()) {
    rescode_ = kKk404NotFound;
    return;
  }

  // fileがfileの場合
  if (file.IsFile()) {
    if (!file.CanRead()) {
      rescode_ = kKk403Forbidden;
      return;
    }
    body_ = file.ReadFileLines();
    rescode_ = kKk200Ok;
    return;
  }

  // fileがdirectoryの場合
  if (file.IsDir()) {
    if (sock->location_context.auto_index == "on") {
      body_ = CreateHttpAutoIndexHtml(sock->pr.request_path, path);
      rescode_ = kKk200Ok;
      return;
    }

    std::string index_path;
    if (sock->location_context.index.empty()) {
      index_path = "index.html";
    } else {
      // 一旦 indexには複数のindexが指定されていることは考えない
      index_path = sock->location_context.index.back();
    }
    File index_file(path + index_path);
    if (!index_file.IsExist()) {
      rescode_ = kKk404NotFound;
      return;
    }
    if (!index_file.CanRead()) {
      rescode_ = kKk403Forbidden;
      return;
    }
    body_ = index_file.ReadFileLines();
    rescode_ = kKk200Ok;
    return;
  }
  // File f(path);
  // if (f.IsFile()) {
  //   if (!f.CanRead()) {
  //     body_ = f.ReadFileLines();
  //     rescode_ = kKk200Ok;
  //   } else {
  //     rescode_ = kKk403Forbidden;
  //   }
  // } else
  // if (f.IsDir() && sock->location_context.auto_index == "on") {
  //   // directry 挿入
  //   body_ = "autoindex";
  // } else {
  //   rescode_ = kKk404NotFound;
  // }
}
void Get::UpdateSocketData(Socket *sock) {
  sock->response_body = body_;
  sock->response_code = rescode_;
}
