#ifndef  _COMMON_LOCK_H
#define  _COMMON_LOCK_H

#include<stdexcept>
#include <pthread.h>
#include <sstream>

namespace CommonLock{
class LockException : public std::runtime_error{
	public:
		explicit LockException(std::string s, std::string file, int line, int err)
			:_line(line), _filename(file), std::runtime_error(s), _errno(err){ }
		explicit LockException(std::string& s):std::runtime_error(s){ }
		virtual ~LockException() throw (){ }
		int _line;
		std::string _filename;
		int _errno;
		
		virtual const char* what() const throw(){
			std::ostringstream formatStr;
			formatStr << "file: "<< _filename<<":"<<_line<<" errno:" << _errno<<" message:"<<std::runtime_error::what();
			return formatStr.str().c_str();
		}
};

template <typename __LOCK>
class TempGuard
{
	public:
		TempGuard(__LOCK &lock):_lock(lock){
			_lock.enter();
		}

		virtual ~TempGuard(){
			_lock.leave();
		}
	private:
		__LOCK & _lock;
		TempGuard(const TempGuard &t);
		TempGuard &operator=(const TempGuard &t);
};

class BaseLock{
	public:
		/*
		BaseLock();
		virtual ~BaseLock();
		*/
	public:
		virtual  void enter() = 0;
		virtual  void leave() = 0;
		//virtual  bool init(){ return true;}
};

//对线程锁的控制类
class LockGuard
{
	public:
		explicit LockGuard(BaseLock& lock) : m_lock(lock){
			m_lock.enter();
		}

		~LockGuard(){
			m_lock.leave();
		}
	private:
		LockGuard operator=(const LockGuard&);
		LockGuard (const LockGuard&);
		BaseLock& m_lock;
};

//class mutex
class MutexLock : virtual public BaseLock{
	public:
		MutexLock();
		virtual ~MutexLock();
		virtual void enter();
		virtual void leave();
		bool          tryenter();
	private:
		pthread_mutex_t  _mutex;

		MutexLock(const MutexLock&);
		MutexLock operator=(const MutexLock&);


};
typedef TempGuard<MutexLock>  MutexGuard;

class 

class Semaphore{
	public:
		Semaphore();
		virtual ~Semaphore();
		
	private:
		sem_t _sem;

};

}
#endif
