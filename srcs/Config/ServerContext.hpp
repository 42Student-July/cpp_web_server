#ifndef SRCS_CONFIG_SERVERCONTEXT_HPP_
#define SRCS_CONFIG_SERVERCONTEXT_HPP_

#include <set>
#include <string>
#include <utility>
#include <vector>

#include "LocationContext.hpp"

class ServerContext {
 public:
  int listen_;
  size_t client_body_size_;
  std::string host_;
  std::vector<std::string> server_name_;
  std::pair<std::set<int>, std::string> error_page_;
  std::vector<LocationContext> locations_;

 public:
  ServerContext();
  ~ServerContext();
};
#endif  // SRCS_CONFIG_SERVERCONTEXT_HPP_
