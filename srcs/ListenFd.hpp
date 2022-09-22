#ifndef LISTENFD_HPP
#define LISTENFD_HPP
#include <iostream>
#include <string>
#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include "./utils/Fd.hpp"
#include "./utils/Result.hpp"
#include "Epoll.hpp"
#define string std::string
#define endl std::endl
#define cout std::cout
class ListenFd{
private:
	typedef struct addrinfo addrinfo;
	typedef struct sockaddr SA;
	typedef struct sockaddr_storage sockaddr_storage;
	static const int listen_max = 1024;
	static const int max_line = 8192;
	Fd listen_;
	addrinfo *list_top_;
	void init_hint(addrinfo& hint);
	int MakeSocket(addrinfo* addr)const;
	void SetSocketOption()const;
	bool CanBind(addrinfo *addr)const;
	ListenFd();
public:
	ListenFd(string port);
	~ListenFd();
	void GenerateConnectableFd();
	int GetFd()const;
	int	AcceptFd()const;
	bool IsConnecting(const epoll_event &event)const;
};
#endif // !LISTEN_FD_HPP#endif // !LISTEN_FD_HPP