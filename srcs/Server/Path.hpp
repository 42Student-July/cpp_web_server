#ifndef SRCS_SERVER_PATH_HPP_
#define SRCS_SERVER_PATH_HPP_

#include <algorithm>
#include <map>
#include <string>
#include <vector>

#include "LocationContext.hpp"

typedef std::map<std::string, LocationContext> Locationmap;

class Path {
 public:
  Path();
  explicit Path(const std::string &uri);
  Path(Path const &other);
  Path &operator=(Path const &other);
  ~Path();
  void SetLocation(Locationmap *locs);
  void SetFilePath(std::string name, std::string path);
  std::string GetFilePath(Locationmap *location);


 private:
  std::string file_name_;
  std::string file_path_;
  std::string path_;
};

#endif  // SRCS_SERVER_PATH_HPP_
