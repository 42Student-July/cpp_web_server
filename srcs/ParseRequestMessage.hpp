#ifndef PARSEREQUESTMESSAGE_HPP
#define PARSEREQUESTMESSAGE_HPP
#include <iostream>
#include <string>
#define string std::string
class ParseRequestMessage{
private:
	ParseRequestMessage();
	string message_;
public:
	ParseRequestMessage(const char* message);
	~ParseRequestMessage();
	string GetPath();
	string GetFilename(string & exec_file);
	string PathAnalyzer();
};


#endif //