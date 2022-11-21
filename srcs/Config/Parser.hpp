#ifndef SRCS_CONFIG_PARSER_HPP_
#define SRCS_CONFIG_PARSER_HPP_
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>

#include <iostream>
#include <limits>
#include <map>
#include <string>
#include <utility>
#include <vector>

#include "Lexer.hpp"
#include "ServerContext.hpp"
#include "Token.hpp"
#include "TokenManager.hpp"
#include "Utils.hpp"
class Parser {
 private:
  static const long kPortMax = 65535;
  static const long kPortMin = 1;
  static const long kStatusCodeMin = 300;
  static const long kStatusCodeMax = 599;
  std::vector<ServerContext> contexts_;

  TokenManager tkns_;
  std::map<std::string, void (Parser::*)(ServerContext *sc)>
      server_directive_case_;
  std::map<std::string, void (Parser::*)(LocationContext *lc)>
      location_directive_case_;
  Parser();

 public:
  explicit Parser(const Lexer &lexer);
  ~Parser();
  void Parse();
  const std::vector<ServerContext> &ConfigSetting() const;

 private:
  void StoreServDirectives();
  LocationContext MakeLocationDirectives();

  void StoreListen(ServerContext *sc);
  void StoreClientBodySize(ServerContext *sc);
  void StoreServerName(ServerContext *sc);
  void StoreErrorPage(ServerContext *sc);
  void StoreUploadPass(ServerContext *sc);
  void StoreLocation(ServerContext *sc);

  void StoreLimitExcept(LocationContext *lc);
  void StoreRedirect(LocationContext *lc);
  void StoreRoot(LocationContext *lc);
  void StoreAutoIndex(LocationContext *lc);
  void StoreIndex(LocationContext *lc);
  void StoreCgiExtension(LocationContext *lc);
  void StoreUploadPass(LocationContext *lc);

  static std::string ParseListen(Token tkn);
  static std::string ParseHost(Token tkn);

  static void SetTokenIfEmpty(std::string *str, const Token &tkn,
                              std::string err_msg);
  void ThrowExceptionIfMatch(Token tkn, std::string str, std::string err_msg);
  void ThrowExceptionIfNotMatch(Token tkn, std::string str,
                                std::string err_msg);
  template <typename T1, typename T2>
  bool IsUnknownDirective(const std::map<T1, T2> &m, T1 key) {
    return m.find(key) == m.end();
  }
};
class ConfigErrException {
 private:
  std::string err_msg_;

 public:
  ConfigErrException(std::string msg, const Token &tkn);
  explicit ConfigErrException(std::string msg);
  ~ConfigErrException();
  std::string Msg() const throw();
};

#endif  // SRCS_CONFIG_PARSER_HPP_
