#ifndef SRCS_SERVER_PARSEREQUESTMESSAGE_HPP_
#define SRCS_SERVER_PARSEREQUESTMESSAGE_HPP_
#include <iostream>
#include <string>
class ParseRequestMessage {
 private:
  ParseRequestMessage();
  std::string message_;

 public:
  explicit ParseRequestMessage(const char* message);
  ~ParseRequestMessage();
  std::string GetPath();
  std::string GetFilename(const std::string& exec_file);
  static std::string PathAnalyzer();
};

#endif  // SRCS_SERVER_PARSEREQUESTMESSAGE_HPP_
