#include "ServerContext.hpp"
ServerContext::ServerContext() : client_body_size(-1) {}
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
