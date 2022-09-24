#include "ParseRequestMessage.hpp"
ParseRequestMessage::ParseRequestMessage(const char *message)
    : message_(message) {}

ParseRequestMessage::~ParseRequestMessage() {}

// string ParseRequestMessage::GetFilename(string &exec_file){
//  return "";
// }

std::string ParseRequestMessage::GetPath() {
  std::string path;
  for (size_t i = 0, space_num = 0; i < message_.size(); i++) {
    if (space_num == 2) {
      return path;
    }
    if (space_num == 1 && message_[i] != ' ') {
      path += message_[i];
    }
    if (message_[i] == ' ') space_num++;
  }
  return "";
}
std::string ParseRequestMessage::PathAnalyzer() {
  // string request_path = GetPath();
  // string path_stirng;
  return "./index.html";
}
