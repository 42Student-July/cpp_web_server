
#include "HttpProcessor.hpp"

#include <vector>

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
    if (file.CanRead()) {
      ReadLocalFile(file, result);
      return;
    } else {
      result->SetStatusCode(403);
      return;
    }
  }

  // fileがdirectoryの場合
  if (file.IsDir()) {
    if (selected_location_context.second.auto_index == "on") {
      // return autoindex file
      return;
    } else {
      ReadIndexFile(full_path, selected_location_context.second, result);
      return;
    }
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
