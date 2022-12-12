#include "PrepareElementsFromRequestAndConfig.hpp"
PrepareElementsFromRequestAndConfig::PrepareElementsFromRequestAndConfig(
    const ServerContext &sc, const ParsedRequest &pr)
    : sc_(sc), pr_(pr) {
  selected_location_context_ =
      Path::FindBestLocation(sc.locations, pr.request_path);
  full_path_ = Path::GetAliasPath(selected_location_context_, pr.request_path);
  f_.SetFileName(full_path_);
}
PrepareElementsFromRequestAndConfig::~PrepareElementsFromRequestAndConfig() {}
std::string PrepareElementsFromRequestAndConfig::GetFullPath() const {
  return full_path_;
}
void PrepareElementsFromRequestAndConfig::UpdateData(Socket *sock) const {
  sock->location_context = selected_location_context_.second;
  sock->full_path = full_path_;
}
LocationContext PrepareElementsFromRequestAndConfig::GetLocation() const {
  return selected_location_context_.second;
}
bool PrepareElementsFromRequestAndConfig::IsRequestCgi() {
  // cgi
  return selected_location_context_.second.IsAllowExtensionCgi(full_path_) &&
         f_.IsFile();
}
bool PrepareElementsFromRequestAndConfig::RequestMethodAllowed() {
  return selected_location_context_.second.IsAllowMethod(pr_.m);
}
