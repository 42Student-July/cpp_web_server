#ifndef SRCS_CONFIG_SERVERCONTEXT_HPP_
#define SRCS_CONFIG_SERVERCONTEXT_HPP_

#include <set>
#include <string>
#include <utility>
#include <vector>

#include "LocationContext.hpp"

class ServerContext {
 private:
  // const static int klisten_ = 0;
  // const static size_t kclient_body_size_ = 0;
  std::string host_;
  std::vector<std::string> server_name_;
  std::pair<std::set<int>, std::string> error_page_;
  std::vector<LocationContext> locations_;

 public:
  ServerContext();
  ~ServerContext();
};
#endif  // SRCS_CONFIG_SERVERCONTEXT_HPP_
