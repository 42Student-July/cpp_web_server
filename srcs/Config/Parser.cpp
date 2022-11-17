#include "Parser.hpp"

namespace {
std::string UIntToString(size_t num) {
  std::ostringstream oss;

  oss << num;
  return oss.str();
}

std::string ConfigErrMessage(const std::string &msg, const Token &tkn) {
  size_t line_num = tkn.GetLine() + 1;
  return msg + " `" + tkn.GetData() + "` :" + UIntToString(line_num) +
         " line Error";
}
}  // namespace

Parser::Parser(const Lexer &lexer) : tkns_(lexer.GetTokens()) {
  server_directive_case_["listen"] = &Parser::StoreListen;
  server_directive_case_["client_max_body_size"] = &Parser::StoreClientBodySize;
  server_directive_case_["server_name"] = &Parser::StoreServerName;
  server_directive_case_["error_page"] = &Parser::StoreErrorPage;
  server_directive_case_["location"] = &Parser::StoreLocation;
  location_directive_case_["limit_except"] = &Parser::StoreLimitExcept;
  location_directive_case_["return"] = &Parser::StoreRedirect;
  location_directive_case_["root"] = &Parser::StoreRoot;
  location_directive_case_["autoindex"] = &Parser::StoreAutoIndex;
  location_directive_case_["index"] = &Parser::StoreIndex;
  location_directive_case_["cgi_extension"] = &Parser::StoreCgiExtension;
  location_directive_case_["upload_pass"] = &Parser::StoreUploadPass;
}
Parser::~Parser() {}
const std::vector<ServerContext> &Parser::ConfigSetting() const {
  return contexts_;
}

void Parser::Parse() {
  if (tkns_.Empty()) throw ConfigErrException("config file empty");
  while (!tkns_.End()) {
    if (!TokenManager::Equal(tkns_.Current(), "server"))
      throw ConfigErrException("`server` not found", tkns_.Current());
    tkns_.Next();
    if (!TokenManager::Equal(tkns_.Current(), "{"))
      throw ConfigErrException("parentheses `{` not found", tkns_.Current());
    StoreServDirectives();
    tkns_.Increment();
  }
}

void Parser::StoreServDirectives() {
  ServerContext sc;
  while (!TokenManager::Equal(tkns_.Next(), "}")) {
    if (IsUnknownDirective(server_directive_case_, tkns_.Data()))
      throw ConfigErrException("unknown directive", tkns_.Current());
    (this->*server_directive_case_[tkns_.Data()])(&sc);
  }
  contexts_.push_back(sc);
}

LocationContext Parser::MakeLocationDirectives() {
  LocationContext lc;
  while (!TokenManager::Equal(tkns_.Next(), "}")) {
    if (IsUnknownDirective(location_directive_case_, tkns_.Data()))
      throw ConfigErrException("unknown directive", tkns_.Current());
    (this->*location_directive_case_[tkns_.Data()])(&lc);
  }
  return lc;
}
void Parser::StoreListen(ServerContext *sc) {
  if (!sc->listen.first.empty() || !sc->listen.first.empty())
    throw ConfigErrException("listen directive duplicate", tkns_.Current());
  tkns_.Next();
  std::string listen = ParseListen(tkns_.Current());
  std::string host = ParseHost(tkns_.Current());
  sc->listen.first = listen;
  sc->listen.second = host;
  ThrowExceptionIfNotMatch(tkns_.Next(), ";", "semicolon `;` not found");
}

std::string Parser::ParseListen(Token tkn) {
  size_t colon_pos = 0;
  std::string listen_str = tkn.GetData();
  if ((colon_pos = listen_str.find_first_of(":")) != std::string::npos) {
    listen_str = listen_str.substr(colon_pos + 1);
  }
  long listen = utils::StrToLong(listen_str);
  if (kPortMin > listen || listen > kPortMax)
    throw ConfigErrException("listen invalid value", tkn);
  return listen_str;
}

