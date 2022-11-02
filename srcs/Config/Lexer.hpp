#ifndef SRCS_CONFIG_LEXER_HPP_
#define SRCS_CONFIG_LEXER_HPP_
#include <exception>
#include <queue>
#include <stack>
#include <string>
#include <vector>

#include "File.hpp"
#include "Token.hpp"
class Lexer {
 private:
  std::string space_;
  std::string separators_;
  File file_;
  std::vector<Token> tokens_;

 public:
  explicit Lexer(std::string file_name);
  Lexer(const Lexer &l);
  Lexer &operator=(const Lexer &l);
  ~Lexer();
  void Tokenize();
  std::vector<Token> GetTokens() const;

  void DebugPrint();

 private:
  void DecomposeAndStoreLineWithDelimiters(std::string *line, size_t line_num);
  void SeparateAndStoreDelimiterCharacters(std::string *piece, size_t line_num);
  void StoreIfNotEmpty(const std::string &str, size_t line_num);
};

#endif  // SRCS_CONFIG_LEXER_HPP_
