#ifndef SRCS_UTILS_FILE_HPP_
#define SRCS_UTILS_FILE_HPP_
#include <string>
#include <vector>

#include "RioFileDescriptor.hpp"
class File {
 private:
  const std::string filename_;

 public:
  enum { OK, NOT_FOUND, IS_DIR, EXEC_PERMISSION, READ_PERMISSION };
  explicit File(const std::string filename);
  ~File();
  int Status() const;
  std::string ReadFileLines() const;
};

#endif  // SRCS_UTILS_FILE_HPP_
