#include "CgiParser.hpp"
CgiParser::CgiParser() : restype_(kToBeDetermined), parse_state_(kHeader) {}
CgiParser::~CgiParser() {}
void CgiParser::Parse(const std::string &str) {
  if (str.empty() && (restype_ == kToBeDetermined)) {
    throw ErrorResponse("cgi response empty", kKk500internalServerError);
  }
  if (parse_state_ == kHeader) {
    header_ += str;
    size_t header_end_pos = 0;
    if ((header_end_pos = header_.find("\r\n\r\n")) != std::string::npos)
      new_line_ = "\r\n";
    else if ((header_end_pos = header_.find("\n\n")) != std::string::npos)
      new_line_ = "\n";
    if (header_end_pos != std::string::npos) {
      body_ = header_.substr(header_end_pos + (new_line_.size() * 2));
      header_ = header_.substr(0, header_end_pos);
      parse_state_ = kBody;
      ParseHeader();
      ValidateHeader();
    } else if (header_.size() > kMaxHeaderSize) {
      throw ErrorResponse("header size error", kKk500internalServerError);
    }
  } else if (parse_state_ == kBody) {
    body_ += str;
  }
}
void CgiParser::ValidateHeader() {
  // 応答タイプを調べる //なかったらエラー
  if (IsExist("CONTENT-LENGTH") || IsExist("CONNECTION")) {
    restype_ = kErrType;
    throw ErrorResponse("header content-length connection exist",
                        kKk500internalServerError);
  }
  if (ClinetRedirectResponseWithDocument()) {
    restype_ = kClientRediredocResponse;
  } else if (DocumentResponse()) {
    restype_ = kDocumentResponse;
  } else if (LocalRedirectResponse()) {
    ParseQuery();
    restype_ = kLocalRedirResponse;
  } else if (ClientRedirectResponse()) {
    restype_ = kClientRedirResponse;
  } else {
    restype_ = kErrType;
    throw ErrorResponse("bad cgi resopnse", kKk500internalServerError);
  }
}
void CgiParser::ParseQuery() {
  HeaderPair p = FindByKey(header_vec_, "LOCATION");
  size_t pos = p.second.find("?");
  if (pos == std::string::npos) return;
  local_path_ = p.second.substr(0, pos);
  query_ = p.second.substr(pos + 1);
}

bool CgiParser::DocumentResponse() {
  if (header_vec_.empty() ||
      "CONTENT-TYPE" != FindByKey(header_vec_, "CONTENT-TYPE").first)
    return false;
  HeaderPair status_pair = FindByKey(header_vec_, "STATUS");
  return !(!status_pair.first.empty() &&
           !IsValidStatusCode(status_pair.second));
}

