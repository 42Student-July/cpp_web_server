#include "DecodeChunkedBody.hpp"

#include "Utils.hpp"

ChunkedBody::ChunkedBody() {
  str_head_ = 0;
  next_size_ = 0;
  stat_ = kWaitSize;
}
ChunkedBody::~ChunkedBody() {}

DecodeStat ChunkedBody::DecodeChunkedBody(std::string *request_buf) {
  buf_ += *request_buf;
  *request_buf = "";
  size_t pos = 0;

  for (;;) {
    if (stat_ == kWaitSize) {
      pos = buf_.find(NL, str_head_);
      if (std::string::npos != pos) {
        std::string size_line = buf_.substr(str_head_, pos - str_head_);
        next_size_ = utils::HexStrToLong(size_line);
        if (next_size_ == -1) {
          stat_ = kChunkError;
          break;
        }
        stat_ = kWaitChunkedBody;

        str_head_ = pos + 2;
      } else {
        stat_ = kWaitSize;
        break;
      }
    }
    if (stat_ == kWaitChunkedBody) {
      if (next_size_ == 0 && buf_.size() >= str_head_ + 2 &&
          buf_.substr(str_head_, 2) == "\r\n") {
        stat_ = kDecodeComplete;
        break;
      }
      if (buf_.size() >= str_head_) {
        decoded_body_ += buf_.substr(str_head_, next_size_);
        str_head_ += next_size_;
        stat_ = kWaitSize;
        if (buf_.substr(str_head_, 2) == "\r\n") {
          str_head_ += 2;
        } else {
          stat_ = kChunkError;
          break;
        }
      } else {
        break;
      }
    }
  }
  return stat_;
}

std::string ChunkedBody::GetDecodedBody() { return decoded_body_; }
DecodeStat ChunkedBody::GetDecodedStat() { return stat_; }
