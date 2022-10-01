#include "File.hpp"
#include <gtest/gtest.h>
#include <fcntl.h>
#define maxline 8000
// TEST(fileStatus,cantRead){
// 	File f("./text/read_perm.txt");
// 	EXPECT_EQ(f.READ_PERMISSION,f.Status());
// }
TEST(fileStatus,isdir){
	File f("./text/is_dir");
	EXPECT_EQ(f.IS_DIR,f.Status());
}
TEST(fileStatus,notFound){
	File f("./text/a");
	EXPECT_EQ(f.NOT_FOUND,f.Status());
}
TEST(fileStatus,ok){
	File f("./text/ok.txt");
	EXPECT_EQ(f.OK,f.Status());
}