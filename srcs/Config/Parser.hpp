#ifndef SRCS_CONFIG_PARSER_HPP_
#define SRCS_CONFIG_PARSER_HPP_
#include <vector>

#include "ServerContext.hpp"
class Parser {
 private:
  std::vector<ServerContext> setting_;

 public:
  Parser();
  ~Parser();
};

#endif  // SRCS_CONFIG_PARSER_HPP_
