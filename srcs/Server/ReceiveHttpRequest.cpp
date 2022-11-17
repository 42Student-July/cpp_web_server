#include "ReceiveHttpRequest.hpp"
ReceiveHttpRequest::ReceiveHttpRequest() {
  fd_data_.s = UNREAD;
  pr_.m = ERROR;
  pr_.status_code = 0;
}

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
  size_t key_pos = 0;
  size_t val_pos = 0;

  key_pos = line.find(':');
  val_pos = key_pos;
  while (isspace(line[key_pos]) != 0 && key_pos > 0) {
    key_pos--;
  }
  while (isspace(line[val_pos + 1]) != 0) {
    val_pos++;
  }
  key = line.substr(0, key_pos);
  value = line.substr(val_pos + 1);

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
                                              parsed_request *pr) {
  size_t pos = 0;
  ssize_t read_ret = 0;
  char buf[BUFFER_SIZE];

  read_ret = read(fd, buf, BUFFER_SIZE);
  if (read_ret == -1) {
    return READ_ERROR;
  }
  buf[read_ret] = '\0';
  fd_data_.buf += buf;
  if (fd_data_.s == UNREAD || fd_data_.s == WAIT_REQUEST) {
    pos = fd_data_.buf.find(NL);
    if (std::string::npos != pos) {
      fd_data_.request_line = TrimByPos(&fd_data_.buf, pos, 2);
      if (InputHttpRequestLine(fd_data_.request_line, &pr_) == ERROR) {
        fd_data_.s = ERROR_REQUEST;
      } else {
        fd_data_.s = WAIT_HEADER;
      }
    } else {
      fd_data_.s = WAIT_REQUEST;
      *pr = pr_;
      return WAIT_REQUEST;
    }
  }

  if (fd_data_.s == WAIT_HEADER) {
    pos = fd_data_.buf.find(NLNL);
    if (std::string::npos != pos) {
      fd_data_.request_header = TrimByPos(&fd_data_.buf, pos, 4);
      pr_.request_header = ParseRequestHeader(fd_data_.request_header);
      fd_data_.s = WAIT_BODY;
    } else {
      fd_data_.s = WAIT_HEADER;
      *pr = pr_;
      return WAIT_HEADER;
    }
  }

  if (fd_data_.s == WAIT_BODY) {
    pos = fd_data_.buf.find(NL);
    pr_.request_body = TrimByPos(&fd_data_.buf, pos, 2);
  }
  *pr = pr_;
  return READ_COMPLETE;
}

std::string ReceiveHttpRequest::GetBuf() { return (fd_data_.buf); }