bool CgiParser::IsValidStatusCode(const std::string &str) {
  if (str.size() < 3) return false;
  size_t pos = 0;
  for (; pos < str.size(); pos++) {
    if (pos <= 2) {
      if (isalnum(str[pos]) == 0) return false;
      continue;
    }
  }
  if (pos == str.size()) return true;
  size_t text_pos = pos;
  for (; text_pos < str.size(); text_pos++) {
    if (isspace(str[text_pos]) == 0) break;
  }
  if (text_pos == pos) return false;
  for (; text_pos < str.size(); text_pos++) {
    if (isspace(str[text_pos]) != 0) return false;
  }
  return true;
}
bool CgiParser::LocalRedirectResponse() {
  return header_vec_.size() == 1 && body_.empty() &&
         header_vec_[0].first == "LOCATION" &&
         Path::IsValidPath(header_vec_[0].second) &&
         Path::IsFullPath(header_vec_[0].second);
}
bool CgiParser::ClientRedirectResponse() {
  if (header_vec_.empty()) return false;
  HeaderPair location_p = FindByKey(header_vec_, "LOCATION");
  return !location_p.first.empty() && Path::IsAbsoluteUri(location_p.second);
}
bool CgiParser::ClinetRedirectResponseWithDocument() {
  if (header_vec_.size() < 3) return false;
  HeaderPair status_p = FindByKey(header_vec_, "STATUS");
  HeaderPair content_p = FindByKey(header_vec_, "CONTENT-TYPE");
  return !status_p.first.empty() && !content_p.first.empty() &&
         status_p.second == "302 Found" && IsValidStatusCode(status_p.second) &&
         ClientRedirectResponse();
}
std::pair<std::string, std::string> CgiParser::FindByKey(
    const HeaderVec &vec, const std::string &key) {
  std::vector<std::pair<std::string, std::string> >::const_iterator it =
      vec.begin();
  for (; it != vec.end(); it++) {
    if (it->first == key) return *it;
  }
  return std::make_pair("", "");
}
bool CgiParser::IsExist(const std::string &key) {
  HeaderPair p = FindByKey(header_vec_, key);
  return !p.first.empty();
}
void CgiParser::ParseHeader() {
  std::vector<std::string> headers =
      utils::SplitWithMultipleSpecifier(header_, new_line_);
  for (std::vector<std::string>::iterator it = headers.begin();
       it != headers.end(); it++) {
    HeaderPair p = MakeHeader(*it);
    if (p.first == FindByKey(header_vec_, p.first).first)
      throw ErrorResponse("duplicate header", kKk500internalServerError);
    header_vec_.push_back(p);
  }
}
std::pair<std::string, std::string> CgiParser::MakeHeader(
    const std::string &str) {
  size_t header_key_end_pos = 0;
  size_t val_start_pos = str.find(':');
  size_t val_end_pos = 0;
  if (val_start_pos == std::string::npos)
    throw ErrorResponse("header no clon", kKk500internalServerError);
  for (; header_key_end_pos < str.size(); header_key_end_pos++) {
    if (std::isalpha(str[header_key_end_pos]) != 0 ||
        str[header_key_end_pos] == '-' || str[header_key_end_pos] == '_')
      continue;
    break;
  }
  if (header_key_end_pos == 0)
    throw ErrorResponse("header no key", kKk500internalServerError);
  for (size_t colon_pos = header_key_end_pos; colon_pos < str.size();
       colon_pos++) {
    if (isspace(str[colon_pos]) != 0) continue;
    if (str[colon_pos] == ':') break;
    throw ErrorResponse("header two key", kKk500internalServerError);
  }
  val_start_pos++;
  for (; val_start_pos < str.size(); val_start_pos++) {
    if (isspace(str[val_start_pos]) == 0) break;
  }
  for (val_end_pos = val_start_pos; val_end_pos < str.size(); val_end_pos++) {
    if (isspace(str[val_end_pos]) != 0) break;
  }
  if (val_start_pos == val_end_pos)
    throw ErrorResponse("header no val", kKk500internalServerError);
  size_t substr_end_pos = str.size() - 1;
  for (; substr_end_pos > 0; substr_end_pos--) {
    if (isspace(str[substr_end_pos]) == 0) break;
  }
  std::string key = str.substr(0, header_key_end_pos);
  transform(key.begin(), key.end(), key.begin(), ::toupper);
  return std::make_pair(
      key, str.substr(val_start_pos, substr_end_pos + 1 - val_start_pos));
  // oomozini
}
ResponseType CgiParser::GetResponseType() const { return restype_; }
void CgiParser::UpdateData(Socket *socket, size_t cgi_pos) {
  socket->response_body = body_;
  socket->pr.query_string = query_;
  socket->response_headder = header_vec_;
  socket->cgi_res[cgi_pos].type = restype_;
  if (restype_ == kLocalRedirResponse) {
    // std::cout << "local_path :" << local_path_ << std::endl;
    socket->pr.request_path = local_path_;
  }
}
std::string CgiParser::HeaderPairToStr(const HeaderPair &header) {
  std::string str = header.first;
  str += ": ";
  str += header.second + "\r\n";
  return str;
}
