
#include "thread.h"
#include <errno.h>

namespace Common{

Thread::Thread( bool detach/*= false*/)
:_wait(true)
{
	if(_wait){
		if(sem_init(&_semstart, 0, 0)){
			throw ThreadExcept("failed to init sem", __FILE__, __LINE__, errno);
		}
	}
	pthread_attr_t attr;
	int res = pthread_attr_init(&attr);
	if( !res && detach){
		res = pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);	
	}
	if(res != 0){
		throw ThreadExcept("failed to init thread attr", __FILE__, __LINE__, errno);
	}
	if( 0 != pthread_create(&_tid, &attr, Thread::thread_exec, (void*)this)){
		pthread_attr_destroy(&attr);
		throw ThreadExcept("failed to create thread", __FILE__, __LINE__, errno);
	}
	pthread_attr_destroy(&attr);
}

Thread::~Thread()
{
	if(0 != sem_destroy(&_semstart) )
		throw ThreadExcept("faied to detroy semstart", __FILE__, __LINE__, errno);
}

void Thread::start()
{
	sem_post(&_semstart);
}

void* Thread::thread_exec(void* th)
{
	Thread* curr_th = static_cast<Thread*>(th);
	if(curr_th->_wait)
	{
INTERRUPT:
		int ret = sem_wait(&(curr_th->_semstart));
		if(ret == -1)
		{
			if(EINTR == errno)
				goto INTERRUPT;
			throw ThreadExcept("sem failed to wait single", __FILE__, __LINE__, errno);
		}
	}
	curr_th->run();
	// sem_post(&th->_thsem);
	try{
		curr_th->_final();
	}catch(...)
	{
		//_final error do nothing
	}
	return (void *) NULL;
}

//class NativThread

NativeThread::NativeThread(bool detach /*=false*/ )
:_detach(detach), _status(THREAD_STOP),_tid(0)
{
	
}

NativeThread::~NativeThread( )
{

}

void NativeThread::start()
{
	//TODO create defalt thread
	if( !_detach )
	{
		if( 0 != pthread_create(&_tid, NULL, NativeThread::thread_exec, (void*)this)){
			throw ThreadExcept("failed to create thread", __FILE__, __LINE__, errno);
		}
		_status=THREAD_RUNING;
		return;
	}
	//TOD create detach thread
	pthread_attr_t attr;
	int res = pthread_attr_init(&attr);
	if( res == 0 ){
		res = pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);	
	}

	if( res != 0 ){
		throw ThreadExcept("failed to init thread attr", __FILE__, __LINE__, errno);
	}

	if( 0 != pthread_create(&_tid, &attr, thread_exec, (void*)this)){
		pthread_attr_destroy(&attr);
		throw ThreadExcept("failed to create thread", __FILE__, __LINE__, errno);
	}
	pthread_attr_destroy(&attr);
	_status=THREAD_RUNING;
	return;
}

void NativeThread::stop()
{
	_status=THREAD_STOP;
	pthread_cancel(_tid);
}

void* NativeThread::thread_exec(void* th)
{
	NativeThread* curr_th = static_cast<NativeThread*>(th);
	try{
		curr_th->run();
		
		curr_th->_final();
	}catch(...)
	{
		throw ThreadExcept("get exception from thread run or final", __FILE__, __LINE__, 0);
	}
	return (void *) NULL;
}


}
