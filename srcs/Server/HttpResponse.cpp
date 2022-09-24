#include "HttpResponse.hpp"
HttpResponse::HttpResponse() {}
HttpResponse::~HttpResponse() {}
std::vector<std::string> HttpResponse::make_response200(int body_length) {
  std::ostringstream oss;
  oss << "Content-Length: " << body_length << "\r\n";

  std::vector<std::string> header;
  header.push_back("HTTP/1.1 200 OK\r\n");
  header.push_back("Content-Type: text/html; charset=UTF-8\r\n");

  header.push_back(oss.str());
  header.push_back("Connection: Keep-Alive\r\n");
  header.push_back("\r\n");
  return header;
}
std::string HttpResponse::response() { return "HTTP/1.1 200 OK"; }
