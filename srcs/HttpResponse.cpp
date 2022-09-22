#include "HttpResponse.hpp"
HttpResponse::HttpResponse(){}
HttpResponse::~HttpResponse(){}
std::vector<string> HttpResponse::make_response200(int body_length) {
    std::ostringstream oss;
    oss << "Content-Length: " << body_length << "\r\n";

    std::vector<string> header = {
        "HTTP/1.1 200 OK\r\n",
        "Content-Type: text/html; charset=UTF-8\r\n",
        oss.str(),
        "Connection: Keep-Alive\r\n",
        "\r\n"
    };
    return header;
}
string HttpResponse::response(){
	return "HTTP/1.1 200 OK";
}