#include "PrepareNextEventFromRequestAndConfig.hpp"

#include "Cgi.hpp"
#include "CgiRead.hpp"
#include "CgiWrite.hpp"
#include "HttpMethod.hpp"
PrepareNextEventFromRequestAndConfig::PrepareNextEventFromRequestAndConfig(
    const ServerContext &sc, const ParsedRequest &pr)
    : sc_(sc), pr_(pr) {
  try {
    selected_location_context_ =
        Path::FindBestLocation(sc.locations, pr.request_path);
    full_path_ =
        Path::GetAliasPath(selected_location_context_, pr.request_path);
    f_.SetFileName(full_path_);
  } catch (...) {
    throw ErrorResponse("best location err", kKk404NotFound);
  }
}
PrepareNextEventFromRequestAndConfig::~PrepareNextEventFromRequestAndConfig() {}
std::string PrepareNextEventFromRequestAndConfig::GetFullPath() const {
  return full_path_;
}
void PrepareNextEventFromRequestAndConfig::UpdateData(Socket *sock) const {
  sock->location_context = selected_location_context_.second;
  sock->full_path = full_path_;
}
LocationContext PrepareNextEventFromRequestAndConfig::GetLocation() const {
  return selected_location_context_.second;
}
bool PrepareNextEventFromRequestAndConfig::IsRequestCgi() {
  return selected_location_context_.second.IsAllowExtensionCgi(full_path_) &&
         f_.IsFile();
}
bool PrepareNextEventFromRequestAndConfig::RequestMethodAllowed() {
  return selected_location_context_.second.IsAllowMethod(pr_.m);
}
