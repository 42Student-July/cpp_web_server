#include "ReceiveHttpRequest.hpp"

#include "Socket.hpp"
#include "Utils.hpp"

#define CRLF_SIZE 2

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

static bool IsBodyRequired(const Method &m) {
  return (!(m == kGet || m == kDelete));
}

bool ReceiveHttpRequest::IsValidHeader() {
  Header rh = fd_data_.pr.request_header;
  const size_t num_of_transfer_encoding = CountTransferEncoding(&rh);
  const size_t num_of_content_length = CountHeaderField(&rh, "content-length");

  if (IsBodyRequired(fd_data_.pr.m)) {
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
      fd_data_.pr.status_code = kKk400BadRequest;
      return false;
    }
  }
  if (CountHeaderField(&rh, "host") != 1) {
    fd_data_.pr.status_code = kKk400BadRequest;
    return false;
  }
  return true;
}

ReceiveHttpRequest::ReceiveHttpRequest() {
  content_size_ = 0;
  fd_data_.s = kUnread;
  fd_data_.pr.m = kError;
  fd_data_.pr.status_code = kKkNotSet;
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

static std::string TrimByCRLF(std::string *buf, const size_t &pos) {
  std::string trim;
  trim = buf->substr(0, pos);
  *buf = buf->substr(pos + CRLF_SIZE);
  return trim;
}

Method ConvertMethod(const std::string &method) {
  int i = static_cast<int>(method == "DELETE") |
          static_cast<int>(method == "GET") * 2 |
          static_cast<int>(method == "POST") * 3;
  switch (i) {
    case 1:
      return (kDelete);
    case 2:
      return (kGet);
    case 3:
      return (kPost);
    default:
      return (kError);
  }
}

Method InputHttpRequestLine(const std::string &line, ParsedRequest *pr) {
  std::vector<std::string> v;
  std::string request_path_buf;
  v = utils::SplitWithMultipleSpecifier(line, " ");
  if (v.size() != 3) {
    throw ErrorResponse("Invalid request line", kKk400BadRequest);
  }
  pr->m = ConvertMethod(v.at(0));
  request_path_buf = v.at(1);
  size_t question_pos = request_path_buf.find_last_of("?");
  size_t last_slash_pos = request_path_buf.find_last_of("/");
  if (question_pos != std::string::npos && question_pos > last_slash_pos) {
    pr->request_path = request_path_buf.substr(0, question_pos);
    pr->query_string = request_path_buf.substr(question_pos + 1);
  } else {
    pr->request_path = request_path_buf;
  }
  pr->version = v.at(2);
  if (pr->version != "HTTP/1.1")
    throw ErrorResponse("HTTP Version Not Supported",
                        kKk505HTTPVersionNotSupported);
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
    throw ErrorResponse("Invalid header", kKk400BadRequest);
  }
  val_pos = key_pos;
  while (isspace(line[val_pos + 1]) != 0) {
    val_pos++;
  }
  key = line.substr(0, key_pos);
  value = line.substr(val_pos + 1);
  while (value.size() > 0 && isspace(value[value.size() - 1]) != 0) {
    value.erase(value.size() - 1);
  }
  std::transform(key.begin(), key.end(), key.begin(), ::tolower);
  if (key.size() == 0 || value.size() == 0)
    throw ErrorResponse("Invalid header", kKk400BadRequest);
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
    p = SplitRequestHeaderLine(trim);
    header.push_back(p);
    top = pos + 2;
    if (top >= header_line.length()) break;
  }
  return header;
}

void TrimLR(std::string *str) {
  while (str->length() > 0 && (isspace((*str)[0])) != 0) {
    str->erase(0, 1);
  }
  while (str->size() > 0 && isspace((*str)[str->size() - 1]) != 0) {
    str->erase(str->size() - 1);
  }
}

ReadStat ReceiveHttpRequest::ReadHttpRequest(const int &fd, ParsedRequest *pr,
                                             std::vector<ServerContext> sc) {
  size_t pos = 0;
  ssize_t read_ret = 0;
  char buf[BUFFER_SIZE + 1];
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
      fd_data_.request_line = TrimByCRLF(&fd_data_.buf, pos);
      InputHttpRequestLine(fd_data_.request_line, &fd_data_.pr);
      fd_data_.s = kWaitHeader;
    } else {
      fd_data_.s = kWaitRequest;
      *pr = fd_data_.pr;
      return kWaitRequest;
    }
  }

  if (fd_data_.s == kWaitHeader) {
    for (;;) {
      pos = fd_data_.buf.find(NL);
      if (std::string::npos != pos) {
        fd_data_.request_header = TrimByCRLF(&fd_data_.buf, pos);

        if (fd_data_.request_header.length() == 0) {
          if (IsValidHeader()) {
            sc_ = SelectServerContext(&sc);
            fd_data_.s = kWaitBody;
            break;
          }
        }

        std::pair<std::string, std::string> p;
        p = SplitRequestHeaderLine(fd_data_.request_header);

        std::string &key = p.first;
        std::string &value = p.second;
        if (key.length() == 0 || value.length() == 0) {
          throw ErrorResponse("Invalid header", kKk400BadRequest);
        }

        TrimLR(&value);

        std::transform(key.begin(), key.end(), key.begin(), ::tolower);

        fd_data_.pr.request_header.push_back(p);
      } else {
        fd_data_.s = kWaitHeader;
        *pr = fd_data_.pr;
        return kWaitHeader;
      }

      *pr = fd_data_.pr;
    }
  }

  if (fd_data_.s == kWaitBody && IsBodyRequired(fd_data_.pr.m)) {
    if (!fd_data_.is_chunked) {
      size_t size = fd_data_.buf.length();
      if (size >= content_size_) {
        fd_data_.pr.request_body = fd_data_.buf.substr(0, content_size_);
        fd_data_.buf = "";
        *pr = fd_data_.pr;
        fd_data_.s = kReadComplete;
      } else {
        fd_data_.s = kWaitBody;
        return kWaitBody;
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
  std::string port;
  size_t size = contexts->size();
  if (size > 1) {
    try {
      hostname = GetValueByKey("host");
      utils::Connection conn = utils::ParseHostHeader(hostname);
      hostname = conn.hostname;
      port = conn.port;
    } catch (...) {
      return *contexts->begin();
    }

    for (std::vector<ServerContext>::iterator it = contexts->begin();
         it != contexts->end(); it++) {
      if (it->server_name == hostname &&
          (port.empty() || it->listen.second == port))
        return *it;
    }
  } else if (size == 0) {
    throw ErrorResponse("Missing Server Context", kKk400BadRequest);
  }
  return *contexts->begin();
}

std::string ReceiveHttpRequest::GetValueByKey(const std::string &key) const {
  Header h = fd_data_.pr.request_header;

  Header::iterator it = std::find_if(h.begin(), h.end(), SearchValueByKey(key));
  if (it == h.end()) {
    throw ErrorResponse("Missing key", kKkNotSet);
  }
  return it->second;
}

DecodeStat ReceiveHttpRequest::GetDecodeStat() const { return ds_; }

ServerContext ReceiveHttpRequest::GetSelectedSercerContext() const {
  return sc_;
}

size_t ReceiveHttpRequest::GetContentLength() const { return content_size_; }
