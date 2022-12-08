#include "File.hpp"

#include <fcntl.h>
#include <sys/stat.h>

#include <fstream>
#include <iostream>
File::File(std::string filename) : filename_(filename) {}
File::File() {}
File::~File() {}
File::File(const File &file) { *this = file; }
File &File::operator=(const File &file) {
  if (this == &file) return *this;
  filename_ = file.filename_;
  return *this;
}

bool File::IsExist() const {
  struct stat st;
  return stat(filename_.c_str(), &st) == 0;
}
bool File::IsDir() const {
  struct stat st;
  return (stat(filename_.c_str(), &st) == 0 && S_ISDIR(st.st_mode));
}
bool File::IsFile() const {
  struct stat st;
  return (stat(filename_.c_str(), &st) == 0 && S_ISREG(st.st_mode));
}
bool File::CanRead() const {
  struct stat st;
  return (stat(filename_.c_str(), &st) == 0 && S_ISREG(st.st_mode) &&
          (S_IRUSR & st.st_mode) != 0u);
}
bool File::CanWrite() const {
  struct stat st;
  return (stat(filename_.c_str(), &st) == 0 && S_ISREG(st.st_mode) &&
          (S_IWUSR & st.st_mode) != 0u);
}
bool File::CanExec() const {
  struct stat st;
  return (stat(filename_.c_str(), &st) == 0 && S_ISREG(st.st_mode) &&
          (S_IXUSR & st.st_mode) != 0u);
}
void File::SetFileName(const std::string &filename) { filename_ = filename; }

std::string File::ReadFileLines() const {
  std::string lines;
  if (!CanRead()) return lines;
  std::fstream reading_file;
  std::string reading_line;
  reading_file.open(filename_.c_str(), std::ios::in);
  while (true) {
    std::getline(reading_file, reading_line);
    if (reading_file.eof()) {
      break;
    }
    lines += reading_line + "\n";
  }
  lines += reading_line;
  reading_file.close();
  return lines;
}

std::vector<std::string> File::StoreFileLinesInVec() const {
  std::vector<std::string> lines;
  if (!CanRead()) return lines;
  std::fstream reading_file;
  std::string reading_line;
  reading_file.open(filename_.c_str(), std::ios::in);
  while (true) {
    std::getline(reading_file, reading_line);
    if (reading_file.eof()) {
      break;
    }
    lines.push_back(reading_line);
  }
  if (!reading_line.empty()) lines.push_back(reading_line);
  reading_file.close();
  return lines;
}

int File::DelFile() { return unlink(filename_.c_str()); }
int File::Replace(const std::string &str) {
  int fd = open(filename_.c_str(), O_TRUNC | O_WRONLY | O_CREAT, 0644);
  if (fd == -1) return -1;
  ssize_t byte = 0;
  size_t writen_size = 0;
  while (writen_size != str.size()) {
    if ((byte = write(fd, str.c_str() + writen_size,
                      str.size() - writen_size)) < 0) {
      return -1;
    }
    writen_size += byte;
  }
  return 0;
}
