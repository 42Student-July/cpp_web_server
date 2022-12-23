#ifndef SRCS_CONFIG_LOCATIONCONTEXT_HPP_
#define SRCS_CONFIG_LOCATIONCONTEXT_HPP_
#include <stdlib.h>

#include <map>
#include <set>
#include <string>
#include <utility>
#include <vector>

enum Method {
  kError,
  kConnect,
  kDelete,
  kGet,
  kHead,
  kOptions,
  kPost,
  kPut,
  kTrace
};
class LocationContext {
 public:
  std::set<std::string> limit_except;
  std::pair<long, std::string> redirect;  // return
  std::string root;
  std::string auto_index;
  std::string index;
  std::vector<std::string> cgi_extension;
  // std::string upload_path;

 public:
  LocationContext();
  ~LocationContext();
  LocationContext(const LocationContext &lc);
  LocationContext &operator=(const LocationContext &lc);
  bool IsAllowMethod(const Method &m);
  bool IsAllowExtensionCgi(const std::string &file_nmae);
  // bool IsUploadPath(const std::string &path) const;
  static std::string MethodToStr(Method m);
};

#endif  // SRCS_CONFIG_LOCATIONCONTEXT_HPP_
