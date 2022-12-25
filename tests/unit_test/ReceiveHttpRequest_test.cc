#include "ReceiveHttpRequest.hpp"

#include <fcntl.h>
#include <gtest/gtest.h>

#include "Socket.hpp"

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
                        {"accept-language", "ja,en-US;q=0.8,en;q=0.6"}};

Header expected_full2 = {{"host", "hoge.com"},
                         {"connection", "keep-alive"},
                         {"content-length", "173"},
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
                         {"accept-language", "ja,en-US;q=0.8,en;q=0.6"}};

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

ReadStat do_test(int &fd, ReceiveHttpRequest &rhr, ParsedRequest &pr,
                 const std::string &msg) {
  std::vector<ServerContext> sc(1);
  ReadStat rs;
  try {
    rs = rhr.ReadHttpRequest(fd, &pr, sc);
  } catch (ErrorResponse &e) {
    EXPECT_EQ(msg, e.Msg());
    std::cout << e.Msg() << std::endl;
  }
  return rs;
}

TEST(ReceiveHttpRequest, full) {
  ReceiveHttpRequest rhr;
  ParsedRequest pr;
  ReadStat rs;
  int fd = open_pseudo_socket();
  copy_fd(fd, "FullRequest");
  rs = do_test(fd, rhr, pr, "");
  EXPECT_EQ(kReadComplete, rs);
  EXPECT_EQ(kPost, pr.m);
  EXPECT_EQ("/search.html", pr.request_path);
  EXPECT_EQ("HTTP/1.1", pr.version);
  EXPECT_EQ(pr.request_header, expected_full);
  EXPECT_EQ("q=test&submitSearch=%E6%A4%9C%E7%B4%A2", pr.request_body);
  EXPECT_EQ(38, rhr.GetContentLength());
  close(fd);
}

TEST(ReceiveHttpRequest, full2) {
  ReceiveHttpRequest rhr;
  ParsedRequest pr;
  ReadStat rs;
  int fd = open_pseudo_socket();
  copy_fd(fd, "FullRequest2");
  rs = do_test(fd, rhr, pr, "");

  EXPECT_EQ(kReadComplete, rs);
  EXPECT_EQ(kPost, pr.m);
  EXPECT_EQ("/search.html", pr.request_path);
  EXPECT_EQ("HTTP/1.1", pr.version);
  EXPECT_EQ(pr.request_header, expected_full2);
  EXPECT_EQ(
      "hogehogehogehogehogehogehogehogehogehogehogehogehogehoge\r\nhogehogehoge"
      "hogehogehogehogehogehogehogehogehogehogehoge\r\nhogehogehogehogehogehoge"
      "hogehogehogehogehogehogehogehogeh",
      pr.request_body);
  EXPECT_EQ(173, rhr.GetContentLength());
  close(fd);
}

TEST(ReceiveHttpRequest, empty_then_full) {
  ReceiveHttpRequest rhr;
  ParsedRequest pr;
  ReadStat rs;
  std::vector<ServerContext> sc;

  int fd = open_pseudo_socket();
  copy_fd(fd, "EmptyRequest");
  rs = do_test(fd, rhr, pr, "");

  EXPECT_EQ(kReadNoRequest, rs);

  copy_fd(fd, "FullRequest");
  rs = do_test(fd, rhr, pr, "");

  EXPECT_EQ(kReadComplete, rs);
  EXPECT_EQ(kPost, pr.m);
  EXPECT_EQ("/search.html", pr.request_path);
  EXPECT_EQ("HTTP/1.1", pr.version);
  EXPECT_EQ(pr.request_header, expected_full);
  EXPECT_EQ("q=test&submitSearch=%E6%A4%9C%E7%B4%A2", pr.request_body);
  EXPECT_EQ(38, rhr.GetContentLength());
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
  rs = do_test(fd, rhr, pr, "");

  EXPECT_EQ(kWaitHeader, rs);
  EXPECT_EQ(kPost, pr.m);
  EXPECT_EQ("/search.html", pr.request_path);
  EXPECT_EQ("HTTP/1.1", pr.version);
  EXPECT_EQ(0, rhr.GetContentLength());
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
  rs = do_test(fd, rhr, pr, "");
  EXPECT_EQ(kWaitRequest, rs);
  EXPECT_EQ(kError, pr.m);
  EXPECT_EQ("", pr.request_path);
  EXPECT_EQ("", pr.version);
  EXPECT_EQ("POST /search.", rhr.GetBuf());

  copy_fd(fd, "HalfRequestLine2");
  rs = do_test(fd, rhr, pr, "");
  EXPECT_EQ(kWaitHeader, rs);
  EXPECT_EQ(kPost, pr.m);
  EXPECT_EQ("/search.html", pr.request_path);
  EXPECT_EQ("HTTP/1.1", pr.version);
  EXPECT_EQ("", rhr.GetBuf());
  EXPECT_EQ(0, rhr.GetContentLength());
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
  rs = do_test(fd, rhr, pr, "Invalid request line: aaaaaaaaa");
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
  rs = do_test(fd, rhr, pr, "Invalid header: User-Agent:");

  EXPECT_EQ(0, rhr.GetContentLength());
  close(fd);
}

