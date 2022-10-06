#include "Parser.hpp"
Parser::Parser() {}
Parser::~Parser() {}
std::vector<ServerContext> Parser::GetSetting() {
  ServerContext s1;
  ServerContext s2;
  s1.listen_ = 80;
  s2.listen_ = 90;
  s1.client_body_size_ = 1000;
  s2.client_body_size_ = 200;
  s1.host_ = "192.168.1.1";
  s2.host_ = "192.168.1.2";
  s1.server_name_.push_back("aa");
  s1.server_name_.push_back("bb");
  s2.server_name_.push_back("cc");
  s2.server_name_.push_back("dd");
  s1.error_page_.first.insert(403);
  s1.error_page_.first.insert(404);
  s1.error_page_.second = "error404.html";
  s2.error_page_.first.insert(500);
  s2.error_page_.first.insert(503);
  s2.error_page_.second = "error500.html";

  LocationContext a, b, c, d;
  a.location_ = "/";
  b.location_ = "/image/";
  c.location_ = "/www/";
  d.location_ = "/";
  a.auto_index_ = true;
  b.auto_index_ = false;
  c.auto_index_ = true;
  d.auto_index_ = false;

  s1.locations_.push_back(a);
  s1.locations_.push_back(b);
  s2.locations_.push_back(c);
  s2.locations_.push_back(d);
  setting_.push_back(s1);
  setting_.push_back(s2);
  return setting_;
}
