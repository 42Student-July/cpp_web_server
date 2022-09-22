#ifndef FD_HPP
#define FD_HPP
#include <unistd.h>
class Fd{
	private:
	int fd_;
	Fd();
	Fd(const Fd& fd);
	Fd operator=(const Fd &fd);
	public:
	Fd(const int fd);
	~Fd();
	int GetFd()const;
	void Reset(int fd);
	void Close();
};
#endif