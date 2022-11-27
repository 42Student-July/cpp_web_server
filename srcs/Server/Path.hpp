#ifndef SRCS_SERVER_PATH_HPP_
#define SRCS_SERVER_PATH_HPP_

#include <algorithm>
#include <map>
#include <string>
#include <vector>

#include "LocationContext.hpp"

typedef std::map<std::string, LocationContext> Locmap;

class Path {
 public:
  Path();
  explicit Path(const std::string &uri);
  Path(Path const &other);
  Path &operator=(Path const &other);
  ~Path();
  std::string SetLocation(Locmap *locs);
  void SetFilePath(std::string name, std::string path);
  std::string GetFilePath(std::map<std::string, LocationContext> *location);
  std::vector<std::string> sort(const Locmap &locs);

 private:
  std::string file_name_;
  std::string file_path_;
  std::string path_;
};

#endif  // SRCS_SERVER_PATH_HPP_
