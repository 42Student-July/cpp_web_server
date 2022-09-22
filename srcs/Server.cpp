#include "Server.hpp"
//Server::Server(){}
Server::Server(const string p):listen_(p),epoll_(){
	epoll_.Init(listen_.GetFd());
}
Server::~Server(){}

void Server::Run(){
	while(1){
		int num_fd = epoll_.Wait();
		for(int i = 0; i < num_fd;i++){
			epoll_event event = epoll_.FindEvent(i);
			if(listen_.IsConnecting(event)){
				StartCommunicationWithClient();
			}else{
				CommunicatingWithClient(event);
			}
		}
	}
}

void Server::StartCommunicationWithClient(){
	Fd connfd(listen_.AcceptFd());
	cout << "connfd :" << connfd.GetFd() << endl;
	epoll_event ev = epoll_.Create(connfd);
	epoll_.Add(ev);
}
void Server::CommunicatingWithClient(epoll_event &ep){
	Fd connfd(ep.data.fd);
	cout << "epofd :" << connfd.GetFd() << endl;
	if(Echo(connfd) == 0){
		epoll_.Del(ep);
		connfd.Close();
	}
}

int Server::Echo(const Fd &fd){
	size_t n;
	char buff[max_line];
	RioFileDescriptor rio(fd);
	//n = rio.ReadLineByteEach(buff, max_line);
	//cout << "server received " << std::min(n - 2, std::min(n -1, n))<< " bytes" << endl;
	//cout << buff << endl;
	ParseRequestMessage p(buff);
	string index = p.PathAnalyzer();
	Fd op_fd(open(index.c_str(), O_RDONLY));
	RioFileDescriptor rio1(op_fd);
	char buff1[max_line];
	std::vector<string> v;
	n = 1;
	int body_size = 0;
	while ((n = rio1.ReadLineByteEach(buff1, max_line)) != 0){
		body_size += n;
		v.push_back(string(buff1));
	}
	HttpResponse hr;
	std::vector<string> header = hr.make_response200(body_size);
	for(size_t i = 0;i < v.size();i++){
		header.push_back(v[i]);
	}
	// for(size_t i = 0;i < header.size();i++){
	// 	cout <<header[i] << std::flush;
	// }

	// 	cout << v[i] << std::flush;
	for(size_t i = 0;i < header.size();i++){
		rio.WriteNbyte(header[i].c_str(), header[i].size());
	}
	return 0;
}