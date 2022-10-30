#include "TokenManager.hpp"

#include "Parser.hpp"
TokenManager::TokenManager(const std::vector<Token>& tokens)
    : tokens_(tokens), tkn_itr_(tokens_.begin()) {}
TokenManager::~TokenManager() {}
bool TokenManager::CurrentTknEqual(std::string str) {
  return Current().GetData() == str;
}
bool TokenManager::CurrentTknIsIdentifier() {
  return CurrentTknEqual("{") || CurrentTknEqual("}") || CurrentTknEqual(";");
}
bool TokenManager::End() const { return tkn_itr_ == tokens_.end(); }
bool TokenManager::Empty() const { return tokens_.empty(); }
Token TokenManager::Next() {
  if (++tkn_itr_ == tokens_.end())
    throw ConfigErrException::Err("unexpected", *(--tkn_itr_));
  return *tkn_itr_;
}
void TokenManager::Increment() { tkn_itr_++; }
Token TokenManager::Current() { return *tkn_itr_; }
std::string TokenManager::Data() { return Current().GetData(); }
Token TokenManager::Prev() {
  if (tkn_itr_ == tokens_.begin())
    throw ConfigErrException::Err("unexpected", *tkn_itr_);
  tkn_itr_--;
  return *tkn_itr_;
}

bool TokenManager::Equal(Token tkn, std::string str) {
  return tkn.GetData() == str;
}
