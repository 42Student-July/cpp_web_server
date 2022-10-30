#include "RioFileDescriptor.hpp"
#include <gtest/gtest.h>
#include <fcntl.h>
#define maxline 8000
TEST(RioFd,Readnormal){
	Fd fd(open("./text/normal.txt",O_RDONLY));
	RioFileDescriptor rio(fd);
	char buf[maxline];
	rio.ReadFdIntoBuf();
	EXPECT_EQ(3, rio.ReadLineFromBuffer(buf,maxline));
	EXPECT_EQ(3, rio.ReadLineFromBuffer(buf,maxline));
	EXPECT_EQ(3, rio.ReadLineFromBuffer(buf,maxline));
	EXPECT_EQ(0, rio.ReadLineFromBuffer(buf,maxline));
}
TEST(RioFd,Readnewline){
	Fd fd(open("./text/newline.txt",O_RDONLY));
	RioFileDescriptor rio(fd);
	char buf[maxline];
	rio.ReadFdIntoBuf();
	EXPECT_EQ(0, rio.ReadLineFromBuffer(buf,maxline));
}
// TEST(RioFd,Readovermaxline){
// 	Fd fd(open("./text/10000.txt",O_RDONLY));
// 	RioFileDescriptor rio(fd);
// 	char buf[maxline];
// 	rio.ReadFdIntoBuf();
// 	EXPECT_EQ(7999, rio.ReadLineFromBuffer(buf,maxline));
// 	EXPECT_EQ(2001, rio.ReadLineFromBuffer(buf,maxline));
// 	EXPECT_EQ(0, rio.ReadLineFromBuffer(buf,maxline));
// }
// TEST(RioFd,copyassign){
// 	Fd fd(open("./text/normal.txt",O_RDONLY));
// 	RioFileDescriptor rio(fd);
// 	char buf[maxline];
// 	rio.ReadFdIntoBuf();
// 	EXPECT_EQ(3, rio.ReadLineFromBuffer(buf,maxline));
// 	RioFileDescriptor rio2(rio);
// 	EXPECT_EQ(3, rio2.ReadLineFromBuffer(buf,maxline));
// 	std::string res(buf);
// 	std::string b("bbb\n");
// 	EXPECT_EQ(b, res);
// 	EXPECT_EQ(3, rio.ReadLineFromBuffer(buf,maxline));
// }