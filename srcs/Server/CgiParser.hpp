#ifndef SRCS_SERVER_CGIPARSER_HPP_
#define SRCS_SERVER_CGIPARSER_HPP_
#include <algorithm>
#include <cctype>
#include <iostream>
#include <map>
#include <string>
#include <utility>
#include <vector>

#include "Path.hpp"
#include "Socket.hpp"
#include "Utils.hpp"
enum ResponseType {
  kToBeDetermined,
  kDocumentResponse,
  kLocalRedirResponse,
  kClientRedirResponse,
  kClientRediredocResponse,
  kErrType
};
enum ParseState { kHeader, kBody };
class CgiParser {
 public:
  typedef std::pair<std::string, std::string> HeaderPair;
  typedef std::vector<HeaderPair> HeaderVec;

 private:
  static const size_t kMaxHeaderSize = 2048;
  std::string status_code_;
  // std::string cgi_response_;
  std::string header_;
  // size_t parse_pos_;
  HeaderVec header_vec_;
  std::string body_;
  ResponseType restype_;
  ParseState parse_state_;
  std::string query_;
  std::string local_path_;
  std::string new_line_;
  void ParseHeader();
  void ParseBody();
  static HeaderPair MakeHeader(const std::string &str);
  void ValidateHeader();

  bool IsValidHeader(const std::string &str);
  bool DocumentResponse();
  bool LocalRedirectResponse();
  bool ClientRedirectResponse();
  bool ClinetRedirectResponseWithDocument();
  static bool IsValidStatusCode(const std::string &str);
  bool IsExist(const std::string &key);
  void ParseQuery();

 public:
  CgiParser();
  ~CgiParser();
  void Parse(const std::string &str);
  ResponseType GetResponseType() const;
  void UpdateData(Socket *socket);
  static HeaderPair FindByKey(const HeaderVec &vec, const std::string &key);
  static std::string HeaderPairToStr(const HeaderPair &header);
};
#endif  // SRCS_SERVER_CGIPARSER_HPP_
