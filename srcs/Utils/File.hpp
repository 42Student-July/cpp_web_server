#ifndef SRCS_UTILS_FILE_HPP_
#define SRCS_UTILS_FILE_HPP_
#include <stdexcept>
#include <string>
#include <vector>
enum { OK, NOT_FOUND, IS_DIR, EXEC_PERMISSION, READ_PERMISSION };
class File {
 private:
  std::string filename_;

 public:
  File();
  explicit File(const std::string filename);
  File(const File &file);
  File &operator=(const File &file);

  ~File();
  int Status() const;
  std::string ReadFileLines() const;
  std::vector<std::string> StoreFileLinesInVec() const;
};

#endif  // SRCS_UTILS_FILE_HPP_
