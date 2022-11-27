#include <gtest/gtest.h>

#include "HttpProcessor.hpp"
#include "HttpResponse.hpp"

TEST(HttpProcessor, ProcessHttpRequest) {
  std::map<std::string, LocationContext> location_contexts;

  LocationContext location_context;
  location_context.root = "./html/sample.html";

  location_contexts["/"] = location_context;

  std::string result;

  {
    ParsedRequest parsed_request;

    parsed_request.m = kGet;
    parsed_request.request_path = "/";
    parsed_request.version = "HTTP/1.1";

    HttpProcessor::ProcessHttpRequest(parsed_request, location_contexts,
                                      &result);
  }

  std::string expected;
  {
    HttpResponse http_response;
    http_response.SetStatusCode(200);
    http_response.SetHeader("Content-Type", "text/html");
    File file("./html/sample.html");
    std::vector<std::string> file_contents = file.StoreFileLinesInVec();

    std::string body;
    for (size_t i = 0; i < file_contents.size(); ++i) {
      body += file_contents[i];
    }
    http_response.SetBody(body);
    http_response.SetHeader("Content-Length", std::to_string(body.size()));
    expected = http_response.GetRawResponse();
  }

  EXPECT_EQ(result, expected);
}
