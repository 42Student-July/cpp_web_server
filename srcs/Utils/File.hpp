#ifndef FILE_HPP
#define FILE_HPP
#include <string>
class File{
private:
	const std::string filename_;
public:
	enum{
		OK,
		NOT_FOUND,
		IS_DIR,
		EXEC_PERMISSION,
		READ_PERMISSION,

	};
	File(std::string filename);
	~File();
	const int Status()const;
};


#endif // !FILE_HPP