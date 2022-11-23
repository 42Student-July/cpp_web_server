#ifndef SRCS_SERVER_HTTPPROCESSOR_HPP_
#define SRCS_SERVER_HTTPPROCESSOR_HPP_

#include <map>
#include <string>

#include "File.hpp"
#include "Path.hpp"
#include "ReceiveHttpRequest.hpp"
#include "ServerContext.hpp"

class HttpProcessor {
 public:
  HttpProcessor();
  ~HttpProcessor();

  static void ProcessHttpRequest(
      const ParsedRequest &parsed_request,
      std::map<std::string, LocationContext> locations, std::string *result);

 private:
  HttpProcessor(HttpProcessor const &other);
  HttpProcessor &operator=(HttpProcessor const &other);

  static void ProcessHttpRequestGet(
      const ParsedRequest &parsed_request,
      std::map<std::string, LocationContext> locations, std::string *result);
  static void ReadLocalFile(const File &file, std::string *result);
};

#endif  // SRCS_SERVER_HTTPPROCESSOR_HPP_
