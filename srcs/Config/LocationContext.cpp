#include "LocationContext.hpp"
LocationContext::LocationContext() : redirect(-1, "") {}
LocationContext::~LocationContext() {}

LocationContext::LocationContext(const LocationContext &lc) { *this = lc; }
LocationContext &LocationContext::operator=(const LocationContext &lc) {
  if (this == &lc) return *this;
  root = lc.root;
  limit_except = lc.limit_except;
  auto_index = lc.auto_index;
  index = lc.index;
  redirect = lc.redirect;
  cgi_extension = lc.cgi_extension;
  upload_pass = lc.upload_pass;
  return *this;
}
