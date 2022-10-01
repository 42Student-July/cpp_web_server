#include "File.hpp"
#include <sys/stat.h>
File::File(std::string filename):filename_(filename){}
File::~File(){}

const int File::Status()const{
	struct stat st;
	if(stat(filename_.c_str(), &st) < 0){
		return NOT_FOUND;
	}
	if(S_ISDIR(st.st_mode)){
		return IS_DIR;
	}
	if(! S_ISREG(st.st_mode) || !(S_IRUSR & st.st_mode)){
		return READ_PERMISSION;
	}
	if(! S_ISREG(st.st_mode) || !(S_IXUSR & st.st_mode)){
		return EXEC_PERMISSION;
	}
	return OK;
}