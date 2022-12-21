#ifndef SRCS_UTILS_UTILS_HPP_
#define SRCS_UTILS_UTILS_HPP_

#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

#include <cerrno>
#include <map>
#include <string>
#include <vector>

#include "ReceiveHttpRequest.hpp"
namespace utils {
std::string Itoa(int n);
int Atoi(const std::string &s);
long StrToLong(const std::string &str);
long HexStrToLong(const std::string &str);
std::vector<std::string> SplitWithMultipleSpecifier(
    const std::string &str, const std::string &separators);

char *StrToCharPtr(const std::string &str);
std::string UIntToString(size_t num);
void DelPtr(char **ptr);
char **MapToCharDoublePtr(const std::map<std::string, std::string> &m);
char **VecToCharDoublePtr(const std::vector<std::string> &vec);
std::string ToStr(const Method &m);
bool StartWith(const std::string &str, const std::string &prefix);
}  // namespace utils

#endif  // SRCS_UTILS_UTILS_HPP_
