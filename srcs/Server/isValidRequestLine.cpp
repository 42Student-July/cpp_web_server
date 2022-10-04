#include <iostream>
#include <string>

#include "HttpRequest.hpp"

method ConvertMethod(const std::string &method) {
  int i = static_cast<int>(method == "CONNECT") |
          static_cast<int>(method == "DELETE") * 2 |
          static_cast<int>(method == "GET") * 3 |
          static_cast<int>(method == "HEAD") * 4 |
          static_cast<int>(method == "OPTIONS") * 5 |
          static_cast<int>(method == "POST") * 6 |
          static_cast<int>(method == "PUT") * 7 |
          static_cast<int>(method == "TRACE") * 8;
  switch (i) {
    case 1:
      return (CONNECT);
    case 2:
      return (DELETE);
    case 3:
      return (GET);
    case 4:
      return (HEAD);
    case 5:
      return (OPTIONS);
    case 6:
      return (POST);
    case 7:
      return (PUT);
    case 8:
      return (TRACE);
    default:
      return (ERROR);
  }
}

bool isValidRequestLine(std::string *line, HttpRequestLine *rl) {
  std::size_t pos = 0;

  for (size_t i = 0; i < 3; i++) {
    pos = line->find(" ");
    switch (i) {
      case 0:
        rl->m = ConvertMethod(line->substr(0, pos));
      case 1:
        rl->path = line->substr(0, pos);
      case 2:
        rl->version = line->substr(0, pos);
    }
    while ((*line)[pos] == ' ') pos++;
    line->erase(0, pos);
  }
  return (rl->m != ERROR);
}
