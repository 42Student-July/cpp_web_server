#include "Path.hpp"

bool static Compare(const std::string &left, const std::string &right) {
  return left.length() > right.length();
}

std::vector<std::string> static LocationSort(const Locmap &locs) {
  std::vector<std::string> v;

  for (Locmap::const_iterator itr = locs.begin(); itr != locs.end(); itr++) {
    v.push_back(itr->first);
  }
  std::sort(v.begin(), v.end(), Compare);
  return v;
}

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

void Path::SetLocation(Locmap *locs) {
  std::vector<std::string> location_vector = LocationSort(*locs);
  std::string path;
  std::string root;
  std::string cat;
  for (std::vector<std::string>::const_iterator itr = location_vector.begin();
       itr != location_vector.end(); itr++) {
    path = *itr;
    root = (*locs)[path].root;

    std::string::reverse_iterator it = path.rbegin();
    if (*it != '/') {
      path += '/';
    }

    it = root.rbegin();
    if (*it != '/') {
      root += '/';
    }

    it = file_path_.rbegin();
    if (*it != '/') {
      file_path_ += '/';
    }

    if (file_path_.size() >= path.size() &&
        std::equal(path.begin(), path.end(), file_path_.begin())) {
      path_ = root + file_path_.erase(0, path.size()) + file_name_;
    } else if ((file_path_ + file_name_ + '/').size() >= path.size() &&
               std::equal(path.begin(), path.end(),
                          (file_path_ + file_name_ + '/').begin())) {
      path_ = root;
    }
  }
}

void Path::SetFilePath(std::string name, std::string path) {
  file_name_ = name;
  file_path_ = path;
}

std::string Path::GetFilePath(
    std::map<std::string, LocationContext> *location) {
  SetLocation(location);
  return path_;
}
