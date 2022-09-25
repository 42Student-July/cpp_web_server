#include "HttpRequest.hpp"

HttpRequest::HttpRequest() {
  method_ = ERROR;
  status_code_ = 500;
}

HttpRequest::HttpRequest(std::vector<std::string> *rec) {
  method_ = ERROR;
  status_code_ = 500;
  this->PerseHttpRequest(rec);
}

HttpRequest::~HttpRequest() {}

method HttpRequest::GetMethod() { return (method_); }

std::string HttpRequest::GetVersion() { return (version_); }

void HttpRequest::SetStatusCode(const int &s) { status_code_ = s; }

int HttpRequest::GetStatusCode() { return (status_code_); }

std::string HttpRequest::GetPath() { return (request_path_); }

std::vector<std::pair<std::string, std::string> >
HttpRequest::GetRequestHeader() {
  return (request_header_);
}

std::string HttpRequest::GetRequestBody() { return (request_body_); }

std::vector<std::string> PerseRequestLine(std::string *rec) {
  std::vector<std::string> rl;
  std::size_t pos = 0;

  pos = (*rec).find("\r\n");
  (*rec).erase(pos, (*rec).size());
  for (int i = 0; i < 3; i++) {
    pos = (*rec).find(" ");
    rl.push_back((*rec).substr(0, pos));
    (*rec).erase(0, pos + 1);
  }
  return rl;
}

method ConvertMethod(const std::string &s) {
  int i = static_cast<int>(s == "GET") | static_cast<int>(s == "DELETE") * 2 |
          static_cast<int>(s == "POST") * 3;

  switch (i) {
    case 1:
      return (GET);
    case 2:
      return (DELETE);
    case 3:
      return (POST);
    default:
      return (ERROR);
  }
  return (ERROR);
}

size_t FindEmptyIndex(const std::vector<std::string> &rec) {
  for (size_t i = 0; i < rec.size(); i++) {
    if (rec[i] == "\r\n") return (i);
  }
  return -1;
}

std::pair<std::string, std::string> SplitRequestHeaderLine(
    const std::string &line) {
  size_t i = 1;
  std::string key;
  std::string value;
  size_t pos = 0;

  pos = line.find(":");
  key = line.substr(0, pos);
  while (line[pos + i] == ' ' && line[pos + i] != '\0') i++;
  value = &line[pos + i];
  value.erase(value.find("\r\n"));
  std::pair<std::string, std::string> pair = std::make_pair(key, value);
  return pair;
}

HEADER PerseRequestHeader(std::vector<std::string> *rec, size_t end) {
  HEADER header;

  for (size_t i = 0; i < end; i++) {
    header.push_back(SplitRequestHeaderLine((*rec)[i]));
  }
  return header;
}

void HttpRequest::PerseHttpRequest(std::vector<std::string> *rec) {
  std::vector<std::string> rline;
  size_t empty = 0;

  rline = PerseRequestLine(&(*rec)[0]);
  (*rec).erase((*rec).begin());
  method_ = ConvertMethod(rline[0]);
  request_path_ = rline[1];
  version_ = rline[2];
  empty = FindEmptyIndex(*rec);
  PerseRequestHeader(rec, empty);
  for (size_t i = empty + 1; i < (*rec).size(); i++) request_body_ += (*rec)[i];
}