TEST(ReceiveHttpRequest, invalid_request3) {
  ReceiveHttpRequest rhr;
  ParsedRequest pr;
  ReadStat rs;
  std::vector<ServerContext> sc;
  int fd = open_pseudo_socket();
  copy_fd(fd, "invalidrequest2");
  rs = do_test(fd, rhr, pr, "Invalid header: ho");
  close(fd);
}

TEST(ReceiveHttpRequest, invalid_request4) {
  ReceiveHttpRequest rhr;
  ParsedRequest pr;
  ReadStat rs;
  std::vector<ServerContext> sc;
  int fd = open_pseudo_socket();
  copy_fd(fd, "invalidrequest3");
  rs = do_test(fd, rhr, pr, "Invalid header: host:                        ");
  close(fd);
}

TEST(ReceiveHttpRequest, request_then_nobody) {
  ReceiveHttpRequest rhr;
  ParsedRequest pr;
  ReadStat rs;
  std::vector<ServerContext> sc;
  int fd = open_pseudo_socket();
  copy_fd(fd, "request_then_nobody");
  rs = do_test(fd, rhr, pr, "Invalid header: host a");
  EXPECT_EQ(0, rhr.GetContentLength());
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
  rs = do_test(fd, rhr, pr, "");
  EXPECT_EQ(kReadComplete, rs);
  EXPECT_EQ(0, rhr.GetContentLength());
  close(fd);
  remove("./text/ReceiveHttpRequest/pseudo_socket.txt");
}

TEST(ReceiveHttpRequest, curl2) {
  ReceiveHttpRequest rhr;
  ParsedRequest pr;
  ReadStat rs;
  std::vector<ServerContext> sc;
  int fd = open_pseudo_socket();
  copy_fd(fd, "curl2");
  rs = do_test(fd, rhr, pr, "");
  EXPECT_EQ(kReadComplete, rs);
  EXPECT_EQ(22, rhr.GetContentLength());
  close(fd);
  remove("./text/ReceiveHttpRequest/pseudo_socket.txt");
}

TEST(ReceiveHttpRequest, many_ws) {
  ReceiveHttpRequest rhr;
  ParsedRequest pr;
  ReadStat rs;
  std::vector<ServerContext> sc;
  int fd = open_pseudo_socket();
  copy_fd(fd, "request_many_ws");
  rs = do_test(fd, rhr, pr, "");
  EXPECT_EQ(kReadComplete, rs);
  EXPECT_EQ(22, rhr.GetContentLength());
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
  rs = do_test(fd, rhr, pr, "");
  EXPECT_EQ("hogehogefugapiyopfoofoofoofoofoofoofoo",
            rhr.GetParsedRequest().request_body);
  EXPECT_EQ(kDecodeComplete, rhr.GetDecodeStat());
  EXPECT_EQ(0, rhr.GetContentLength());
  close(fd);
  remove("./text/ReceiveHttpRequest/pseudo_socket.txt");
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
  rs = do_test(fd, rhr, pr, "");
  EXPECT_EQ(expext, rhr.GetParsedRequest().request_body);
  EXPECT_EQ(kDecodeComplete, rhr.GetDecodeStat());
  EXPECT_EQ(0, rhr.GetContentLength());
  close(fd);
  remove("./text/ReceiveHttpRequest/pseudo_socket.txt");
}

TEST(ReceiveHttpRequest, request_chunked_error) {
  ReceiveHttpRequest rhr;
  ParsedRequest pr;
  ReadStat rs;
  std::vector<ServerContext> sc;

  int fd = open_pseudo_socket();
  copy_fd(fd, "ChunkedRequestError");
  rs = do_test(fd, rhr, pr, "Payload Too Large");
  EXPECT_EQ(0, rhr.GetContentLength());
  close(fd);
  remove("./text/ReceiveHttpRequest/pseudo_socket.txt");
}

TEST(ReceiveHttpRequest, invalidheader) {
  ReceiveHttpRequest rhr;
  ParsedRequest pr;
  ReadStat rs;
  std::vector<ServerContext> sc;
  int fd = open_pseudo_socket();
  copy_fd(fd, "invalid_request2");
  rs = do_test(fd, rhr, pr, "Invalid header: aaaa");

  EXPECT_EQ(kPost, pr.m);
  EXPECT_EQ("/p", pr.request_path);
  EXPECT_EQ("HTTP/1.1", pr.version);
  close(fd);
  remove("./text/ReceiveHttpRequest/pseudo_socket.txt");
}

TEST(ReceiveHttpRequest, over_length) {
  ReceiveHttpRequest rhr;
  ParsedRequest pr;
  ReadStat rs;
  std::vector<ServerContext> sc;
  int fd = open_pseudo_socket();
  copy_fd(fd, "over_content_length");
  rs = do_test(fd, rhr, pr, "Payload Too Large");
  close(fd);
  remove("./text/ReceiveHttpRequest/pseudo_socket.txt");
}

TEST(ReceiveHttpRequest, length_eq) {
  ReceiveHttpRequest rhr;
  ParsedRequest pr;
  ReadStat rs;
  std::vector<ServerContext> sc;
  int fd = open_pseudo_socket();
  copy_fd(fd, "content_length_eq");
  rs = do_test(fd, rhr, pr, "Payload Too Large");
  EXPECT_EQ(kPost, pr.m);
  EXPECT_EQ("/", pr.request_path);
  EXPECT_EQ("HTTP/1.1", pr.version);
  close(fd);
  remove("./text/ReceiveHttpRequest/pseudo_socket.txt");
}
