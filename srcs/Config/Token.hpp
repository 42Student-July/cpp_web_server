#ifndef SRCS_CONFIG_TOKEN_HPP_
#define SRCS_CONFIG_TOKEN_HPP_
#include <exception>
#include <queue>
#include <sstream>
#include <string>
class Token {
 private:
  std::string data_;
  size_t line_;

 public:
  Token();
  Token(std::string str, size_t line);
  Token(const Token& token);
  ~Token();
  Token& operator=(const Token& token);
  std::string GetData() const;
  size_t GetLine() const;
  void Set(const std::string& str);
};

#endif  // SRCS_CONFIG_TOKEN_HPP_
