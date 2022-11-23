// #include <gtest/gtest.h>

// #include "Server.hpp"

// #define private public

// void init(LocationContext *lc) { lc->root = "/usr/local/www/nginx"; }

// TEST(Path, 1) {
//   LocationContext lc;
//   init(&lc);
//   std::map<std::string, LocationContext> mapList = {
//       {"/", lc},
//       {"/aa/", lc},
//       {"/bb/", lc},
//       {"/cc/", lc},
//   };

//   ParsedRequest pr;
//   pr.request_path = "/a/bbb/abc.html";

//   std::vector<ServerContext> contexts;
//   Server serv(contexts);

//   serv.receive_request_.SetParsedRequest(pr);
//   std::string str = serv.ReplaceURILocation(mapList);
//   EXPECT_EQ("/usr/local/www/nginx/a/bbb/abc.html", str);
// }
