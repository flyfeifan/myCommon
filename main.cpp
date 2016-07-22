#include <iostream>
#include "thread.h"
#include "lock.h"
#include <sys/time.h>

class ThreadTack : public Common::Thread{

public:
	ThreadTack( int &count, CommonLock::CondLock &lock, int man = 0) : _count(count), _lock(lock), manger(man) { }
	virtual ~ThreadTack(){ }
	virtual int run()
	{
		sleep(5);
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


int main(void)
{
	int count = 0;
	CommonLock::CondLock countlock;
	try{
		Common::Thread *th1 = new ThreadTack(count, countlock, 1);
		Common::Thread *th2 = new ThreadTack(count, countlock);
		Common::Thread *th3 = new ThreadTack(count, countlock);
		Common::Thread *th4 = new ThreadTack(count, countlock);
		Common::Thread *th5 = new ThreadTack(count, countlock);
		Common::Thread *th6 = new ThreadTack(count, countlock);
		Common::Thread *th7 = new ThreadTack(count, countlock);
		Common::Thread *th8 = new ThreadTack(count, countlock);
		Common::Thread *th9 = new ThreadTack(count, countlock);
		Common::Thread *th10 = new ThreadTack(count, countlock);
		Common::Thread *th11 = new ThreadTack(count, countlock);
		Common::Thread *th12 = new ThreadTack(count, countlock);
		Common::Thread *th13 = new ThreadTack(count, countlock);
		Common::Thread *th14 = new ThreadTack(count, countlock);
		Common::Thread *th15 = new ThreadTack(count, countlock);
		Common::Thread *th16 = new ThreadTack(count, countlock);
		Common::Thread *th17 = new ThreadTack(count, countlock);
		Common::Thread *th18 = new ThreadTack(count, countlock);
		Common::Thread *th19 = new ThreadTack(count, countlock);
		Common::Thread *th20 = new ThreadTack(count, countlock);
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
