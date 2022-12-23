#include "Path.hpp"

#include "Utils.hpp"
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
  LocationPair max_path_length_location;
  for (std::map<std::string, LocationContext>::const_iterator itr =
           locations.begin();
       itr != locations.end(); itr++) {
    std::string location_uri = itr->first;

    if (request_uri.size() >= location_uri.size() &&
        utils::StartWith(request_uri, location_uri)) {
      if (max_path_length_location.first.size() < location_uri.size()) {
        max_path_length_location = *itr;
      }
      continue;
    }
  }
  if (max_path_length_location.first.empty()) {
    throw LocationNotFound();
  }
  return max_path_length_location;
}

std::string Path::GetAliasPath(const LocationPair &location_pair,
                               const std::string &request_uri) {
  std::string location_uri = location_pair.first;
  std::string root = location_pair.second.root;

  std::string alias_path = root + request_uri.substr(location_uri.size());
  return alias_path;
}

const char *LocationNotFound::what() const throw() {
  return "Location Context not found";
}

bool Path::IsValidPath(const std::string &path) {
  std::vector<std::string> v = utils::SplitWithMultipleSpecifier(path, "/");
  std::stack<std::string> s;
  for (size_t i = 0; i < v.size(); i++) {
    if (v[i] == ".") continue;
    if (v[i] == "..") {
      if (s.empty()) return false;
      s.pop();
    } else {
      s.push(v[i]);
    }
  }
  return true;
}
bool Path::IsFullPath(const std::string &path) {
  if (path.empty()) return false;
  return path[0] == '/';
}
bool Path::IsAbsoluteUri(const std::string &uri) {
  if (uri.empty()) return false;
  size_t colon_pos = uri.find(":");
  if (colon_pos == std::string::npos) return false;
  std::string scheme = uri.substr(0, colon_pos);
  std::string path = uri.substr(colon_pos + 1);
  if (scheme != "https" && scheme != "http") return false;
  if (path.size() < 2 || (path[0] != '/' || path[1] != '/')) return false;
  return true;
}

std::string Path::Normalize(const std::string &path) {
  bool last_slash = false;
  if (path.find_last_of("/") != std::string::npos &&
      path[path.size() - 1] == '/')
    last_slash = true;
  std::vector<std::string> v = utils::SplitWithMultipleSpecifier(path, "/");
  std::deque<std::string> dque;
  std::string normalize_path = "/";
  for (size_t i = 0; i < v.size(); i++) {
    if (v[i] == ".") continue;
    if (v[i] == "..") {
      if (dque.empty()) std::runtime_error("path normalize err");
      dque.pop_back();
    } else {
      dque.push_back(v[i]);
    }
  }
  while (!dque.empty()) {
    std::string top = dque.front();
    dque.pop_front();
    if (dque.empty() && !last_slash) {
      normalize_path += top;
    } else {
      normalize_path += top + "/";
    }
  }
  return normalize_path;
}
