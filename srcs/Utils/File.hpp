#ifndef SRCS_UTILS_FILE_HPP_
#define SRCS_UTILS_FILE_HPP_
#include <fcntl.h>
#include <unistd.h>

#include <stdexcept>
#include <string>
#include <vector>
class File {
 private:
  std::string filename_;

 public:
  File();
  explicit File(const std::string filename);
  File(const File &file);
  File &operator=(const File &file);

  ~File();
  std::string ReadFileLines() const;
  std::vector<std::string> StoreFileLinesInVec() const;
  int DelFile();
  int Replace(const std::string &body);
  bool IsExist() const;
  bool IsDir() const;
  bool IsFile() const;
  bool CanRead() const;
  bool CanWrite() const;
  bool CanExec() const;

  void SetFileName(const std::string &name);
};

#endif  // SRCS_UTILS_FILE_HPP_
