
#include "HttpProcessor.hpp"

#include <vector>

#include "HttpResponse.hpp"

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
    std::map<std::string, LocationContext> locations, std::string *result) {
  if (parsed_request.m == kGet) {
    ProcessHttpRequestGet(parsed_request, locations, result);
  }
}

void HttpProcessor::ProcessHttpRequestGet(
    const ParsedRequest &parsed_request,
    std::map<std::string, LocationContext> locations, std::string *result) {
  Path path;

  // pathってどうやってつかうの？
  path.SetFilePath("", parsed_request.request_path);
  path.SetLocation(locations);

  // for (auto& loc : locations) {
  //   File file(path.GetFilePath() + loc.second);
  //   if (file.IsExist() && file.CanRead()) {
  //     // fullPath
  //   }
  // }

  // 仮
  std::string full_path = "./html/sample.html";

  File file(full_path);
  if (file.IsExist() && file.CanRead()) {
    ReadLocalFile(file, result);
  }
}

void HttpProcessor::ReadLocalFile(const File &file, std::string *result) {
  HttpResponse http_response;

  http_response.SetStatusCode(200);
  http_response.SetHeader("Content-Type", "text/html");
  std::vector<std::string> file_contents = file.StoreFileLinesInVec();
  std::string body;
  for (std::vector<std::string>::iterator it = file_contents.begin();
       it != file_contents.end(); ++it) {
    body += *it;
  }
  http_response.SetBody(body);
  http_response.SetHeader("Content-Length", http_response.GetBody().size());
  *result = http_response.GetRawResponse();
}
