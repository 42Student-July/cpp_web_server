#include <gtest/gtest.h>

#include "HttpResponse.hpp"

TEST(HttpResponseTest_Default, StatusCode) {
  HttpResponse res;
  res.SetStatusCode(200);
  EXPECT_EQ(200, res.GetStatusCode());
}

TEST(HttpResponseTest_Default, Body) {
  HttpResponse res;
  std::string body = "Hello World";
  res.SetBody(body);
  EXPECT_EQ(body, res.GetBody());
}

TEST(HttpResponseTest_Default, Header) {
  HttpResponse res;
  std::string key = "Content-Type";
  std::string value = "text/html";
  res.SetHeader(key, value);

  std::vector<std::string> response_headers = res.GetResponseHeaders();
  std::string expected = "Content-Type: text/html\r\n";
  EXPECT_EQ(expected, response_headers[0]);
}

TEST(HttpResponseTest_Default, Header_Multiple) {
  HttpResponse res;
  std::string key = "Content-Type";
  std::string value = "text/html";
  res.SetHeader(key, value);

  std::string key2 = "Content-Length";
  std::string value2 = "100";
  res.SetHeader(key2, value2);

  std::vector<std::string> response_headers = res.GetResponseHeaders();

  std::vector<std::string> expected;
  expected.push_back("Content-Length: 100\r\n");
  expected.push_back("Content-Type: text/html\r\n");

  EXPECT_EQ(expected, response_headers);
}

TEST(HttpResponseTest_Default, Header_Duplicated) {
  HttpResponse res;
  std::string key = "Content-Type";
  std::string value = "text/html";
  res.SetHeader(key, value);

  try {
    res.SetHeader(key, value);

    FAIL() << "Expected std::logic_error";
  } catch (std::logic_error &e) {
    EXPECT_STREQ("key already exists", e.what());
  }
}

TEST(HttpResponseTest_Default, RequestLine) {
  HttpResponse res;
  res.SetStatusCode(200);
  std::string expected = "HTTP/1.1 200 OK\r\n";

  EXPECT_EQ(expected, res.GetStatusLine());
}

TEST(HttpResponseTest_Default, RequestLine_400) {
  HttpResponse res;
  res.SetStatusCode(400);
  std::string expected = "HTTP/1.1 400 Bad Request\r\n";

  EXPECT_EQ(expected, res.GetStatusLine());
}

TEST(HttpResponseTest_Default, RequestLine_500) {
  HttpResponse res;
  res.SetStatusCode(500);
  std::string expected = "HTTP/1.1 500 Internal Server Error\r\n";

  EXPECT_EQ(expected, res.GetStatusLine());
}

TEST(HttpResponseTest_Default, GetRawResponse) {
  HttpResponse http_response;
  http_response.SetStatusCode(200);
  http_response.SetBody("Hello World");
  http_response.SetHeader("Content-Length", http_response.GetBody().size());

  std::string expected =
      "HTTP/1.1 200 OK\r\n"
      "Content-Length: 11\r\n"
      "\r\n"
      "Hello World";

  std::string res = http_response.GetRawResponse();
  EXPECT_EQ(expected, res);
}

TEST(HttpResponseTest_Default, GetRawResponse_400) {
  HttpResponse http_response;
  http_response.SetStatusCode(400);
  http_response.SetBody("");
  http_response.SetHeader("Content-Length", http_response.GetBody().size());

  std::string expected =
      "HTTP/1.1 400 Bad Request\r\n"
      "Content-Length: 0\r\n"
      "\r\n";

  std::string res = http_response.GetRawResponse();
  EXPECT_EQ(expected, res);
}
