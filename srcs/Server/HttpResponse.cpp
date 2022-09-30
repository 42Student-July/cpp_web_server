#include "HttpResponse.hpp"

#include <iostream>

#include "Utils.hpp"
#include "sys/socket.h"

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
  // TODO(akito) pair is C++03
  this->headers_.push_back(std::make_pair(key, value));
}

void HttpResponse::SetHeader(std::string const &key, int value) {
  this->SetHeader(key, utils::Itoa(value));
}

int HttpResponse::GetStatusCode() const { return this->statusCode_; }

std::string const &HttpResponse::GetBody() const { return this->body_; }

std::string HttpResponse::GetRequestLine() const {
  return this->version_ + " " + utils::Itoa(this->statusCode_) + " " +
         this->GetStatusMessage() + kCRLF;
}

std::vector<std::string> HttpResponse::GetResponseHeader() const {
  std::vector<std::string> headers;
  for (std::vector<std::pair<std::string, std::string> >::const_iterator it =
           this->headers_.begin();
       it != this->headers_.end(); ++it) {
    headers.push_back(it->first + ": " + it->second + kCRLF);
  }
  return headers;
}

std::string HttpResponse::GetStatusMessage() const {
  switch (this->statusCode_) {
    case 200:
      return "OK";
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

std::vector<std::string> HttpResponse::GetResponse() const {
  std::vector<std::string> response;
  response.push_back(this->GetRequestLine());
  for (std::vector<std::pair<std::string, std::string> >::const_iterator it =
           this->headers_.begin();
       it != this->headers_.end(); ++it) {
    response.push_back(it->first + ": " + it->second + kCRLF);
  }

  response.push_back(kCRLF);

  if (this->body_.size() > 0) {
    response.push_back(GetBody());
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