std::string Parser::ParseHost(Token tkn) {
  size_t colon_pos = 0;
  std::string host;
  if ((colon_pos = tkn.GetData().find(":")) != std::string::npos) {
    host = tkn.GetData().substr(0, colon_pos);
    in_addr addr;
    int res = inet_aton(host.c_str(), &addr);
    if (res == 0) throw ConfigErrException("host invalid value", tkn);
  }
  return host;
}
void Parser::StoreClientBodySize(ServerContext *sc) {
  if (sc->client_body_size != -1)
    throw ConfigErrException("`client_max_body_size` directive duplicate",
                             tkns_.Current());
  tkns_.Next();
  long size = utils::StrToLong(tkns_.Data().c_str());
  if (size < 0)
    throw ConfigErrException("`client_max_body_size` directive invalid value",
                             tkns_.Current());
  sc->client_body_size = size;
  ThrowExceptionIfNotMatch(tkns_.Next(), ";", "semicolon `;` not found");
}
void Parser::StoreServerName(ServerContext *sc) {
  ThrowExceptionIfMatch(tkns_.Next(), ";{}",
                        "invalid arguments in `server_name` directive");
  SetTokenIfEmpty(&sc->server_name, tkns_.Current(),
                  "server_name directive duplicate");
  ThrowExceptionIfNotMatch(tkns_.Next(), ";", "semicolon `;` not found");
}
void Parser::StoreErrorPage(ServerContext *sc) {
  Token current = tkns_.Next();
  Token next = tkns_.Next();
  ThrowExceptionIfMatch(next, ";",
                        "invalid arguments in `error_page` directive");
  while (!TokenManager::Equal(next, ";")) {
    ThrowExceptionIfMatch(next, "{}",
                          "invalid arguments in `error_page` directive");
    long err_num = utils::StrToLong(current.GetData().c_str());
    if (err_num < kStatusCodeMin || kStatusCodeMax < err_num)
      throw ConfigErrException(
          "error_page directive value must be between 300 and 599", current);
    sc->error_page.insert(std::make_pair(err_num, ""));
    current = next;
    next = tkns_.Next();
  }
  ThrowExceptionIfMatch(current, "{}",
                        "invalid arguments in `error_page` directive");
  for (std::map<long, std::string>::iterator it = sc->error_page.begin();
       it != sc->error_page.end(); it++) {
    it->second = current.GetData();
  }
  ThrowExceptionIfNotMatch(tkns_.Current(), ";", "semicolon `;` not found");
}

void Parser::StoreLocation(ServerContext *sc) {
  Token tkn_location = tkns_.Next();

  ThrowExceptionIfMatch(tkn_location, "{};",
                        "invalid arguments in `location` directive");
  if (sc->locations.find(tkn_location.GetData()) != sc->locations.end())
    throw ConfigErrException("`location` directive duplicate", tkn_location);
  tkns_.Next();
  if (!TokenManager::Equal(tkns_.Current(), "{"))
    throw ConfigErrException("parentheses `{` not found", tkns_.Current());
  sc->locations.insert(
      std::make_pair(tkn_location.GetData(), MakeLocationDirectives()));
}

