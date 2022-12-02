#include "Path.hpp"

#include <gtest/gtest.h>

#include "LocationContext.hpp"

TEST(Path, 1) {
  LocationContext lc;
  lc.root = "/usr/local/www/nginx/";
  Locationmap mapList = {
      {"/a/", lc},
  };
  std::string request = "/a/bbb/abc.html";
  LocationPair pair = Path::FindBestLocation(mapList, request);
  std::string path = Path::GetAliasPath(pair, request);
  std::string expected = "/usr/local/www/nginx/bbb/abc.html";
  EXPECT_EQ(expected, path);
}

TEST(Path, 2) {
  LocationContext lc;
  lc.root = "/usr/local/www/nginx/";
  Locationmap mapList = {
      {"/a/bbb/", lc},
  };
  std::string request = "/a/bbb/abc.html";
  LocationPair pair = Path::FindBestLocation(mapList, request);
  std::string path = Path::GetAliasPath(pair, request);
  std::string expected = "/usr/local/www/nginx/abc.html";
  EXPECT_EQ(expected, path);
}

TEST(Path, 3) {
  LocationContext lc;
  lc.root = "/usr/local/www/nginx/";
  Locationmap mapList = {
      {"/a/bbb", lc},
  };
  std::string request = "/a/bbb/abc.html";
  EXPECT_THROW(Path::FindBestLocation(mapList, request),
               Path::LocationNotFound);
}

TEST(Path, 4) {
  LocationContext lc;
  lc.root = "/tmp/www/";
  Locationmap mapList = {
      {"/kapounet", lc},
  };
  std::string request = "/kapounet/pouic/toto/pounet.html";
  EXPECT_THROW(Path::FindBestLocation(mapList, request),
               Path::LocationNotFound);
}

TEST(Path, 5) {
  LocationContext lc;
  lc.root = "/tmp/www/";
  Locationmap mapList = {
      {"/kapounet/", lc},
  };
  std::string request = "/kapounet/pouic/toto/pounet.html";
  LocationPair pair = Path::FindBestLocation(mapList, request);
  std::string path = Path::GetAliasPath(pair, request);
  std::string expected = "/tmp/www/pouic/toto/pounet.html";
  EXPECT_EQ(expected, path);
}

TEST(Path, 6) {
  LocationContext lc;
  lc.root = "/tmp/www/";
  Locationmap mapList = {
      {"/kapounet", lc},
  };
  std::string request = "/kapounet/pouic/toto/pounet.html";
  EXPECT_THROW(Path::FindBestLocation(mapList, request),
               Path::LocationNotFound);
}

TEST(Path, 7) {
  LocationContext lc;
  lc.root = "/tmp/www/";
  Locationmap mapList = {
      {"/kapounet", lc},
      {"/kapounet/pouic", lc},
  };
  std::string request = "/kapounet/pouic/toto/pounet.html";
  EXPECT_THROW(Path::FindBestLocation(mapList, request),
               Path::LocationNotFound);
}

TEST(Path, 8) {
  LocationContext lc;
  lc.root = "/tmp/www/";
  Locationmap mapList = {
      {"/kapounet/pouic", lc},
  };
  std::string request = "/kapounet/pouic";
  LocationPair pair = Path::FindBestLocation(mapList, request);
  std::string path = Path::GetAliasPath(pair, request);
  std::string expected = "/tmp/www/";
  EXPECT_EQ(expected, path);
}

// add

TEST(Path, saample) {
  LocationContext lc1, lc2;
  lc1.root = "/tmp/www";
  lc2.root = "/tmp/www2";  // match
  Locationmap mapList = {
      {"/kapounet", lc1},
      {"/kapounet/pouic", lc2},
  };

  std::string request_uri = "/kapounet/pouic";
  LocationPair lp = Path::FindBestLocation(mapList, request_uri);
  EXPECT_EQ(lc2.root, lp.second.root);
}

TEST(Path, exact_match) {
  LocationContext lc1, lc2;
  lc1.root = "/ok";  // match
  lc2.root = "/ng";
  Locationmap mapList = {
      {"/images", lc1},  // ok
      {"/", lc2},        // ng
  };

  std::string request_uri = "/images";
  LocationPair lp = Path::FindBestLocation(mapList, request_uri);
  EXPECT_EQ(lc1.root, lp.second.root);
}

