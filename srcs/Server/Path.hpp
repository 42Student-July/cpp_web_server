#ifndef SRCS_SERVER_PATH_HPP_
#define SRCS_SERVER_PATH_HPP_

#include <algorithm>
#include <map>
#include <string>
#include <utility>
#include <vector>

#include "LocationContext.hpp"

typedef std::pair<std::string, LocationContext> LocationPair;
typedef std::map<std::string, LocationContext> Locationmap;

class Path {
 public:
  Path();
  explicit Path(const std::string &uri);
  Path(Path const &other);
  Path &operator=(Path const &other);
  ~Path();
  static LocationPair FindBestLocation(const Locationmap &locations,
                                       const std::string &request_uri);

  static std::string GetAliasPath(const LocationPair &location_pair,
                                  const std::string &request_uri);
  class LocationNotFound : public std::exception {
   public:
    const char *what() const throw();
  };
};

#endif  // SRCS_SERVER_PATH_HPP_
