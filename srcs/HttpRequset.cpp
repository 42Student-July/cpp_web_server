#include "HttpRequset.hpp"

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

std::string HttpRequest::GetRequestHeader() { return (request_header_); }

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

void HttpRequest::PerseHttpRequest(std::vector<std::string> *rec) {
  std::vector<std::string> rline;
  size_t empty = 0;

  rline = PerseRequestLine(&(*rec)[0]);
  method_ = ConvertMethod(rline[0]);
  request_path_ = rline[1];
  version_ = rline[2];
  empty = FindEmptyIndex(*rec);
  for (size_t i = 1; i < empty; i++) request_header_ += (*rec)[i];
  for (size_t i = empty + 1; i < (*rec).size(); i++) request_body_ += (*rec)[i];
}

// int main(int argc, char **argv) {
//   std::vector<std::string> input;

//   if (argc < 2)
//     input = {"POST /search.html HTTP/1.1\r\n",
//              "Host:localhost\r\n",
//              "Connection: keep-alive\r\n",
//              "Content-Length: 38\r\n",
//              "Cache-Control: max-age=0\r\n",
//              "Origin: http://wa3.i-3-i.info\r\n",
//              "Upgrade-Insecure-Requests: 1\r\n",
//              "User-Agent: hoge\r\n",
//              "Content-Type: application/x-www-form-urlencoded\r\n",
//              "Accept: "
//              "text/html,application/xhtml+xml,application/xml;q=0.9,image/"
//              "webp,image/apng,*/*;q=0.8\r\n",
//              "Referer: http://wa3.i-3-i.info/index.html\r\n",
//              "Accept-Encoding: gzip, deflate\r\n",
//              "Accept-Language: ja,en-US;q=0.8,en;q=0.6\r\n",
//              "\r\n",
//              "q=test&submitSearch=%E6%A4%9C%E7%B4%A2"};

//   HttpRequest hr(&input);
//   std::cout << "=====RequestLine=====\nMethod:\t\t" << hr.GetMethod()
//             << std::endl;
//   std::cout << "Path:\t\t" << hr.GetPath() << std::endl;
//   std::cout << "Version:\t" << hr.GetVersion() << std::endl;
//   std::cout << "\n=====Header=====\n" << hr.GetRequestHeader() << std::endl;
//   std::cout << "\n=====Body=====\n" << hr.GetRequestBody() << std::endl;
//   return 0;
// }
