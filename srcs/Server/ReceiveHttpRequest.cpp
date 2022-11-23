#include "ReceiveHttpRequest.hpp"
ReceiveHttpRequest::ReceiveHttpRequest() {
  fd_data_.s = kUnread;
  fd_data_.pr.m = kError;
  fd_data_.pr.status_code = 0;
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

ssize_t LoopRead(const int &fd, std::string *str) {
  ssize_t buffer_remaining = 0;
  std::string strbuf = "";

  char buf[BUFFER_SIZE];
  for (;;) {
    buffer_remaining = read(fd, buf, BUFFER_SIZE);
    if (buffer_remaining == -1) {
      return -1;
    }
    strbuf = buf;
    *str += strbuf;
    if (buffer_remaining < BUFFER_SIZE) {
      break;
    }
  }
  return 0;
}

static std::string TrimByPos(std::string *buf, const size_t &pos,
                             const size_t &size) {
  std::string trim;
  trim = buf->substr(0, pos);
  *buf = buf->substr(pos + size);
  return trim;
}

Method ConvertMethod(const std::string &method) {
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
      return (kConnect);
    case 2:
      return (kDelete);
    case 3:
      return (kGet);
    case 4:
      return (kHead);
    case 5:
      return (kOptions);
    case 6:
      return (kPost);
    case 7:
      return (kPut);
    case 8:
      return (kTrace);
    default:
      return (kError);
  }
}

Method InputHttpRequestLine(const std::string &line, ParsedRequest *pr) {
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

Header ParseRequestHeader(const std::string &header_line) {
  Header header;
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

ReadStat ReceiveHttpRequest::ReadHttpRequest(const int &fd, ParsedRequest *pr) {
  size_t pos = 0;
  ssize_t read_ret = 0;
  char buf[BUFFER_SIZE];

  read_ret = read(fd, buf, BUFFER_SIZE);
  if (read_ret == -1) {
    return kReadError;
  }
  buf[read_ret] = '\0';
  fd_data_.buf += buf;
  if (fd_data_.s == kUnread || fd_data_.s == kWaitRequest) {
    pos = fd_data_.buf.find(NL);
    if (std::string::npos != pos) {
      fd_data_.request_line = TrimByPos(&fd_data_.buf, pos, 2);
      if (InputHttpRequestLine(fd_data_.request_line, &fd_data_.pr) == kError) {
        fd_data_.s = kErrorRequest;
      } else {
        fd_data_.s = kWaitHeader;
      }
    } else {
      fd_data_.s = kWaitRequest;
      *pr = fd_data_.pr;
      return kWaitRequest;
    }
  }

  if (fd_data_.s == kWaitHeader) {
    pos = fd_data_.buf.find(NLNL);
    if (std::string::npos != pos) {
      fd_data_.request_header = TrimByPos(&fd_data_.buf, pos, 4);
      fd_data_.pr.request_header = ParseRequestHeader(fd_data_.request_header);
      fd_data_.s = kWaitBody;
    } else {
      fd_data_.s = kWaitHeader;
      *pr = fd_data_.pr;
      return kWaitHeader;
    }
  }

  if (fd_data_.s == kWaitBody) {
    pos = fd_data_.buf.find(NL);
    fd_data_.pr.request_body = TrimByPos(&fd_data_.buf, pos, 2);
  }
  *pr = fd_data_.pr;
  return kReadComplete;
}

std::string ReceiveHttpRequest::GetBuf() { return (fd_data_.buf); }
ParsedRequest ReceiveHttpRequest::GetParsedRequest() const {
  return fd_data_.pr;
}
void ReceiveHttpRequest::SetParsedRequest(const ParsedRequest &pr) {
  fd_data_.pr = pr;
}
