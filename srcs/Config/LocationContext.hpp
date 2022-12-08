#ifndef SRCS_CONFIG_LOCATIONCONTEXT_HPP_
#define SRCS_CONFIG_LOCATIONCONTEXT_HPP_
#include <stdlib.h>

#include <map>
#include <set>
#include <string>
#include <utility>
#include <vector>

#include "ReceiveHttpRequest.hpp"
class LocationContext {
 public:
  std::set<std::string> limit_except;
  std::pair<long, std::string> redirect;  // return
  std::string root;
  std::string auto_index;
  std::vector<std::string> index;
  std::vector<std::string> cgi_extension;
  std::string upload_path;

 public:
  LocationContext();
  ~LocationContext();
  LocationContext(const LocationContext &lc);
  LocationContext &operator=(const LocationContext &lc);
  bool IsAllowMethod(const Method &m);
  bool IsAllowExtensionCgi(const std::string &file_nmae);
  bool IsUploadPath(const std::string &path) const;
};

#endif  // SRCS_CONFIG_LOCATIONCONTEXT_HPP_
