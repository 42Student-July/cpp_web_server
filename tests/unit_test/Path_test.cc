#include "Path.hpp"

#include <gtest/gtest.h>

#include "LocationContext.hpp"

void init(LocationContext *lc) { lc->root = "/usr/local/www/nginx"; }

TEST(Path, 1) {
  LocationContext lc;
  init(&lc);
  Locmap mapList = {
      {"/a/", lc},
      {"/aa/", lc},
      {"/bb/", lc},
      {"/cc/", lc},
  };

  Path p;
  p.SetFilePath("", "/a/bbb/abc.html");
  std::string str = p.SetLocation(mapList);
  EXPECT_EQ("/usr/local/www/nginx/bbb/abc.html", str);
}

TEST(Path, 2) {
  LocationContext lc;
  init(&lc);
  Locmap mapList = {
      {"/a/bbb/", lc},
      {"/aa/", lc},
      {"/bb/", lc},
      {"/cc/", lc},
  };

  Path p;
  p.SetFilePath("", "/a/bbb/abc.html");
  std::string str = p.SetLocation(mapList);
  EXPECT_EQ("/usr/local/www/nginx/abc.html", str);
}

// /usr/local/www/nginx/aaa/abc.html
