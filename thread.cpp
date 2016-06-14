
#include "thread.h"
#include <errno.h>

namespace Common{

Thread::Thread( bool detach/*= false*/, bool wait/*=false*/)
:_wait(wait)
{
	if(wait){
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
	if( 0 != pthread_create(&_tid, &attr, thread_exec, (void*)this)){
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
	Thread* curr_th = (Thread*) th;
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

	delete curr_th;
	return (void *) NULL;
}
}
