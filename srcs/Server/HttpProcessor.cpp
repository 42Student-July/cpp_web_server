
#include "HttpProcessor.hpp"

#include <vector>

#include "Utils.hpp"

HttpProcessor::HttpProcessor() {}

HttpProcessor::HttpProcessor(HttpProcessor const &other) { *this = other; }

HttpProcessor &HttpProcessor::operator=(HttpProcessor const &other) {
  if (this != &other) {
    (void)other;
  }
  return *this;
}

HttpProcessor::~HttpProcessor() {}

void HttpProcessor::ProcessHttpRequest(
    const ParsedRequest &parsed_request,
    std::map<std::string, LocationContext> locations, HttpResponse *result) {
  if (parsed_request.m == kGet) {
    ProcessHttpRequestGet(parsed_request, locations, result);
  }
}

void HttpProcessor::ProcessHttpRequestGet(
    const ParsedRequest &parsed_request,
    std::map<std::string, LocationContext> locations, HttpResponse *result) {
  LocationPair selected_location_context =
      Path::FindBestLocation(locations, parsed_request.request_path);

  std::string full_path = Path::GetAliasPath(selected_location_context,
                                             parsed_request.request_path);

  std::cout << full_path << std::endl;
  selected_location_context.second.root += parsed_request.request_path;
  File file(full_path);
  if (!file.IsExist()) {
    result->SetStatusCode(404);
    return;
  }

  // fileがfileの場合
  if (file.IsFile()) {
    if (!file.CanRead()) {
      result->SetStatusCode(403);
      return;
    }
    ReadLocalFile(file, result);
    return;
  }

  // fileがdirectoryの場合
  if (file.IsDir()) {
    if (selected_location_context.second.auto_index == "on") {
      CreateHttpAutoIndexHtml(parsed_request.request_path, full_path, result);
      return;
    }
    ReadIndexFile(full_path, selected_location_context.second, result);
    return;
  }
}

void HttpProcessor::ReadLocalFile(const File &file, HttpResponse *result) {
  std::vector<std::string> file_contents = file.StoreFileLinesInVec();
  std::string body;
  for (std::vector<std::string>::iterator it = file_contents.begin();
       it != file_contents.end(); ++it) {
    body += *it;
  }
  result->SetHttpResponse(200, body);
}

void HttpProcessor::ReadIndexFile(const std::string &full_path,
                                  const LocationContext &loc,
                                  HttpResponse *result) {
  std::string index_path;
  if (loc.index.empty()) {
    index_path = "index.html";
  } else {
    // 一旦 indexには複数のindexが指定されていることは考えない
    index_path = loc.index.back();
  }
  File file(full_path + index_path);
  if (!file.IsExist()) {
    result->SetStatusCode(404);
    return;
  }
  if (!file.CanRead()) {
    result->SetStatusCode(403);
    return;
  }
  ReadLocalFile(file, result);
}

void HttpProcessor::CreateHttpAutoIndexHtml(const std::string &request_path,
                                            const std::string &full_path,
                                            HttpResponse *result) {
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
  result->SetHttpResponse(200, body);
}
