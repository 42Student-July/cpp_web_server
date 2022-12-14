#include <fcntl.h>
#include <gtest/gtest.h>

#include <string>
#include <vector>

#include "File.hpp"
#define maxline 8000
// TEST(fileStatus,cantRead){
// 	File f("./text/read_perm.txt");
// 	EXPECT_EQ(f.READ_PERMISSION,f.Status());
// }
TEST(fileStatus, isdir) {
  File f("./text/is_dir");
  EXPECT_EQ(true, f.IsDir());
}
TEST(fileStatus, notFound) {
  File f("./text/a");
  EXPECT_EQ(false, f.IsExist());
}
TEST(fileStatus, ok) {
  File f("./text/ok.txt");
  EXPECT_EQ(true, f.CanRead());
}
TEST(fileRead, normal) {
  File f("./text/normal.txt");
  std::string a = "aaa\nbbb\nccc";
  EXPECT_EQ(a, f.ReadFileLines());
}
TEST(fileRead, empty) {
  File f("./text/empty.txt");
  std::string str;
  EXPECT_EQ(str, f.ReadFileLines());
}
TEST(fileRead, newline) {
  File f("./text/newline.txt");
  std::string str = "\n";
  EXPECT_EQ(str, f.ReadFileLines());
}
TEST(fileReadVec, normal) {
  File f("./text/normal.txt");
  std::string a = "aaa";
  std::string b = "bbb";
  std::string c = "ccc";
  std::vector<std::string> v;
  v.push_back(a);
  v.push_back(b);
  v.push_back(c);
  EXPECT_EQ(v, f.StoreFileLinesInVec());
}
TEST(fileReadVec, empty) {
  File f("./text/empty.txt");
  std::vector<std::string> v;
  EXPECT_EQ(v, f.StoreFileLinesInVec());
}
TEST(fileReadVec, newline) {
  File f("./text/newline.txt");
  std::string str;
  std::vector<std::string> v;
  v.push_back(str);
  EXPECT_EQ(v, f.StoreFileLinesInVec());
}
