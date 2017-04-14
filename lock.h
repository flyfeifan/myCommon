#ifndef  _COMMON_LOCK_H
#define  _COMMON_LOCK_H

#include<stdexcept>
#include <pthread.h>
#include <sstream>
#include <semaphore.h>

namespace Common{
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

template <typename MYLOCK>
class TempGuard
{
	public:
		TempGuard(MYLOCK &lock):_lock(lock){
			_lock.enter();
		}

		virtual ~TempGuard(){
			_lock.leave();
		}
	private:
		MYLOCK & _lock;
		TempGuard(const TempGuard &t);
		TempGuard& operator=(const TempGuard &t);
};

template <typename WRLOCK>
class WriteGuard
{
	public:
		WriteGuard(WRLOCK &lock):_lock(lock){
			_lock.wenter();
		}

		virtual ~WriteGuard(){
			_lock.leave();
		}
	private:
		WRLOCK & _lock;
		WriteGuard(const WriteGuard &t);
		WriteGuard& operator=(const WriteGuard &t);
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
		LockGuard& operator=(const LockGuard&);
		LockGuard (const LockGuard&);
		BaseLock& m_lock;
};

//class mutex
class MutexLock : virtual public BaseLock{
	public:
		MutexLock(bool shared = false);
		virtual ~MutexLock();
		virtual void enter();
		virtual void leave();
		bool          tryenter();
	protected:
		pthread_mutex_t  _mutex;
	private:
		MutexLock(const MutexLock&);
		//MutexLock();
		MutexLock& operator=(const MutexLock&);


};
typedef TempGuard<MutexLock>  MutexGuard;

class ReadWriteLock : virtual public BaseLock{
	public:
		ReadWriteLock();
		virtual ~ReadWriteLock();
		virtual void enter(); //read lock
		virtual void leave(); //release lock
		virtual void wenter(); //write lock
	private:
		pthread_rwlock_t   _rwlock;

		ReadWriteLock(const ReadWriteLock&);
		ReadWriteLock& operator=(const ReadWriteLock&);
};
typedef TempGuard<ReadWriteLock>  RrwLockGuard;
typedef WriteGuard<ReadWriteLock>  WrwLockGuard;

class CondLock : virtual public MutexLock{
	public:
		CondLock(bool shared = false);
		virtual ~CondLock();
		virtual bool wait(int msec = 0);
		virtual bool signal();
		virtual bool broadcast();
	public:
		virtual void enter();
		virtual void leave();
		
	protected:
		pthread_cond_t   _condlock;
		//pthread_mutex_t  _mutexlock;
		//bool             _outlock;
	private:
		CondLock(const CondLock&);
		CondLock& operator=(const CondLock&);
		//CondLock();
};

class Semaphore{
	public:
		Semaphore();
		virtual ~Semaphore();
		
		bool post();
		bool wait(int mesc = 0);
	private:
		sem_t _sem;

};

}
#endif
