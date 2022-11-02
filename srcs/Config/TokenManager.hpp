#ifndef SRCS_CONFIG_TOKENMANAGER_HPP_
#define SRCS_CONFIG_TOKENMANAGER_HPP_
#include <string>
#include <vector>

#include "Token.hpp"
class TokenManager {
 private:
  std::vector<Token> tokens_;
  std::vector<Token>::iterator tkn_itr_;

 public:
  explicit TokenManager(const std::vector<Token>& tokens);
  ~TokenManager();
  bool CurrentTknEqual(std::string str);
  bool CurrentTknIsIdentifier();
  bool End() const;
  bool Empty() const;
  Token Next();
  Token Current();
  Token Prev();
  void Increment();
  std::string Data();

  static bool Equal(Token tkn, std::string str);
};
#endif  // SRCS_CONFIG_TOKENMANAGER_HPP_
