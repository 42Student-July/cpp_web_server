#ifndef SRCS_CONFIG_LOCATIONCONTEXT_HPP_
#define SRCS_CONFIG_LOCATIONCONTEXT_HPP_
#include <stdlib.h>

#include <map>
#include <set>
#include <string>
#include <utility>
#include <vector>
class LocationContext {
 public:
  std::set<std::string> limit_except;
  std::pair<long, std::string> redirect;  // return
  std::string root;
  std::string auto_index;
  std::vector<std::string> index;
  std::vector<std::string> cgi_extension;
  std::string upload_pass;

 public:
  LocationContext();
  ~LocationContext();
  LocationContext(const LocationContext &lc);
  LocationContext &operator=(const LocationContext &lc);
};

#endif  // SRCS_CONFIG_LOCATIONCONTEXT_HPP_
