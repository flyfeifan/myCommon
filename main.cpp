#include <iostream>
#include "thread.h"
#include "lock.h"

class ThreadTack : public Common::Thread{

public:
	ThreadTack( int &count, CommonLock::MutexLock &lock) : _count(count), _lock(lock) { }
	virtual ~ThreadTack(){ }
	virtual int run()
	{
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
		}
		return 100;
	}
private:
	int &_count;
	CommonLock::MutexLock &_lock;


};


int main(void)
{
	int count = 0;
	CommonLock::MutexLock countlock;
	Common::Thread *th1 = new ThreadTack(count, countlock);
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
	
	while(1)
	{
		int a = 0;
		a++;
	}

	return 0;
}
