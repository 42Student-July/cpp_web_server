#include "ReceiveHttpRequest.hpp"

#include "Utils.hpp"

static size_t CountTransferEncoding(Header *rh) {
  size_t count = 0;

  for (Header::iterator it = rh->begin(); it != rh->end(); it++) {
    if (it->first == "transfer-encoding" && it->second == "chunked") {
      count++;
    }
  }
  return count;
}

static size_t CountHeaderField(Header *rh, const std::string &key) {
  size_t count = 0;

  for (Header::iterator it = rh->begin(); it != rh->end(); it++) {
    if (it->first == key) {
      count++;
    }
  }
  return count;
}

static bool IsNeededBody(const Method &m) {
  return (!(m == kGet || m == kHead));
}

bool ReceiveHttpRequest::IsValidHeader() {
  Header rh = fd_data_.pr.request_header;
  const size_t num_of_transfer_encoding = CountTransferEncoding(&rh);
  const size_t num_of_content_length = CountHeaderField(&rh, "content-length");

  if (IsNeededBody(fd_data_.pr.m)) {
    if (num_of_transfer_encoding == 1 && num_of_content_length == 0) {
      fd_data_.is_chunked = true;
    } else if (num_of_transfer_encoding == 0 && num_of_content_length == 1) {
      fd_data_.is_chunked = false;
      std::string str = GetValueByKey("content-length");
      long l = utils::StrToLong(str);
      if (l >= 0) {
        content_size_ = l;
      } else {
        return false;
      }
    } else {
      fd_data_.pr.status_code = 400;
      return false;
    }
  }
  if (CountHeaderField(&rh, "host") != 1) {
    fd_data_.pr.status_code = 400;
    return false;
  }
  return true;
}

ReceiveHttpRequest::ReceiveHttpRequest() {
  content_size_ = 0;
  fd_data_.s = kUnread;
  fd_data_.pr.m = kError;
  fd_data_.pr.status_code = 0;
  fd_data_.is_chunked = false;
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
  std::vector<std::string> v;
  std::string request_path_buf;
  size_t pos = 0;

  v = utils::SplitWithMultipleSpecifier(line, " ");
  if (v.size() != 3) return kError;
  pr->m = ConvertMethod(v.at(0));
  request_path_buf = v.at(1);
  pos = request_path_buf.find(".cgi?");
  if (pos == std::string::npos) {
    pr->request_path = request_path_buf;
  } else {
    pr->request_path = request_path_buf.substr(0, pos + 4);
    pr->query_string = request_path_buf.substr(pos + 5);
  }
  pr->version = v.at(2);
  return pr->m;
}

std::pair<std::string, std::string> SplitRequestHeaderLine(
    const std::string &line) {
  std::string key;
  std::string value;
  size_t key_pos = 0;
  size_t val_pos = 0;

  key_pos = line.find(':');
  if (key_pos == std::string::npos) {
    throw std::exception();
  }
  val_pos = key_pos;
  while (isspace(line[key_pos]) != 0 && key_pos > 0) {
    key_pos--;
  }
  while (isspace(line[val_pos + 1]) != 0) {
    val_pos++;
  }
  key = line.substr(0, key_pos);
  value = line.substr(val_pos + 1);
  std::transform(key.begin(), key.end(), key.begin(), ::tolower);
  std::transform(value.begin(), value.end(), value.begin(), ::tolower);
  if (key.size() == 0 || value.size() == 0) throw std::exception();
  return std::make_pair(key, value);
}

Header ParseRequestHeader(const std::string &header_line) {
  Header header;
  size_t top = 0;
  size_t pos = 0;
  std::string trim;
  std::pair<std::string, std::string> p;

  for (;;) {
    pos = header_line.find(NL, top);
    if (pos == std::string::npos) {
      pos = header_line.length();
    }
    trim = header_line.substr(top, pos - top);
    try {
      p = SplitRequestHeaderLine(trim);
    } catch (...) {
      throw std::exception();
    }
    header.push_back(p);
    top = pos + 2;
    if (top >= header_line.length()) break;
  }
  return header;
}

ReadStat ReceiveHttpRequest::ReadHttpRequest(const int &fd, ParsedRequest *pr,
                                             std::vector<ServerContext> sc) {
  size_t pos = 0;
  ssize_t read_ret = 0;
  char buf[BUFFER_SIZE];
  read_ret = read(fd, buf, BUFFER_SIZE);
  if (read_ret == -1) {
    return kReadError;
  }
  if (read_ret == 0) {
    return kReadNoRequest;
  }
  buf[read_ret] = '\0';
  fd_data_.buf += buf;
  if (fd_data_.s == kUnread || fd_data_.s == kWaitRequest) {
    pos = fd_data_.buf.find(NL);
    if (std::string::npos != pos) {
      fd_data_.request_line = TrimByPos(&fd_data_.buf, pos, 2);
      if (InputHttpRequestLine(fd_data_.request_line, &fd_data_.pr) == kError) {
        fd_data_.s = kErrorRequest;
        return kErrorRequest;
      }
      fd_data_.s = kWaitHeader;
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
      try {
        fd_data_.pr.request_header =
            ParseRequestHeader(fd_data_.request_header);
      } catch (...) {
        fd_data_.s = kErrorHeader;
        return kErrorHeader;
      }
      if (IsValidHeader()) {
        sc_ = SelectServerContext(&sc);
        fd_data_.s = kWaitBody;
      } else {
        fd_data_.s = kErrorHeader;
        return kErrorHeader;
      }
    } else {
      fd_data_.s = kWaitHeader;
      *pr = fd_data_.pr;
      return kWaitHeader;
    }
  }
  if (fd_data_.s == kWaitBody && IsNeededBody(fd_data_.pr.m)) {
    if (!fd_data_.is_chunked) {
      size_t size = fd_data_.buf.length();
      if (size >= content_size_) {
        fd_data_.pr.request_body = fd_data_.buf.substr(0, content_size_);
        fd_data_.buf = "";
        *pr = fd_data_.pr;
        fd_data_.s = kReadComplete;
        return kReadComplete;
      }
    } else {
      ds_ = cb_.DecodeChunkedBody(&fd_data_.buf);
      fd_data_.pr.request_body += cb_.GetDecodedBody();
    }
  }
  *pr = fd_data_.pr;
  return kReadComplete;
}

std::string ReceiveHttpRequest::GetBuf() { return (fd_data_.buf); }
ParsedRequest ReceiveHttpRequest::GetParsedRequest() const {
  return fd_data_.pr;
}

ServerContext ReceiveHttpRequest::SelectServerContext(
    std::vector<ServerContext> *contexts) const {
  std::string hostname;
  if (contexts->size() > 1) {
    try {
      hostname = GetValueByKey("host");
    } catch (...) {
      return *contexts->begin();
    }

    for (std::vector<ServerContext>::iterator it = contexts->begin();
         it != contexts->end(); it++) {
      if (it->server_name == hostname) return *it;
    }
  }
  return *contexts->begin();
}

std::string &ReceiveHttpRequest::GetValueByKey(const std::string &key) const {
  Header h = fd_data_.pr.request_header;

  Header::iterator it = std::find_if(h.begin(), h.end(), SearchValueByKey(key));
  if (it == h.end()) {
    throw std::exception();
  }
  return it->second;
}

DecodeStat ReceiveHttpRequest::GetDecodeStat() const { return ds_; }

ServerContext ReceiveHttpRequest::GetSelectedSercerContext() const {
  return sc_;
}

size_t ReceiveHttpRequest::GetContentLength() const { return content_size_; }
