#include "ReceiveHttpRequest.hpp"

#include <fcntl.h>
#include <gtest/gtest.h>
#define DIR "./text/ReceiveHttpRequest/"

Header expected_full = {{"Host", "hoge.com"},
                        {"Connection", "keep-alive"},
                        {"Content-Length", "38"},
                        {"Cache-Control", "max-age=0"},
                        {"Origin", "http://hoge.com"},
                        {"Upgrade-Insecure-Requests", "1"},
                        {"User-Agent", "hoge"},
                        {"Content-Type", "application/x-www-form-urlencoded"},
                        {"Accept",
                         "text/html,application/xhtml+xml,application/"
                         "xml;q=0.9,image/webp,image/apng,*/*;q=0.8"},
                        {"Referer", "http://hoge.com/index.html"},
                        {"Accept-Encoding", "gzip, deflate"},
                        {"Accept-Language", "ja,en-US;q=0.8,en;q=0.6"}};

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

void compare_header(Header header, Header expected_header) {
  for (size_t i = 0; i < header.size(); i++) {
    EXPECT_EQ(header.at(i).first, expected_header.at(i).first);
    EXPECT_EQ(header.at(i).second, expected_header.at(i).second);
  }
}

TEST(ReceiveHttpRequest, full) {
  ReceiveHttpRequest rhr;
  ParsedRequest pr;
  ReadStat rs;
  int fd = open("./text/ReceiveHttpRequest/ReceiveHttpRequest.txt", O_RDWR);
  copy_fd(fd, "FullRequest");
  rs = rhr.ReadHttpRequest(fd, &pr);

  EXPECT_EQ(kReadComplete, rs);
  EXPECT_EQ(kPost, pr.m);
  EXPECT_EQ("/search.html", pr.request_path);
  EXPECT_EQ("HTTP/1.1", pr.version);
  compare_header(pr.request_header, expected_full);
  EXPECT_EQ("q=test&submitSearch=%E6%A4%9C%E7%B4%A2", pr.request_body);
  close(fd);
}

TEST(ReceiveHttpRequest, empty_then_full) {
  ReceiveHttpRequest rhr;
  ParsedRequest pr;
  ReadStat rs;

  int fd = open("./text/ReceiveHttpRequest/ReceiveHttpRequest.txt",
                O_RDWR | O_TRUNC);
  copy_fd(fd, "EmptyRequest");
  rs = rhr.ReadHttpRequest(fd, &pr);

  EXPECT_EQ(kWaitRequest, rs);

  copy_fd(fd, "FullRequest");
  rs = rhr.ReadHttpRequest(fd, &pr);

  EXPECT_EQ(kReadComplete, rs);
  EXPECT_EQ(kReadComplete, rs);
  EXPECT_EQ(kPost, pr.m);
  EXPECT_EQ("/search.html", pr.request_path);
  EXPECT_EQ("HTTP/1.1", pr.version);
  compare_header(pr.request_header, expected_full);
  EXPECT_EQ("q=test&submitSearch=%E6%A4%9C%E7%B4%A2", pr.request_body);
  close(fd);
}

TEST(ReceiveHttpRequest, only_request_line) {
  ReceiveHttpRequest rhr;
  ParsedRequest pr;
  ReadStat rs;
  int fd = open("./text/ReceiveHttpRequest/ReceiveHttpRequest.txt",
                O_RDWR | O_TRUNC);

  copy_fd(fd, "OnlyRequestLine");
  rs = rhr.ReadHttpRequest(fd, &pr);

  EXPECT_EQ(kWaitHeader, rs);
  EXPECT_EQ(kPost, pr.m);
  EXPECT_EQ("/search.html", pr.request_path);
  EXPECT_EQ("HTTP/1.1", pr.version);
  close(fd);
}

TEST(ReceiveHttpRequest, half_then_half) {
  ReceiveHttpRequest rhr;
  ParsedRequest pr;
  ReadStat rs;
  int fd = open("./text/ReceiveHttpRequest/ReceiveHttpRequest.txt",
                O_RDWR | O_TRUNC);
  copy_fd(fd, "HalfRequestLine");
  rs = rhr.ReadHttpRequest(fd, &pr);
  EXPECT_EQ(kWaitRequest, rs);
  EXPECT_EQ(kError, pr.m);
  EXPECT_EQ("", pr.request_path);
  EXPECT_EQ("", pr.version);
  EXPECT_EQ("POST /search.", rhr.GetBuf());

  copy_fd(fd, "HalfRequestLine2");
  rs = rhr.ReadHttpRequest(fd, &pr);
  EXPECT_EQ(kWaitHeader, rs);
  EXPECT_EQ(kPost, pr.m);
  EXPECT_EQ("/search.html", pr.request_path);
  EXPECT_EQ("HTTP/1.1", pr.version);
  EXPECT_EQ("", rhr.GetBuf());

  close(fd);
}
