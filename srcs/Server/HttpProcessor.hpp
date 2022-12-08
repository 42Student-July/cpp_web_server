#ifndef SRCS_SERVER_HTTPPROCESSOR_HPP_
#define SRCS_SERVER_HTTPPROCESSOR_HPP_

#include <map>
#include <string>

#include "File.hpp"
#include "HttpResponse.hpp"
#include "Path.hpp"
#include "ReceiveHttpRequest.hpp"
#include "ServerContext.hpp"

class HttpProcessor {
 public:
  HttpProcessor();
  ~HttpProcessor();

  static void ProcessHttpRequest(
      const ParsedRequest &parsed_request,
      std::map<std::string, LocationContext> locations, HttpResponse *result);

 private:
  HttpProcessor(HttpProcessor const &other);
  HttpProcessor &operator=(HttpProcessor const &other);

  static void ProcessHttpRequestGet(
      const ParsedRequest &parsed_request,
      std::map<std::string, LocationContext> locations, HttpResponse *result);
  static void ReadLocalFile(const File &file, HttpResponse *result);
  static void ReadIndexFile(const std::string &full_path,
                            const LocationContext &loc, HttpResponse *result);
  static void CreateHttpAutoIndexHtml(const std::string &request_path,
                                      const std::string &full_path,
                                      HttpResponse *result);
};

#endif  // SRCS_SERVER_HTTPPROCESSOR_HPP_
