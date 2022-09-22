#ifndef SRCS_PARSEREQUESTMESSAGE_HPP_
#define SRCS_PARSEREQUESTMESSAGE_HPP_
#include <iostream>
#include <string>
#define string std::string
class ParseRequestMessage {
 private:
  ParseRequestMessage();
  string message_;

 public:
  explicit ParseRequestMessage(const char* message);
  ~ParseRequestMessage();
  string GetPath();
  string GetFilename(const string& exec_file);
  static string PathAnalyzer();
};

#endif  // SRCS_PARSEREQUESTMESSAGE_HPP_
