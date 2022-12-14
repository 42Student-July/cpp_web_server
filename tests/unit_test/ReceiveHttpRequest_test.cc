#include "ReceiveHttpRequest.hpp"

#include <fcntl.h>
#include <gtest/gtest.h>

#define DIR "./text/ReceiveHttpRequest/"

Header expected_full = {{"host", "hoge.com"},
                        {"connection", "keep-alive"},
                        {"content-length", "38"},
                        {"cache-control", "max-age=0"},
                        {"origin", "http://hoge.com"},
                        {"upgrade-insecure-requests", "1"},
                        {"user-agent", "hoge"},
                        {"content-type", "application/x-www-form-urlencoded"},
                        {"accept",
                         "text/html,application/xhtml+xml,application/"
                         "xml;q=0.9,image/webp,image/apng,*/*;q=0.8"},
                        {"referer", "http://hoge.com/index.html"},
                        {"accept-encoding", "gzip, deflate"},
                        {"accept-language", "ja,en-us;q=0.8,en;q=0.6"}};

void copy_fd(int dst, const char *src) {
  char buf[BUFFER_SIZE];
  std::string file = DIR;
  file += src;
  file += ".txt";

  int srcfd = open(file.c_str(), O_RDONLY);
  buf[read(srcfd, buf, BUFFER_SIZE)] = '\0';
  lseek(dst, 0, SEEK_SET);
  write(dst, buf, strlen(buf));
  lseek(dst, 0, SEEK_SET);
  close(srcfd);
}

int open_pseudo_socket() {
  int fd = open("./text/ReceiveHttpRequest/pseudo_socket.txt",
                O_RDWR | O_TRUNC | O_CREAT, 0644);
  return fd;
}

TEST(ReceiveHttpRequest, full) {
  ReceiveHttpRequest rhr;
  ParsedRequest pr;
  ReadStat rs;
  std::vector<ServerContext> sc;
  int fd = open_pseudo_socket();
  copy_fd(fd, "FullRequest");
  rs = rhr.ReadHttpRequest(fd, &pr, sc);

  EXPECT_EQ(kReadComplete, rs);
  EXPECT_EQ(kPost, pr.m);
  EXPECT_EQ("/search.html", pr.request_path);
  EXPECT_EQ("HTTP/1.1", pr.version);
  EXPECT_EQ(pr.request_header, expected_full);
  EXPECT_EQ("q=test&submitSearch=%E6%A4%9C%E7%B4%A2", pr.request_body);
  close(fd);
}

TEST(ReceiveHttpRequest, empty_then_full) {
  ReceiveHttpRequest rhr;
  ParsedRequest pr;
  ReadStat rs;
  std::vector<ServerContext> sc;

  int fd = open_pseudo_socket();
  copy_fd(fd, "EmptyRequest");
  rs = rhr.ReadHttpRequest(fd, &pr, sc);

  EXPECT_EQ(kWaitRequest, rs);

  copy_fd(fd, "FullRequest");
  rs = rhr.ReadHttpRequest(fd, &pr, sc);

  EXPECT_EQ(kReadComplete, rs);
  EXPECT_EQ(kReadComplete, rs);
  EXPECT_EQ(kPost, pr.m);
  EXPECT_EQ("/search.html", pr.request_path);
  EXPECT_EQ("HTTP/1.1", pr.version);
  EXPECT_EQ(pr.request_header, expected_full);
  EXPECT_EQ("q=test&submitSearch=%E6%A4%9C%E7%B4%A2", pr.request_body);
  close(fd);
  remove("./text/ReceiveHttpRequest/pseudo_socket.txt");
}

TEST(ReceiveHttpRequest, only_request_line) {
  ReceiveHttpRequest rhr;
  ParsedRequest pr;
  ReadStat rs;
  std::vector<ServerContext> sc;

  int fd = open_pseudo_socket();

  copy_fd(fd, "OnlyRequestLine");
  rs = rhr.ReadHttpRequest(fd, &pr, sc);

  EXPECT_EQ(kWaitHeader, rs);
  EXPECT_EQ(kPost, pr.m);
  EXPECT_EQ("/search.html", pr.request_path);
  EXPECT_EQ("HTTP/1.1", pr.version);
  close(fd);
  remove("./text/ReceiveHttpRequest/pseudo_socket.txt");
}

