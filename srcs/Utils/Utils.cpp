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
}  // namespace utils
