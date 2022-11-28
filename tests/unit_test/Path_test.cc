#include "Path.hpp"

#include <gtest/gtest.h>

#include "LocationContext.hpp"

void init(LocationContext *lc, std::string str) { lc->root = str; }

TEST(Path, 1) {
  LocationContext lc;
  init(&lc, "/usr/local/www/nginx");
  Locationmap mapList = {
      {"/a/", lc},
  };


  Path p("/a/bbb/abc.html");
  std::string str = p.GetFilePath(&mapList);
  EXPECT_EQ("/usr/local/www/nginx/bbb/abc.html", str);
}

TEST(Path, 2) {
  LocationContext lc;
  init(&lc, "/usr/local/www/nginx");
  Locationmap mapList = {
      {"/a/bbb/", lc},
  };

  Path p("/a/bbb/abc.html");
  std::string str = p.GetFilePath(&mapList);
  EXPECT_EQ("/usr/local/www/nginx/abc.html", str);
}

TEST(Path, 3) {
  LocationContext lc;
  init(&lc, "/usr/local/www/nginx");
  Locationmap mapList = {
      {"/a/bbb", lc},
  };

  Path p("/a/bbb/abc.html");
  std::string str = p.GetFilePath(&mapList);
  EXPECT_EQ("/usr/local/www/nginx/abc.html", str);
}

TEST(Path, 4) {
  LocationContext lc;
  init(&lc, "/tmp/www/");
  Locationmap mapList = {
      {"/kapounet", lc},
  };

  Path p("/kapounet/pouic/toto/pounet.html");
  std::string str = p.GetFilePath(&mapList);
  EXPECT_EQ("/tmp/www/pouic/toto/pounet.html", str);
}

TEST(Path, 5) {
  LocationContext lc;
  init(&lc, "/tmp/www/");
  Locationmap mapList = {
      {"/kapounet/", lc},
  };

  Path p("/kapounet/pouic/toto/pounet.html");
  std::string str = p.GetFilePath(&mapList);
  EXPECT_EQ("/tmp/www/pouic/toto/pounet.html", str);
}

TEST(Path, 6) {
  LocationContext lc;
  init(&lc, "/tmp/www");
  Locationmap mapList = {
      {"/kapounet/", lc},
  };

  Path p("/kapounet/pouic/toto/pounet.html");
  std::string str = p.GetFilePath(&mapList);
  EXPECT_EQ("/tmp/www/pouic/toto/pounet.html", str);
}

TEST(Path, 7) {
  LocationContext lc;
  init(&lc, "/tmp/www");
  Locationmap mapList = {
      {"/kapounet", lc},
      {"/kapounet/pouic", lc},
  };
  Path p("/kapounet/pouic/toto/pounet.html");
  std::string str = p.GetFilePath(&mapList);
  EXPECT_EQ("/tmp/www/toto/pounet.html", str);
}

TEST(Path, 8) {
  LocationContext lc;
  init(&lc, "/tmp/www");
  Locationmap mapList = {
      {"/kapounet/pouic", lc},
  };
  Path p("/kapounet/pouic");
  std::string str = p.GetFilePath(&mapList);
  EXPECT_EQ("/tmp/www/", str);
}

TEST(Path, 9) {
  LocationContext lc;
  init(&lc, "/tmp/www");
  Locationmap mapList = {
      {"/kapounet/pouic/", lc},
  };
  Path p("/kapounet/pouic/");
  std::string str = p.GetFilePath(&mapList);
  EXPECT_EQ("/tmp/www/", str);
}

TEST(Path, 10) {
  LocationContext lc;
  init(&lc, "/tmp/www");
  Locationmap mapList = {
      {"/kapounet/pouic/", lc},
  };
  Path p("/kapounet/pouic/");
  std::string str = p.GetFilePath(&mapList);
  EXPECT_EQ("/tmp/www/", str);
}

// /usr/local/www/nginx/aaa/abc.html
