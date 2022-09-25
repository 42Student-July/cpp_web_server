#include "../../srcs/RioFileDescriptor.hpp"
#include <gtest/gtest.h>
#include <fcntl.h>
#define maxline 8000
TEST(RioFd,Readnormal){
	Fd fd(open("./text/nornal.txt",O_RDONLY));
	RioFileDescriptor rio(fd);
	char buf[maxline];
	EXPECT_EQ(3, rio.ReadLineByteEach(buf,maxline));
	EXPECT_EQ(3, rio.ReadLineByteEach(buf,maxline));
	EXPECT_EQ(3, rio.ReadLineByteEach(buf,maxline));
	EXPECT_EQ(0, rio.ReadLineByteEach(buf,maxline));
}
TEST(RioFd,Readnewline){
	Fd fd(open("./text/newline.txt",O_RDONLY));
	RioFileDescriptor rio(fd);
	char buf[maxline];
	EXPECT_EQ(0, rio.ReadLineByteEach(buf,maxline));
}
TEST(RioFd,Readovermaxline){
	Fd fd(open("./text/10000.txt",O_RDONLY));
	RioFileDescriptor rio(fd);
	char buf[maxline];
	EXPECT_EQ(7999, rio.ReadLineByteEach(buf,maxline));
	EXPECT_EQ(2001, rio.ReadLineByteEach(buf,maxline));
	EXPECT_EQ(0, rio.ReadLineByteEach(buf,maxline));
}