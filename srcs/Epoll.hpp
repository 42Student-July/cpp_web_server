#ifndef EPOLL_HPP
#define EPOLL_HPP
#include "./utils/Fd.hpp"
#include <sys/epoll.h>
#include <string.h>
#include <iostream>
#include <vector>
#define MAX_EVENT 100
class Epoll{
	private:
	//static const int max_event = 100;
	Fd epoll_;
	epoll_event events_[MAX_EVENT];
	public:
	Epoll();
	~Epoll();
	void Create();
	void Init(const Fd& connfd);
	epoll_event  FindEvent(const int& n)const;
	epoll_event Create(const Fd &connfd);
	int	Wait();
	void Del(epoll_event &ev);
	void Add(epoll_event &ev);
};

#endif // !EPOLL_HPP
