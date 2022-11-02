#ifndef SRCS_UTILS_UTILS_HPP_
#define SRCS_UTILS_UTILS_HPP_

#include <stdlib.h>

#include <cerrno>
#include <string>
#include <vector>
namespace utils {
std::string Itoa(int n);
int Atoi(const std::string &s);
long StrToLong(const std::string &str);
std::vector<std::string> SplitWithMultipleSpecifier(
    const std::string &str, const std::string &separators);
}  // namespace utils

#endif  // SRCS_UTILS_UTILS_HPP_
