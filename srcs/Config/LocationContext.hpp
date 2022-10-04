#ifndef SRCS_CONFIG_LOCATIONCONTEXT_HPP_
#define SRCS_CONFIG_LOCATIONCONTEXT_HPP_
#include <map>
#include <set>
#include <string>
#include <vector>
class LocationContext {
 private:
  std::string location_;
  std::string root_;
  bool auto_index_;
  std::string index_;
  std::map<int, std::string> redirect_;

 public:
  LocationContext();
  ~LocationContext();
};

#endif  // SRCS_CONFIG_LOCATIONCONTEXT_HPP_
