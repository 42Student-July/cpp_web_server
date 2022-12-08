#include "File.hpp"

#include <dirent.h>
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

std::vector<FileInfo> File::GetFileListInDir() const {
  std::vector<FileInfo> file_list;

  if (!IsDir()) return file_list;
  DIR *dir = NULL;
  struct dirent *ent = NULL;
  if ((dir = opendir(filename_.c_str())) != NULL) {
    while ((ent = readdir(dir)) != NULL) {
      std::string file_name(ent->d_name);
      std::cout << file_name << std::endl;
      if (file_name == "." || file_name == "..") {
        continue;
      }
      FileInfo file_info;

      struct stat buf;
      int exists = stat(filename_.c_str(), &buf);
      if (exists < 0) {
        std::cerr << "Could not stat file: " << file_name << std::endl;
        continue;
      }
      char time_buf[50];
      ctime_r(&buf.st_mtime, time_buf);
      file_info.timestamp = time_buf;
      file_info.size = buf.st_size;
      file_info.name = file_name;

      file_list.push_back(file_info);
    }
    closedir(dir);
  } else {
    std::cerr << "Could not open directory: " << filename_ << std::endl;
  }
  return file_list;
}
