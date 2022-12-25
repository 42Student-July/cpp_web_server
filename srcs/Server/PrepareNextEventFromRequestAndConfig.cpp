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
    std::string alias =
        Path::GetAliasPath(selected_location_context_, pr.request_path);
    full_path_ = Path::Normalize(alias);
    f_.SetFileName(full_path_);
    // std::cout << "normalize :" << full_path_ << std::endl;
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
  std::string file_path = full_path_;
  File file(file_path);
  if (file.IsDir()) {
    if (!selected_location_context_.second.index.empty()) {
      file_path += selected_location_context_.second.index;
      file.SetFileName(file_path);
    }
  }
  return selected_location_context_.second.IsAllowExtensionCgi(file_path) &&
         file.IsFile();
}
bool PrepareNextEventFromRequestAndConfig::RequestMethodAllowed() {
  return selected_location_context_.second.IsAllowMethod(pr_.m);
}
