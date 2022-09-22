#ifndef HTTPRESPONCE_HPP
#define HTTPRESPONCE_HPP
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#define string std::string
class HttpResponse{
private:
	
public:
	HttpResponse();
	~HttpResponse();

	string response();
	std::vector<string> make_response200(int body_length);
};



#endif