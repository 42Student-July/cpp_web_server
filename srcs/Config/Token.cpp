#include "Token.hpp"

#include "Parser.hpp"
Token::Token() {}
Token::Token(std::string str, size_t line) : data_(str), line_(line) {}
Token::Token(const Token& token) { *this = token; }
Token::~Token() {}
Token& Token::operator=(const Token& token) {
  if (this == &token) return *this;
  data_ = token.data_;
  line_ = token.line_;
  return *this;
}
std::string Token::GetData() const { return data_; }
size_t Token::GetLine() const { return line_; }
void Token::Set(const std::string& str) { data_ = str; }
