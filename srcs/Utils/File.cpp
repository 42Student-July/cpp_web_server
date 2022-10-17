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
int File::Status() const {
  struct stat st;
  if (stat(filename_.c_str(), &st) < 0) {
    return NOT_FOUND;
  }
  if (S_ISDIR(st.st_mode)) {
    return IS_DIR;
  }
  if (!S_ISREG(st.st_mode) || ((S_IRUSR & st.st_mode) == 0u)) {
    return READ_PERMISSION;
  }
  // if(! S_ISREG(st.st_mode) || !(S_IXUSR & st.st_mode)){
  //   return EXEC_PERMISSION;
  // }
  return OK;
}

std::string File::ReadFileLines() const {
  std::string lines;
  if (Status() != OK) return lines;
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
  if (Status() != OK) return lines;
  std::fstream reading_file;
  std::string reading_line;
  reading_file.open(filename_.c_str(), std::ios::in);
  while (true) {
    std::getline(reading_file, reading_line);
    if (reading_file.eof()) {
      break;
    }
    // reading_line += "\n";
    lines.push_back(reading_line);
  }
  if (!reading_line.empty()) lines.push_back(reading_line);
  reading_file.close();
  return lines;
}