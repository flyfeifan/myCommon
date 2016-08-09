#include <iostream>
#include <set>
#include "thread.h"
#include "lock.h"
#include <sys/time.h>
#include <stdlib.h>
#include <strstream>

class ThreadTack : public Common::Thread{
public:
	ThreadTack( int &count, CommonLock::CondLock &lock, int man = 0) : _count(count), _lock(lock), manger(man) { }
	virtual ~ThreadTack(){ }
	virtual int run()
	{
		//sleep(5);
		if(manger)
		{
			for(int i = 0; i < 100000; i++ )
			{
				int t = time(0) % 5;
				 //_count ++;
				 __sync_fetch_and_add(&_count, 1);
				 _lock.signal();
				 //_lock.broadcast();
				 sleep(t);
			}

		}
		else
		{
			while(1)
			{
				if(_lock.wait())
					std::cout<<"thread: " <<_tid<<" count: " << _count <<std::endl;
			}
		}
		/*
		for(int i = 0; i<10000; i++)
		{
			{
				CommonLock::LockGuard gd(_lock);
				_count ++;
			}
			{
				//CommonLock::LockGuard gd(_lock);
				CommonLock::MutexGuard gd(_lock);
				std::cout<<"thread: "<<_tid<<" i: "<<i<<" count: " << _count <<std::endl;
			}
			sleep(1);
		}*/
		return 100;
	}
private:
	int manger;
	int &_count;
	CommonLock::CondLock &_lock;


};

std::string  strCache;
CommonLock::Semaphore semaphore;

class ThreadSem : public Common::Thread{
public:
	ThreadSem(int man = 0) : manger(man) { }
	virtual ~ThreadSem(){ 
		struct timeval tm;
		gettimeofday(&tm, NULL);

		std::cout << "~ThreadSem:"<<manger<< "sec: "<< tm.tv_sec << " msce: "<< tm.tv_usec<<std::endl; }
	virtual int run()
	{
		if(manger)
		{
			for( char i = 'a'; i <= 'z'; i++)
			{
				strCache.push_back(i);
				semaphore.post();
				sleep(2);
			}
		}
		else
		{
			while( 1 ){
				if( semaphore.wait( 3010 ) ){
					struct timeval tm;
					gettimeofday(&tm, NULL);
					std::cout<<strCache<< "###sec: "<< tm.tv_sec << " msce: "<< tm.tv_usec <<std::endl;
				}else
				{
					std::cout<<"break"<<std::endl;
					break;
				}
			}
		}

		return 100;
	}
private:
	int manger;
};

//std::set<std::string> _cache;
std::set<int> _cache;
int     count = 0;
CommonLock::ReadWriteLock readwritelock;

class ThreadWR : public Common::Thread{
public:
	ThreadWR(int man = 0) : manger(man) { }
	virtual ~ThreadWR(){ 
		struct timeval tm;
		gettimeofday(&tm, NULL);

		std::cout << "~ThreadRW:"<<manger<< "sec: "<< tm.tv_sec << " msce: "<< tm.tv_usec<<std::endl; }
	virtual int run()
	{
		srand(time(0));

		if(manger)
		{
			for( int i = 0 ; i < 1000; i++)
			{
				std::strstream ostr;
				//std::cout<<"th: "<<_tid<< ostr.str()<<std::endl;
				{
					CommonLock::WrwLockGuard gd(readwritelock);
					count ++;
					//ostr << _tid << ":" << count;
					//_cache.insert(ostr.str());
					_cache.insert(count);
				}
				//std::cout<<"th: "<<_tid<< ostr.str()<<std::endl;
				sleep(rand() % 3);
			}
		}
		else
		{
			while( 1 ){
				//std::string str;
				int count = 0;
				{
					CommonLock::WrwLockGuard gd(readwritelock);
					//CommonLock::RrwLockGuard gd(readwritelock);
					if( _cache.empty() )
						//str = "empty";
						count = 0;
					else{
						//str = *_cache.begin();
						count = *_cache.begin();
						std::cout<<count<<std::endl;
						_cache.erase(_cache.begin());

					}
				}
				//std::cout<<"th: "<<_tid<< " : "<<str <<std::endl;
				sleep(2);
			}
		}

		return 100;
	}
private:
	int manger;
};

int main(void)
{
	int count = 0;
	CommonLock::CondLock countlock;
	try{
		ThreadWR* t2 = new ThreadWR();
		ThreadWR* t0 = new ThreadWR(1);
		sleep(1);
		ThreadWR* t1 = new ThreadWR(0);
		sleep(1);
		ThreadWR* t3 = new ThreadWR(1);
		ThreadWR* t4 = new ThreadWR(0);
		sleep(1);
		ThreadWR* t5 = new ThreadWR(1);
		ThreadWR* t6 = new ThreadWR(0);
		sleep(1);
		ThreadWR* t7 = new ThreadWR(1);
		ThreadWR* t8 = new ThreadWR(0);
		sleep(1);
		ThreadWR* t9 = new ThreadWR(1);
		sleep(1);
		ThreadWR* t10 = new ThreadWR(1);



	}
	catch(const std::runtime_error &err)
	{
		std::cout<<err.what()<<std::endl;
	}
	catch(...)
	{
		std::cout<<"donot know what exception"<<std::endl;
	}
	while(1)
	{
		int a = 0;
		a++;
	}

	return 0;
}
