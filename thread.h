#ifndef _COMMON_THREAD_H
#define _COMMON_THREAD_H

#include <pthread.h>
#include <semaphore.h>
#include <stdexcept>
#include <sstream>

namespace Common{

class ThreadExcept : public std::runtime_error{
public:
	explicit ThreadExcept(std::string s, std::string file, int line, int err)
		:_line(line), _filename(file), std::runtime_error(s), _errno(err){ }
	explicit ThreadExcept(std::string& s):std::runtime_error(s){ }
	virtual  ~ThreadExcept() throw (){ }
	int _line;
	std::string _filename;
	int _errno;
	virtual const char* what() const throw()
	{
		std::ostringstream formatStr;
		formatStr << "file: "<< _filename<<":"<<_line<<"errno: "<< _errno<< " message:"<<std::runtime_error::what();
		return formatStr.str().c_str();
	}
};

class Thread
{
public:
	static void* thread_exec(void* th);

public:
	Thread( bool detach= false, bool wait=false);
	virtual ~Thread();
	virtual int run() = 0;
	void start();
	int  status(){ return _status; }
protected:
	pthread_t     _tid;
private:
	sem_t         _semstart;
	bool          _wait;
	int           _status;

	//pthread_t     _tid;
};

}


#endif
