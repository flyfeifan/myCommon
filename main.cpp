#include <iostream>
#include <set>
#include "thread.h"
#include "tools/nreentry.h"
#include "lock.h"
#include <vector>
#include <sys/time.h>
#include <stdlib.h>
//#include <strstream>
/*
class ThreadTack : public Common::Thread{
public:
	ThreadTack( int &count, Common::CondLock &lock, int man = 0) : _count(count), _lock(lock), manger(man) { }
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
		
		for(int i = 0; i<10000; i++)
		{
			{
				Common::LockGuard gd(_lock);
				_count ++;
			}
			{
				//Common::LockGuard gd(_lock);
				Common::MutexGuard gd(_lock);
				std::cout<<"thread: "<<_tid<<" i: "<<i<<" count: " << _count <<std::endl;
			}
			sleep(1);
		}
		return 100;
	}
protected:
	virtual void _final()
	{
		delete this;
	}
private:
	int manger;
	int &_count;
	Common::CondLock &_lock;


};

std::string  strCache;
Common::Semaphore semaphore;

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
				sleep(0.8);
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
protected:	
	virtual void _final()
	{
		delete this;
	}

private:
	int manger;
};

//std::set<std::string> _cache;
std::set<int> _cache;
int     count = 0;
Common::ReadWriteLock readwritelock;

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
			for( int i = 0 ; i < 3; i++)
			{
				std::strstream ostr;
				//std::cout<<"th: "<<_tid<< ostr.str()<<std::endl;
				{
					Common::WrwLockGuard gd(readwritelock);
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
					Common::WrwLockGuard gd(readwritelock);
					//Common::RrwLockGuard gd(readwritelock);
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
protected:
	virtual void _final()
	{
		delete this;
	}
private:
	int manger;
};
*/
class DoSomeData{
public:
	DoSomeData() : _dataPos(0){
	//	for(int i = 0; i< 56789; i++ )
	//		_dataVect.push_back(i + 123);
	}
	~DoSomeData() { }
	void addVectorFun( pthread_t tid)
	{
		if( _funEntry.tryEntry() )
		{
			_dataPos ++;
			std::cout<<"add _dataPos: "<< _dataPos<< " tid: "<< tid <<  std::endl;
			_funEntry.out();
		}
		else
		{
			//_dataPos --;
			std::cout<<"not add  _dataPos: "<< _dataPos<<" tid: " << tid<< std::endl;
		}
	}

	void addVectorFunTry(pthread_t tid )
	{
		try{
			Common::NEntryTry   _tryEntry;
			_dataPos ++;
			std::cout<<"add _dataPos: "<< _dataPos<< " tid: "<< tid << std::endl;
		}catch(const std::runtime_error &err)
		{
			std::cout<<"not add _dataPos: "<< _dataPos<< " tid: "<< tid<<std::endl;
			std::cout<<err.what()<<std::endl;
		}
		return;
	}
		
private:
	Common::NEntryFunc  _funEntry;
	//Common::NEntryTry   _tryEntry;
	int                 _dataPos;
	//std::vecthor<int>   _dataVect;
};

std::vector<int> data;
Common::CondLock  cond;
Common::MutexLock mutex;

class DataThread : public Common::NativeThread{
public:
	DataThread(DoSomeData *data) : _data(data) { 
		//Common::Thread::start();
	}
	virtual ~DataThread(){ 
		std::cout<< "stop thread: " << _tid<<std::endl;
	}
	virtual int run()
	{
		std::cout << _tid << " : start run" << std::endl;
		if( rand() % 2 == 0 )
		{
			mutex.enter();
			std::cout <<_tid<<" :  mutex enter" << std::endl;
			/*
			int dd = rand() % 2000;
			cond.enter();
			data.push_back(dd);
			cond.leave();
			cond.signal();
			std::cout<< "G: "<<dd<<std::endl;
			*/
		}
		else
		{
			mutex.leave();
			std::cout << _tid<< " : mutex leave" <<std::endl;
		}

		return 0;
	}

protected:	
	virtual void _final()
	{
		delete this;
	}
private:
	DoSomeData * _data;
};

int main(void)
{
	//int count = 0;
	//Common::CondLock countlock;
	try{
		DoSomeData  thedata;
		
		(new DataThread(&thedata))->start();
		(new DataThread(&thedata))->start();
		(new DataThread(&thedata))->start();
		(new DataThread(&thedata))->start();
		(new DataThread(&thedata))->start();
		(new DataThread(&thedata))->start();
		(new DataThread(&thedata))->start();
		(new DataThread(&thedata))->start();
		(new DataThread(&thedata))->start();
		(new DataThread(&thedata))->start();
		(new DataThread(&thedata))->start();
		(new DataThread(&thedata))->start();
		(new DataThread(&thedata))->start();
		(new DataThread(&thedata))->start();
		(new DataThread(&thedata))->start();
		(new DataThread(&thedata))->start();
		(new DataThread(&thedata))->start();
		(new DataThread(&thedata))->start();
		(new DataThread(&thedata))->start();
		(new DataThread(&thedata))->start();
		(new DataThread(&thedata))->start();
		(new DataThread(&thedata))->start();
		(new DataThread(&thedata))->start();
		(new DataThread(&thedata))->start();
		(new DataThread(&thedata))->start();
		(new DataThread(&thedata))->start();
		(new DataThread(&thedata))->start();
		(new DataThread(&thedata))->start();
		(new DataThread(&thedata))->start();
		(new DataThread(&thedata))->start();
		(new DataThread(&thedata))->start();
		(new DataThread(&thedata))->start();
		(new DataThread(&thedata))->start();
		(new DataThread(&thedata))->start();
		(new DataThread(&thedata))->start();
		(new DataThread(&thedata))->start();
		(new DataThread(&thedata))->start();
		
	}
	catch(const std::runtime_error &err)
	{
		std::cout<<err.what()<<std::endl;
	}
	catch(...)
	{
		std::cout<<"donot know what exception"<<std::endl;
	}
	int a = 66;
	while(a > 0)
	{
		a --;
		sleep(5);
	}

	return 0;
}
