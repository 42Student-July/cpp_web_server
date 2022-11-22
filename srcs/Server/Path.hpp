#ifndef SRCS_SERVER_PATH_HPP_
#define SRCS_SERVER_PATH_HPP_

#include <map>
#include <string>

#include "LocationContext.hpp"

typedef std::map<std::string, LocationContext> Locmap;

class Path {
 public:
  Path();
  explicit Path(const std::string &uri);
  Path(Path const &other);
  Path &operator=(Path const &other);
  ~Path();
  std::string SedLocation(const Locmap &locs);
  void SetFilePath(std::string name, std::string path);

 private:
  std::string file_name_;
  std::string file_path_;
};
#endif  // SRCS_SERVER_PATH_HPP_
