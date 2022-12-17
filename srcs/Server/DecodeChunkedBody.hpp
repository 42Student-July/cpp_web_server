#ifndef SRCS_SERVER_DECODECHUNKEDBODY_HPP_
#define SRCS_SERVER_DECODECHUNKEDBODY_HPP_
#include <iostream>
#include <string>

#define NL "\r\n"
enum DecodeStat { kWaitSize, kChunkError, kWaitChunkedBody, kDecodeComplete };

class ChunkedBody {
 private:
  DecodeStat stat_;
  std::string buf_;
  std::string decoded_body_;
  size_t str_head_;
  long next_size_;

 public:
  ChunkedBody();
  ~ChunkedBody();
  DecodeStat DecodeChunkedBody(std::string *request_buf);
  std::string GetDecodedBody();
  DecodeStat GetDecodedStat();
};

#endif  // SRCS_SERVER_DECODECHUNKEDBODY_HPP_
