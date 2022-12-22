#include "Utils.hpp"

#include <sstream>

#include "ReceiveHttpRequest.hpp"

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
long HexStrToLong(const std::string &str) {
  char *end = NULL;
  errno = 0;
  long ret = strtol(str.c_str(), &end, 16);
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

std::string UIntToString(size_t num) {
  std::ostringstream oss;

  oss << num;
  return oss.str();
}
void DelPtr(char **ptr) {
  if (ptr == NULL) return;
  for (char **tmp_ptr = ptr; *tmp_ptr != NULL; tmp_ptr++) {
    delete[] * tmp_ptr;
  }
  delete[] ptr;
}
char **MapToCharDoublePtr(const std::map<std::string, std::string> &m) {
  if (m.size() == 0) return NULL;
  char **ptr = new char *[m.size() + 1];
  std::map<std::string, std::string>::const_iterator it = m.begin();
  for (size_t i = 0; it != m.end(); i++, it++) {
    ptr[i] = utils::StrToCharPtr(it->second);
  }
  ptr[m.size()] = NULL;
  return ptr;
}
char **VecToCharDoublePtr(const std::vector<std::string> &vec) {
  if (vec.size() == 0) return NULL;
  char **ptr = new char *[vec.size() + 1];
  for (size_t i = 0; i < vec.size(); i++) {
    ptr[i] = utils::StrToCharPtr(vec[i]);
  }
  ptr[vec.size()] = NULL;
  return ptr;
}
std::string ToStr(const Method &m) {
  if (m == kDelete) return "DELETE";
  if (m == kPost) return "POST";
  if (m == kGet) return "GET";
  return "";
}

bool StartWith(const std::string &str, const std::string &prefix) {
  return std::equal(prefix.begin(), prefix.end(), str.begin());
}

//   IMF-fixdate  = day-name "," SP date1 SP time-of-day SP GMT
//  ; fixed length/zone/capitalization subset of the format
//  ; see Section 3.3 of [RFC5322]
std::string ConvertTimeToString(const time_t &time) {
  struct tm timeinfo;
  char buffer[80];

  localtime_r(&time, &timeinfo);

  strftime(buffer, 80, "%a, %d %b %Y %H:%M:%S GMT", &timeinfo);
  std::string str(buffer);
  return str;
}

std::string GetCurrentDate() {
  time_t rawtime;

  time(&rawtime);

  return utils::ConvertTimeToString(rawtime);
}

}  // namespace utils
