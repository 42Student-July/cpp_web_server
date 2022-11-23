#include "Utils.hpp"

namespace utils {

std::string Itoa(int n) {
  std::string s;
  if (n == 0) {
    s = "0";
  } else {
    while (n > 0) {
      s = static_cast<char>(n % 10 + '0') + s;
      n /= 10;
    }
  }
  return s;
}

int Atoi(const std::string &s) {
  int n = 0;
  for (std::string::const_iterator it = s.begin(); it != s.end(); ++it) {
    n = n * 10 + (*it - '0');
  }
  return n;
}

long StrToLong(const std::string &str) {
  char *end = NULL;
  errno = 0;
  long ret = strtol(str.c_str(), &end, 10);
  if (errno == ERANGE || (*end != '\0')) return -1;
  return ret;
}
std::vector<std::string> SplitWithMultipleSpecifier(
    const std::string &str, const std::string &separators) {
  std::vector<std::string> elements;
  std::string item;
  for (size_t i = 0; i < str.size(); i++) {
    if (separators.find(str[i]) != std::string::npos) {
      if (!item.empty()) elements.push_back(item);
      item.clear();
    } else {
      item += str[i];
    }
  }
  if (!item.empty()) elements.push_back(item);
  return elements;
}
char *StrToCharPtr(const std::string &str) {
  char *ptr = new char[str.size() + 1];
  memmove(ptr, str.c_str(), str.size() + 1);
  return ptr;
}
}  // namespace utils
