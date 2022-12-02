#include "Path.hpp"

Path::Path() {}

Path::Path(Path const &other) { *this = other; }

Path &Path::operator=(Path const &other) {
  if (this != &other) {
    (void)other;
  }
  return *this;
}

Path::~Path() {}

LocationPair Path::FindBestLocation(const Locationmap &locations,
                                    const std::string &request_uri) {
  LocationPair selected_location;
  for (std::map<std::string, LocationContext>::const_iterator itr =
           locations.begin();
       itr != locations.end(); itr++) {
    std::string location_uri = itr->first;
    std::string root = itr->second.root;

    // location_uri が / で終端していない場合は、完全一致でないとマッチしない
    if (*location_uri.rbegin() != '/') {
      if (location_uri == request_uri) {
        selected_location = *itr;
      }
      continue;  // ?
    }

    // location_uri が / で終端している場合は、location_uri
    // で始まる場合はマッチする
    std::size_t last_slash = request_uri.find_last_of('/');
    std::string request_directory, request_filename;
    if (last_slash != std::string::npos) {
      request_directory = request_uri.substr(0, last_slash + 1);
      request_filename = request_uri.substr(last_slash + 1);
    }
    if (request_directory.size() >= location_uri.size() &&
        std::equal(location_uri.begin(), location_uri.end(),
                   request_directory.begin())) {
      selected_location = *itr;
      continue;
    }
  }
  if (selected_location.first.empty()) {
    throw LocationNotFound();
  }
  return selected_location;
}

std::string Path::GetAliasPath(const LocationPair &location_pair,
                               const std::string &request_uri) {
  std::string location_uri = location_pair.first;
  std::string root = location_pair.second.root;

  std::size_t last_slash = request_uri.find_last_of('/');
  std::string request_directory, request_filename;
  if (last_slash != std::string::npos) {
    request_directory = request_uri.substr(0, last_slash + 1);
    request_filename = request_uri.substr(last_slash + 1);
  }

  std::string alias_path = root + request_uri.substr(location_uri.size());
  return alias_path;
}

const char *Path::LocationNotFound::what() const throw() {
  return "Location Context not found";
}
