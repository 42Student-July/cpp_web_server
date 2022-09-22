#ifndef SERVER_HPP
#define SERVER_HPP
#include <iostream>
#include <sys/select.h>
#include <stdio.h>
#include <fcntl.h>
#include "Epoll.hpp"
#include "ListenFd.hpp"
#include "RioFileDescriptor.hpp"
#include "./utils/Fd.hpp"
#include "./utils/Result.hpp"
#include "ParseRequestMessage.hpp"
#include "HttpResponse.hpp"
#define string std::string
#define cout std::cout
#define endl std::endl
//namespace ft{
	class Server{
		private:
		static const int max_line = 8192;
		ListenFd listen_;
		Epoll epoll_;
		void CommunicatingWithClient(epoll_event& ep);
		void StartCommunicationWithClient();
		Server();
		public:
		Server(string port);
		~Server();
		void Run();
		int Echo(const Fd& fd);
	};
#endif