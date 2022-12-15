#include "LocationContext.hpp"
LocationContext::LocationContext() : redirect(-1, "") {}
LocationContext::~LocationContext() {}

LocationContext::LocationContext(const LocationContext &lc) { *this = lc; }
LocationContext &LocationContext::operator=(const LocationContext &lc) {
  if (this == &lc) return *this;
  root = lc.root;
  limit_except = lc.limit_except;
  auto_index = lc.auto_index;
  index = lc.index;
  redirect = lc.redirect;
  cgi_extension = lc.cgi_extension;
  upload_path = lc.upload_path;
  return *this;
}
bool LocationContext::IsAllowMethod(const Method &m) {
  if (limit_except.empty()) return true;
  if (m == kDelete) {
    return (limit_except.find("DELETE") != limit_except.end());
  }
  if (m == kGet) {
    return (limit_except.find("GET") != limit_except.end());
  }
  if (m == kPost) {
    return (limit_except.find("POST") != limit_except.end());
  }
  return false;
}
bool LocationContext::IsAllowExtensionCgi(const std::string &file_name) {
  size_t comma_pos = 0;
  if ((comma_pos = file_name.find_last_of(".")) == std::string::npos)
    return false;
  std::string extension = file_name.substr(comma_pos);
  for (size_t i = 0; i < cgi_extension.size(); i++) {
    if (cgi_extension[i] == extension) return true;
  }
  return false;
}
bool LocationContext::IsUploadPath(const std::string &path) const {
  if (upload_path.empty()) return false;
  return path.substr(0, path.find_last_of("/")) ==
         upload_path.substr(0, path.find_last_of("/"));
}

std::string MethodToStr(Method m) {
  switch (m) {
    case kGet:
      return "GET";
    case kDelete:
      return "DELETE";
    case kPost:
      return "POST";
    default:
      break;
  }
  return "";
}