TEST(Path, exact_match2) {
  LocationContext lc1, lc2;
  lc1.root = "/ng";
  lc2.root = "/ok";  // match
  Locationmap mapList = {
      {"/images", lc1},  // ng
      {"/", lc2},        // ok
  };

  std::string request_uri = "/images/index.html";
  LocationPair lp = Path::FindBestLocation(mapList, request_uri);
  EXPECT_EQ(lc2.root, lp.second.root);
}

TEST(Path, exact_match3) {
  LocationContext lc1, lc2;
  lc1.root = "/ok";  // match
  lc2.root = "/ng";
  Locationmap mapList = {
      {"/images", lc1},  // ng
      {"/", lc2},        // ok
  };

  std::string request_uri = "/images/";
  LocationPair lp = Path::FindBestLocation(mapList, request_uri);
  EXPECT_EQ(lc2.root, lp.second.root);
}

TEST(Path, exact_match4) {
  LocationContext lc1, lc2, lc3;
  lc1.root = "/ng";
  lc2.root = "/ng2";
  lc3.root = "/ok";  // match
  Locationmap mapList = {
      {"/images", lc1},   // ng
      {"/", lc2},         // ng
      {"/images/", lc3},  // ok
  };

  std::string request_uri = "/images/";
  LocationPair lp = Path::FindBestLocation(mapList, request_uri);
  EXPECT_EQ(lc3.root, lp.second.root);
}

TEST(PATH, exact_match5) {
  LocationContext lc1, lc2, lc3;
  lc1.root = "/ng";
  lc2.root = "/ng2";
  lc3.root = "/ok";  // match
  Locationmap mapList = {
      {"/images", lc1},   // ng
      {"/", lc2},         // ng
      {"/images/", lc3},  // ok
  };

  std::string request_uri = "/images/";
  LocationPair lp = Path::FindBestLocation(mapList, request_uri);
  EXPECT_EQ(lc3.root, lp.second.root);
}

TEST(PATH, longest_match1) {
  LocationContext lc1, lc2, lc3;
  Locationmap mapList = {
      {"/", lc1},          // ng
      {"/images/", lc2},   // ok
      {"/images/a", lc3},  // ng
  };

  std::string request_uri = "/images/abcdef";
  LocationPair lp = Path::FindBestLocation(mapList, request_uri);
  EXPECT_EQ(lc2.root, lp.second.root);
}

TEST(PATH, longest_match2) {
  LocationContext lc1, lc2, lc3;
  Locationmap mapList = {
      {"/", lc1},                // ng
      {"/images/", lc2},         // ok
      {"/images/abcdefg", lc3},  // ng
  };

  std::string request_uri = "/images/abcdef";
  LocationPair lp = Path::FindBestLocation(mapList, request_uri);
  EXPECT_EQ(lc2.root, lp.second.root);
}

TEST(PATH, longest_match3) {
  LocationContext lc1, lc2, lc3, lc4;
  Locationmap mapList = {
      {"/", lc1},             // ng
      {"/images/", lc2},      // ng
      {"/images/jpg/", lc3},  // ng
      {"/images/png/", lc4},  // ok
  };

  std::string request_uri = "/images/png/abcdef";
  LocationPair lp = Path::FindBestLocation(mapList, request_uri);
  EXPECT_EQ(lc4.root, lp.second.root);
}

TEST(PATH, alias_path_append) {
  LocationContext lc;
  lc.root = "/tmp/www/";
  LocationPair lp = {"/", lc};
  std::string alias_path = Path::GetAliasPath(lp, "/index.html");
  std::string expected = "/tmp/www/index.html";
  EXPECT_EQ(alias_path, expected);
}

TEST(PATH, alias_path_last_not_slashed) {
  LocationContext lc;
  lc.root = "/tmp/www";
  LocationPair lp = {"/", lc};
  std::string alias_path = Path::GetAliasPath(lp, "/index.html");
  std::string expected = "/tmp/wwwindex.html";
  EXPECT_EQ(alias_path, expected);
}

TEST(PATH, alias_path_wrap) {
  LocationContext lc;
  lc.root = "/tmp/www/";
  LocationPair lp = {"/hoge/fuga/", lc};
  std::string alias_path = Path::GetAliasPath(lp, "/hoge/fuga/piyo/index.html");
  std::string expected = "/tmp/www/piyo/index.html";
  EXPECT_EQ(alias_path, expected);
}

// /usr/local/www/nginx/aaa/abc.html
