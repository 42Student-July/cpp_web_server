#include <gtest/gtest.h>
#include "Listen.hpp"
#include <fcntl.h>
TEST(listen, ok){
	Listen l("localhost", "8768");
	EXPECT_LT(2, l.GenerateConnectableFd());
}
TEST(listen, open_fd_over_err){
	for(int i =0; i< 5000;i++){
		int fd = open("./text/empty.txt",O_RDONLY);
	}
	Listen l("localhost", "8769");
	EXPECT_ANY_THROW(l.GenerateConnectableFd());
	for(int i =3; i< 5000;i++){
		close(i);
	}
}
TEST(listen, match_another_host_bind_err){
	Listen l("localhost", "8769");
	Listen l2("162.111.343.111", "8769");
	l.GenerateConnectableFd();
	EXPECT_ANY_THROW(l2.GenerateConnectableFd());
}

