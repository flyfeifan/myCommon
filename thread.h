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
	Thread( bool detach= false);
	virtual ~Thread();
	virtual int run() = 0;
	void start();

	void join() { pthread_join(_tid, NULL); }
	int  status(){ return _status; }
protected:
	virtual void _final(){ }
protected:
	pthread_t     _tid;
private:
	sem_t         _semstart;
	bool          _wait;
	int           _status;
	//pthread_t     _tid;
};

class NativeThread    //class Thread 把未构造完成的对象指针传出，并不安全。改写此升级版本。
{
public:
	enum{ THREAD_STOP = 0, THREAD_RUNING=1 };

	static void* thread_exec(void* th);

public:
	NativeThread(bool detach= false);
	virtual ~NativeThread();
	virtual int run() = 0;
	virtual void _final() { }
	void start();
	void stop();

	int join(){ return  pthread_join(_tid, NULL); }
	int status() { return  _status; }
	pthread_t threadid(){ return _tid; }
protected:
	pthread_t _tid;
	int       _status;
	bool      _detach;

};

}


#endif
