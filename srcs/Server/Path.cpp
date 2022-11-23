#include "Path.hpp"

Path::Path() {}

Path::Path(const std::string &path) {
  std::size_t last_slash = path.find_last_of('/');
  if (last_slash != std::string::npos) {
    file_path_ = path.substr(0, last_slash + 1);
    file_name_ = path.substr(last_slash + 1);
  }
}

Path::Path(Path const &other) { *this = other; }

Path &Path::operator=(Path const &other) {
  if (this != &other) {
    (void)other;
  }
  return *this;
}

Path::~Path() {}

std::string Path::SetLocation(const Locmap &locs) {
  for (Locmap::const_iterator itr = locs.begin(); itr != locs.end(); itr++) {
    std::string path = itr->first;
    std::string root = itr->second.root;

    if (file_path_.size() >= path.size() &&
        std::equal(path.begin(), path.end(), file_path_.begin())) {
      return (root + "/" + file_path_.erase(0, path.size()) + file_name_);
    }
  }
  return "";
}

void Path::SetFilePath(const std::string &name, const std::string &path) {
  file_name_ = name;
  file_path_ = path;
}
