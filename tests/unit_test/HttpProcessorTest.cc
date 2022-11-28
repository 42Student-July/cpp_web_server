#include <gtest/gtest.h>

#include "HttpProcessor.hpp"
#include "HttpResponse.hpp"

TEST(HttpProcessor, ProcessHttpRequest) {
  std::map<std::string, LocationContext> location_contexts;

  LocationContext location_context;
  location_context.root = "./html/";

  location_contexts["/"] = location_context;

  HttpResponse result;

  {
    ParsedRequest parsed_request;

    parsed_request.m = kGet;
    parsed_request.request_path = "/sample.html";
    parsed_request.version = "HTTP/1.1";

    HttpProcessor::ProcessHttpRequest(parsed_request, location_contexts,
                                      &result);
  }

  HttpResponse expected;
  {
    expected.SetStatusCode(200);
    expected.SetHeader("Content-Type", "text/html");
    File file("./html/sample.html");
    std::vector<std::string> file_contents = file.StoreFileLinesInVec();

    std::string body;
    for (size_t i = 0; i < file_contents.size(); ++i) {
      body += file_contents[i];
    }
    expected.SetBody(body);
    expected.SetHeader("Content-Length", std::to_string(body.size()));
  }

  EXPECT_EQ(result.GetRawResponse(), expected.GetRawResponse());
}