void Parser::StoreLimitExcept(LocationContext *lc) {
  Token tkn = tkns_.Next();
  do {
    if (tkn.GetData() == "DELETE" || tkn.GetData() == "GET" ||
        tkn.GetData() == "POST") {
      if (lc->limit_except.find(tkn.GetData()) != lc->limit_except.end())
        throw ConfigErrException("`limit_except` directive is duplicate", tkn);
      lc->limit_except.insert(tkn.GetData());
    } else {
      throw ConfigErrException("`limit_except` directive invalid method", tkn);
    }
    tkn = tkns_.Next();
  } while (!TokenManager::Equal(tkn, ";"));
  ThrowExceptionIfNotMatch(tkns_.Current(), ";", "semicolon `;` not found");
}
void Parser::StoreRedirect(LocationContext *lc) {
  if (lc->redirect.first != -1)
    throw ConfigErrException("return directive duplicate", tkns_.Current());
  long code = utils::StrToLong(tkns_.Next().GetData());
  if (999 < code || code < 0)
    throw ConfigErrException("invalid return code ", tkns_.Current());
  ThrowExceptionIfMatch(tkns_.Next(), "{}", "return directive invalid value");
  if (TokenManager::Equal(tkns_.Current(), ";")) {
    lc->redirect.first = code;
  } else {
    lc->redirect.first = code;
    lc->redirect.second = tkns_.Data();
    ThrowExceptionIfNotMatch(tkns_.Next(), ";", "semicolon `;` not found");
  }
}
void Parser::StoreRoot(LocationContext *lc) {
  tkns_.Next();
  ThrowExceptionIfMatch(tkns_.Current(), ";{}",
                        "invalid arguments in `root` directive");
  SetTokenIfEmpty(&lc->root, tkns_.Current(), "`root` directive is duplicate");
  ThrowExceptionIfNotMatch(tkns_.Next(), ";", "semicolon `;` not found");
}
void Parser::StoreAutoIndex(LocationContext *lc) {
  tkns_.Next();
  if (tkns_.Data() != "off" && tkns_.Data() != "on")
    throw ConfigErrException(
        "invalid value in `autoindex` directive, it must be `on` or `off`",
        tkns_.Current());
  SetTokenIfEmpty(&lc->auto_index, tkns_.Current(),
                  "`autoindex` directive is duplicate`");
  ThrowExceptionIfNotMatch(tkns_.Next(), ";", "semicolon `;` not found");
}
void Parser::StoreIndex(LocationContext *lc) {
  ThrowExceptionIfMatch(tkns_.Next(), ";{}",
                        "invalid arguments in `index` directive");
  while (!TokenManager::Equal(tkns_.Current(), ";")) {
    ThrowExceptionIfMatch(tkns_.Current(), "{}",
                          "invalid arguments in `index` directive");
    lc->index.push_back(tkns_.Data());
    tkns_.Next();
  }
  ThrowExceptionIfNotMatch(tkns_.Current(), ";", "semicolon `;` not found");
}
void Parser::StoreCgiExtension(LocationContext *lc) {
  ThrowExceptionIfMatch(tkns_.Next(), ";{}",
                        "invalid arguments in `cgi_extension` directive");
  while (!TokenManager::Equal(tkns_.Current(), ";")) {
    size_t pos = 0;
    if ((pos = tkns_.Data().find_last_of(".")) == std::string::npos || pos != 0)
      throw ConfigErrException("invalid arguments in `cgi_extension` directive",
                               tkns_.Current());
    lc->cgi_extension.push_back(tkns_.Data());
    tkns_.Next();
  }
  ThrowExceptionIfNotMatch(tkns_.Current(), ";", "semicolon `;` not found");
}
void Parser::StoreUploadPass(LocationContext *lc) {
  ThrowExceptionIfMatch(tkns_.Next(), ";{}",
                        "invalid arguments in `upload_pass` directive");
  SetTokenIfEmpty(&lc->upload_pass, tkns_.Current(),
                  "duplicate `upload_pass` directive");
  ThrowExceptionIfNotMatch(tkns_.Next(), ";", "semicolon `;` not found");
}
void Parser::ThrowExceptionIfMatch(Token tkn, std::string str,
                                   std::string err_msg) {
  if (tkns_.Data().find_first_of(str) != std::string::npos)
    throw ConfigErrException(err_msg, tkn);
}
void Parser::ThrowExceptionIfNotMatch(Token tkn, std::string str,
                                      std::string err_msg) {
  if (tkns_.Data().find_first_of(str) == std::string::npos)
    throw ConfigErrException(err_msg, tkn);
}

void Parser::SetTokenIfEmpty(std::string *str, const Token &tkn,
                             std::string err_msg) {
  if (!str->empty()) throw ConfigErrException(err_msg, tkn);
  *str = tkn.GetData();
}
ConfigErrException::~ConfigErrException() {}
std::string ConfigErrException::Msg() const throw() { return err_msg_; }
ConfigErrException::ConfigErrException(std::string msg, const Token &tkn)
    : err_msg_(ConfigErrMessage(msg, tkn)) {}

ConfigErrException::ConfigErrException(std::string msg) : err_msg_(msg) {}
