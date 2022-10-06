#ifndef SRCS_SERVER_ISVALIDREQUESTLINE_HPP_
#define SRCS_SERVER_ISVALIDREQUESTLINE_HPP_

#include <iostream>
#include <string>

#include "HttpRequest.hpp"

bool isValidRequestLine(std::string *line, HttpRequestLine *rl);

#endif /* SRCS_SERVER_ISVALIDREQUESTLINE_HPP_ */