TEST(ReceiveHttpRequest, half_then_half) {
  ReceiveHttpRequest rhr;
  ParsedRequest pr;
  ReadStat rs;
  std::vector<ServerContext> sc;
  int fd = open_pseudo_socket();
  copy_fd(fd, "HalfRequestLine");
  rs = rhr.ReadHttpRequest(fd, &pr, sc);
  EXPECT_EQ(kWaitRequest, rs);
  EXPECT_EQ(kError, pr.m);
  EXPECT_EQ("", pr.request_path);
  EXPECT_EQ("", pr.version);
  EXPECT_EQ("POST /search.", rhr.GetBuf());

  copy_fd(fd, "HalfRequestLine2");
  rs = rhr.ReadHttpRequest(fd, &pr, sc);
  EXPECT_EQ(kWaitHeader, rs);
  EXPECT_EQ(kPost, pr.m);
  EXPECT_EQ("/search.html", pr.request_path);
  EXPECT_EQ("HTTP/1.1", pr.version);
  EXPECT_EQ("", rhr.GetBuf());

  close(fd);
  remove("./text/ReceiveHttpRequest/pseudo_socket.txt");
}

TEST(ReceiveHttpRequest, invalid_request1) {
  ReceiveHttpRequest rhr;
  ParsedRequest pr;
  ReadStat rs;
  std::vector<ServerContext> sc;
  int fd = open_pseudo_socket();
  copy_fd(fd, "invalidrequest1");
  rs = rhr.ReadHttpRequest(fd, &pr, sc);
  close(fd);
  remove("./text/ReceiveHttpRequest/pseudo_socket.txt");
}

TEST(ReceiveHttpRequest, invalid_request2) {
  ReceiveHttpRequest rhr;
  ParsedRequest pr;
  ReadStat rs;
  std::vector<ServerContext> sc;
  int fd = open_pseudo_socket();
  copy_fd(fd, "InvalidHeader_NoValue");
  rs = rhr.ReadHttpRequest(fd, &pr, sc);

  EXPECT_EQ(kErrorHeader, rs);
  close(fd);
}

TEST(ReceiveHttpRequest, request_then_nobody) {
  ReceiveHttpRequest rhr;
  ParsedRequest pr;
  ReadStat rs;
  std::vector<ServerContext> sc;
  int fd = open_pseudo_socket();
  copy_fd(fd, "request_then_nobody");
  rs = rhr.ReadHttpRequest(fd, &pr, sc);
  close(fd);
  remove("./text/ReceiveHttpRequest/pseudo_socket.txt");
}

TEST(ReceiveHttpRequest, curl) {
  ReceiveHttpRequest rhr;
  ParsedRequest pr;
  ReadStat rs;
  std::vector<ServerContext> sc;
  int fd = open_pseudo_socket();
  copy_fd(fd, "curl_request");
  rs = rhr.ReadHttpRequest(fd, &pr, sc);
  EXPECT_EQ(kReadComplete, rs);
  close(fd);
  remove("./text/ReceiveHttpRequest/pseudo_socket.txt");
}

TEST(ReceiveHttpRequest, request_chunked) {
  ReceiveHttpRequest rhr;
  ParsedRequest pr;
  ReadStat rs;
  std::vector<ServerContext> sc;

  int fd = open_pseudo_socket();
  copy_fd(fd, "ChunkedRequest1");
  rs = rhr.ReadHttpRequest(fd, &pr, sc);
  EXPECT_EQ("hogehogefugapiyopfoofoofoofoofoofoofoo",
            rhr.GetParsedRequest().request_body);
  EXPECT_EQ(kDecodeComplete, rhr.GetDecodeStat());
  close(fd);
}

TEST(ReceiveHttpRequest, request_chunked2) {
  ReceiveHttpRequest rhr;
  ParsedRequest pr;
  ReadStat rs;
  std::vector<ServerContext> sc;

  std::string expext =
      "<HTML>\r\n<HEAD><TITLE>Hello "
      "World</TITLE></HEAD>\r\n<BODY>\r\n<BIG>Hello "
      "World</BIG>\r\n</BODY></HTML>\r\n";

  int fd = open_pseudo_socket();
  copy_fd(fd, "ChunkedRequest2");
  rs = rhr.ReadHttpRequest(fd, &pr, sc);
  EXPECT_EQ(expext, rhr.GetParsedRequest().request_body);
  EXPECT_EQ(kDecodeComplete, rhr.GetDecodeStat());
  close(fd);
}

TEST(ReceiveHttpRequest, request_chunked_error) {
  ReceiveHttpRequest rhr;
  ParsedRequest pr;
  ReadStat rs;
  std::vector<ServerContext> sc;

  int fd = open_pseudo_socket();
  copy_fd(fd, "ChunkedRequestError");
  rs = rhr.ReadHttpRequest(fd, &pr, sc);
  EXPECT_EQ(kChunkError, rhr.GetDecodeStat());
  close(fd);
}
