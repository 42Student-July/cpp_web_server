#include "ReceiveHttpRequest.hpp"

ReceiveHttpRequest::ReceiveHttpRequest() {}

ReceiveHttpRequest::ReceiveHttpRequest(ReceiveHttpRequest const &other) {
  *this = other;
}

ReceiveHttpRequest &ReceiveHttpRequest::operator=(
    ReceiveHttpRequest const &other) {
  if (this != &other) {
    (void)other;
  }
  return *this;
}

ReceiveHttpRequest::~ReceiveHttpRequest() {}

static std::string TrimByPos(std::string *buf, const size_t &pos,
                             const size_t &size) {
  std::string trim;
  trim = buf->substr(0, pos);
  *buf = buf->substr(pos + size);
  return trim;
}

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

method InputHttpRequestLine(const std::string &line, parsed_request *pr) {
  std::size_t pos = 0;
  std::size_t top = 0;

  for (size_t i = 0; i < 3; i++) {
    pos = line.find(" ", top);
    if (i == 0) {
      pr->m = ConvertMethod(line.substr(top, pos - top));
    } else if (i == 1) {
      pr->request_path = line.substr(top, pos - top);
    } else if (i == 2) {
      pr->version = line.substr(top, pos - top);
    }
    while (line[pos] == ' ') pos++;
    top = pos;
  }
  return pr->m;
}

std::pair<std::string, std::string> SplitRequestHeaderLine(
    const std::string &line) {
  std::string key;
  std::string value;
  size_t pos = 0;

  pos = line.find(':');
  while (isspace(line[pos])) pos--;
  key = line.substr(0, pos);
  while (isspace(line[pos + 1])) pos++;
  value = line.substr(pos + 1);

  return std::make_pair(key, value);
}

HEADER ParseRequestHeader(const std::string &header_line) {
  HEADER header;
  size_t top = 0;
  size_t pos = 0;
  std::string trim;

  for (;;) {
    pos = header_line.find(NL, top);
    if (pos == std::string::npos) {
      pos = header_line.length();
    }
    trim = header_line.substr(top, pos - top);
    header.push_back(SplitRequestHeaderLine(trim));
    top = pos + 2;
    if (top >= header_line.length()) break;
  }
  return header;
}

read_stat ReceiveHttpRequest::ReadHttpRequest(const int &fd,
                                              parsed_request *request) {
  FDMAP::iterator itr;
  size_t pos = 0;
  ssize_t read_ret;
  char buf[BUFFER_SIZE];

  itr = fd_map_.find(fd);
  if (itr == fd_map_.end()) {
    fd_map_[fd].buf = "";
    fd_map_[fd].request_line = "";
    fd_map_[fd].request_header = "";
    fd_map_[fd].message_body = "";
    fd_map_[fd].s = UNREAD;
  }
  read_ret = read(fd, buf, BUFFER_SIZE);
  if (read_ret == -1) {
    this->EraseData(fd);
    return READ_ERROR;
  }
  buf[read_ret] = '\0';
  fd_map_[fd].buf += buf;
  *request = fd_map_[fd].pr;
  if (fd_map_[fd].s == UNREAD || fd_map_[fd].s == WAIT_REQUEST) {
    pos = fd_map_[fd].buf.find(NL);
    if (std::string::npos != pos) {
      fd_map_[fd].request_line = TrimByPos(&fd_map_[fd].buf, pos, 2);

      if (InputHttpRequestLine(fd_map_[fd].request_line, &fd_map_[fd].pr) ==
          ERROR) {
        fd_map_[fd].s = ERROR_REQUEST;
      } else {
        fd_map_[fd].s = WAIT_HEADER;
      }
    } else {
      fd_map_[fd].s = WAIT_REQUEST;
      return WAIT_REQUEST;
    }
  } else {
    fd_map_[fd].s = WAIT_REQUEST;
    *request = fd_map_[fd].pr;
    return WAIT_REQUEST;
  }

  if (fd_map_[fd].s == WAIT_HEADER) {
    pos = fd_map_[fd].buf.find(NLNL);
    if (std::string::npos != pos) {
      fd_map_[fd].request_header = TrimByPos(&fd_map_[fd].buf, pos, 4);
      fd_map_[fd].pr.request_header =
          ParseRequestHeader(fd_map_[fd].request_header);
      fd_map_[fd].s = WAIT_BODY;
    } else {
      fd_map_[fd].s = WAIT_HEADER;
      *request = fd_map_[fd].pr;
      return WAIT_HEADER;
    }
  }

  if (fd_map_[fd].s == WAIT_BODY) {
    pos = fd_map_[fd].buf.find(NL);
    if (std::string::npos != pos) {
      fd_map_[fd].pr.request_body = fd_map_[fd].buf.substr(0, pos);
    } else {
      *request = fd_map_[fd].pr;
      return WAIT_BODY;
    }
  }

  *request = fd_map_[fd].pr;
  return READ_COMPLETE;
}

void ReceiveHttpRequest::EraseData(const int &fd) { fd_map_.erase(fd); }

void ReceiveHttpRequest::ShowParsedRequest(const int &fd) {
  parsed_request req = fd_map_[fd].pr;
  const std::string me[9] = {"ERROR",   "CONNECT", "DELETE", "GET",  "HEAD",
                             "OPTIONS", "POST",    "PUT",    "TRACE"};

  std::cout << me[req.m] << std::endl;
  std::cout << req.request_path << std::endl;
  std::cout << req.version << std::endl;
  for (size_t i = 0; i < req.request_header.size(); ++i) {
    std::cout << req.request_header[i].first << ":"
              << req.request_header[i].second << "\n";
  }
  std::cout << req.request_body << std::endl;
}

std::string ReceiveHttpRequest::GetBuf(const int &fd) {
  return (fd_map_[fd].buf);
}
