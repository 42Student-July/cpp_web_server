#ifndef SRCS_CONFIG_SERVERCONTEXT_HPP_
#define SRCS_CONFIG_SERVERCONTEXT_HPP_
#include <stdlib.h>

#include <map>
#include <set>
#include <stack>
#include <string>
#include <utility>
#include <vector>

#include "LocationContext.hpp"
class ServerContext {
 public:
  std::set<std::pair<std::string, std::string> >
      listen;  // host, int; hostのかぶりはbindでエラーでるか確かめる
  std::pair<long, bool> client_body_size;
  std::set<std::string> server_name;  // ここデフォルトあるか
  std::map<long, std::string> error_page;
  std::map<std::string, LocationContext> locations;

 public:
  ServerContext();
  ServerContext(const ServerContext &sc);
  ServerContext &operator=(const ServerContext &sc);
  ~ServerContext();
};
#endif  // SRCS_CONFIG_SERVERCONTEXT_HPP_
