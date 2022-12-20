#include "ServerContext.hpp"

#include <stdexcept>
ServerContext::ServerContext() : client_body_size(false, 100000) {}
ServerContext::~ServerContext() {}
ServerContext::ServerContext(const ServerContext &sc) { *this = sc; }
ServerContext &ServerContext::operator=(const ServerContext &sc) {
  if (this == &sc) return *this;
  listen = sc.listen;
  client_body_size = sc.client_body_size;
  server_name = sc.server_name;
  error_page = sc.error_page;
  locations = sc.locations;
  return *this;
}

std::string ServerContext::GetHost() const { return listen.first; }
std::string ServerContext::GetPort() const { return listen.second; }

ContextMap ServerContext::ToMap(const std::vector<ServerContext> &context) {
  ContextMap cmap;
  for (size_t i = 0; i < context.size(); i++) {
    ContextMap::iterator it = cmap.find(context[i].listen);
    if (it == cmap.end()) {
      if (context[i].GetPort().empty())
        throw std::runtime_error("server context no port");
      std::vector<ServerContext> v;
      v.push_back(context[i]);
      cmap.insert(std::make_pair(context[i].listen, v));
    } else {
      it->second.push_back(context[i]);
    }
  }
  return cmap;
}
