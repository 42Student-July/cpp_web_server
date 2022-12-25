#include "HttpResponse.hpp"

#include <iostream>

#include "Utils.hpp"
#include "sys/socket.h"

namespace http {
std::string GetStatusMessage(const int error_code) {
  switch (error_code) {
    case 200:
      return "OK";
    case 201:
      return "Created";
    case 202:
      return "Accepted";
    case 204:
      return "No Content";
    case 301:
      return "Moved Permanently";
    case 302:
      return "Found";
    case 304:
      return "Not Modified";
    case 307:
      return "Temporary Redirect";
    case 400:
      return "Bad Request";
    case 403:
      return "Forbidden";
    case 404:
      return "Not Found";
    case 405:
      return "Method Not Allowed";
    case 413:
      return "Payload Too Large";
    case 500:
      return "Internal Server Error";
    case 501:
      return "Not Implemented";
    case 505:
      return "HTTP Version Not Supported";
    default:
      return "";
  }
}
}  // namespace http

const char HttpResponse::kCRLF[] = "\r\n";

HttpResponse::HttpResponse()
    : statusCode_(200), version_("HTTP/1.1"), body_("") {}

HttpResponse::HttpResponse(HttpResponse const &other) { *this = other; }

HttpResponse &HttpResponse::operator=(HttpResponse const &other) {
  if (this != &other) {
    (void)other;
  }
  return *this;
}

void HttpResponse::SetStatusCode(int status) { this->statusCode_ = status; }

void HttpResponse::SetBody(std::string const &body) { this->body_ = body; }

void HttpResponse::SetHeader(std::string const &key, std::string const &value) {
  if (headers_.find(key) != headers_.end()) {
    throw std::logic_error("key already exists");
  }
  headers_[key] = value;
}

void HttpResponse::SetHeader(std::string const &key, int value) {
  this->SetHeader(key, utils::Itoa(value));
}

int HttpResponse::GetStatusCode() const { return this->statusCode_; }

std::string const &HttpResponse::GetBody() const { return this->body_; }

/*
HTTP start-line: https://httpwg.org/specs/rfc9112.html#message.format

  start-line     = request-line / status-line
  status-line = HTTP-version SP status-code SP [ reason-phrase ]
*/
std::string HttpResponse::GetStatusLine() const {
  return this->version_ + " " + utils::Itoa(this->statusCode_) + " " +
         this->GetStatusMessage() + kCRLF;
}

std::vector<std::string> HttpResponse::GetResponseHeaders() const {
  std::vector<std::string> headers;

  for (HttpResponse::HttpHeaders::const_iterator it = this->headers_.begin();
       it != this->headers_.end(); ++it) {
    headers.push_back(it->first + ": " + it->second + kCRLF);
  }
  return headers;
}

std::string HttpResponse::GetStatusMessage() const {
  switch (this->statusCode_) {
    case 200:
      return "OK";
    case 201:
      return "Created";

    case 301:
      return "Moved Permanently";
    case 302:
      return "Found";
    case 304:
      return "Not Modified";
    case 307:
      return "Temporary Redirect";

    case 400:
      return "Bad Request";
    case 403:
      return "Forbidden";
    case 404:
      return "Not Found";
    case 405:
      return "Method Not Allowed";
    case 413:
      return "Payload Too Large";
    case 500:
      return "Internal Server Error";
    case 501:
      return "Not Implemented";
    case 505:
      return "HTTP Version Not Supported";
    default:
      return "";
  }
}

/*
HTTP message: https://httpwg.org/specs/rfc9112.html#message.format

  HTTP-message   = start-line CRLF
                   *( field-line CRLF )
                   CRLF
                   [ message-body ]
*/
std::string HttpResponse::GetRawResponse() const {
  std::string response;

  response += this->GetStatusLine();

  std::vector<std::string> headers = this->GetResponseHeaders();
  for (std::vector<std::string>::iterator it = headers.begin();
       it != headers.end(); it++) {
    response += *it;
  }
  response += kCRLF;

  if (this->body_.size() > 0) {
    response += GetBody();
  }

  return response;
}

void HttpResponse::SetHttpResponse200() {
  this->SetStatusCode(200);
  this->SetHeader("Content-Type", "text/html");
  this->SetBody("<html><body><h1>200 OK</h1></body></html>");
  this->SetHeader("Content-Length", this->GetBody().size());
}

HttpResponse::~HttpResponse() {}

void HttpResponse::SetHttpResponse(int status_code, std::string const &body,
                                   const HttpHeaders &headers) {
  this->SetStatusCode(status_code);
  this->SetBody(body);
  this->headers_.insert(headers.begin(), headers.end());
  this->SetHeader("Content-Type", "text/html");
  this->SetHeader("Connection", "close");
  this->SetHeader("Date", utils::GetCurrentDate());
  this->SetHeader("Content-Length", this->GetBody().size());
}
