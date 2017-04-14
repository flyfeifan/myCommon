#include <errno.h>
#include <sys/time.h>
#include <stdlib.h>

#include "lock.h"

namespace Common{
MutexLock::MutexLock(bool shared /*= false*/)
{
	pthread_mutexattr_t  m_attr;
	if(0 != pthread_mutexattr_init(&m_attr) )
		throw LockException("init mutex attr failed", __FILE__, __LINE__, errno);
	if(0 != pthread_mutexattr_settype(&m_attr, PTHREAD_MUTEX_ERRORCHECK) )
		throw LockException("set mutex attr type failed", __FILE__, __LINE__, errno);
	
	if( shared )
	{
		if( 0 != pthread_mutexattr_setpshared(&m_attr, PTHREAD_PROCESS_SHARED) )
			throw LockException("failed to set PTHREAD_PROCESS_SHARED", __FILE__, __LINE__, errno );
	}

	if(0 != pthread_mutex_init(&_mutex,&m_attr) )
	{
		pthread_mutexattr_destroy(&m_attr);
		throw LockException("init mutex failed", __FILE__, __LINE__, errno);
	}
	pthread_mutexattr_destroy(&m_attr);
}

MutexLock::~MutexLock()
{
	if(0 != pthread_mutex_destroy(&_mutex))
		throw LockException("destroy mutex failed", __FILE__, __LINE__, errno);
}

void MutexLock::enter()
{
	if ( 0 != pthread_mutex_lock(&_mutex) )
		throw LockException("lock mutex failed", __FILE__, __LINE__, errno);
}

void MutexLock::leave()
{
	if( 0 != pthread_mutex_unlock(&_mutex) )
		throw LockException("unlock mutex failed", __FILE__, __LINE__, errno);
}

bool  MutexLock::tryenter()
{
	return pthread_mutex_trylock(&_mutex) == 0 ;
}

ReadWriteLock::ReadWriteLock()
{
	if( 0 != pthread_rwlock_init(&_rwlock, NULL) )//user defalt attr
		throw LockException("init rwlock failed", __FILE__, __LINE__, errno);
}

ReadWriteLock::~ReadWriteLock()
{
	if( 0 != pthread_rwlock_destroy(&_rwlock) )
		throw LockException("destroyed rwlock failed", __FILE__, __LINE__, errno);
}

void ReadWriteLock::enter()
{
	if( 0 != pthread_rwlock_rdlock(&_rwlock) )
		throw LockException("lock read lock failed", __FILE__, __LINE__, errno);
}

void ReadWriteLock::leave()
{
	if( 0 != pthread_rwlock_unlock(&_rwlock) )
		throw LockException("unlock rw lock failed", __FILE__, __LINE__, errno);
}

void ReadWriteLock::wenter()
{
	if( 0 != pthread_rwlock_wrlock(&_rwlock) )
		throw LockException("lock write lock failed", __FILE__, __LINE__, errno);
}

CondLock::CondLock(bool shared /*=false*/) : MutexLock(shared)
{
	if( !shared )
	{
		//if(0 != pthread_mutex_init(&_mutexlock, NULL) )
		//	throw LockException("init mutex of condlock failed", __FILE__, __LINE__, errno);
		if(0 != pthread_cond_init(&_condlock, NULL ) )
			throw LockException("init cond of condlock failed", __FILE__, __LINE__, errno);
	}
	else
	{
		/*
		pthread_mutexattr_t  m_attr;
		if(0 != pthread_mutexattr_init(&m_attr) )
			throw LockException("init mutex attr failed", __FILE__, __LINE__, errno);

		if( 0 != pthread_mutexattr_setpshared(&m_attr, PTHREAD_PROCESS_SHARED) )
			throw LockException("failed to set PTHREAD_PROCESS_SHARED", __FILE__, __LINE__, errno );
		if(0 != pthread_mutex_init(&_mutexlock,&m_attr) )
		{
			pthread_mutexattr_destroy(&m_attr);
			throw LockException("init mutex failed", __FILE__, __LINE__, errno);
		}
		pthread_mutexattr_destroy(&m_attr);
		*/
		pthread_condattr_t  c_attr;
		if( 0 != pthread_condattr_init(&c_attr) )
			throw LockException("init cond attr failed", __FILE__, __LINE__, errno);
		if( 0 != pthread_condattr_setpshared(&c_attr, PTHREAD_PROCESS_SHARED) )
			throw LockException("failed to set PTHREAD_PROCESS_SHARED", __FILE__, __LINE__, errno );
		
		if(0 != pthread_cond_init(&_condlock, NULL ) )
		{
			pthread_condattr_destroy(&c_attr);
			throw LockException("init cond of condlock failed", __FILE__, __LINE__, errno);
		}	
		pthread_condattr_destroy(&c_attr);
	}
}

CondLock::~CondLock()
{
	//if( 0 != pthread_mutex_destroy(&_mutexlock) )
	//	throw LockException("destroyed mutex of condlock failed", __FILE__, __LINE__, errno);

	if( 0 != pthread_cond_destroy(&_condlock) )
		throw LockException("destroyed cond of condlock failed", __FILE__, __LINE__, errno);
}

void CondLock::enter()
{
	MutexLock::enter();
}

void CondLock::leave()
{
	MutexLock::leave();
}

bool CondLock::wait( int msec /*=0*/)
{
	if( msec <= 0 )
	{
		if( 0 != pthread_cond_wait(&_condlock, &_mutex) )
		{
			return false;
		}
	}
	else
	{
		struct timeval timenow;
		if( 0 != gettimeofday(&timenow, NULL) )
			return false;
		
		struct timespec timv;
		timv.tv_sec = timenow.tv_sec + msec / 1000;
		timv.tv_nsec = (timenow.tv_usec + msec % 1000) * 1000 ;

		if( 0 != pthread_cond_timedwait(&_condlock, &_mutex, &timv) )
		{
			return false;
		}
	}
	return true;
}

bool CondLock::signal()
{
	return (0 == pthread_cond_signal(&_condlock));
}

bool CondLock::broadcast()
{
	return (0 == pthread_cond_broadcast(&_condlock));
}

Semaphore::Semaphore()
{
	if(0 != sem_init(&_sem, 0, 0))
		throw LockException("create Semaphore failed", __FILE__, __LINE__, errno);
}

Semaphore::~Semaphore()
{
	if(0 != sem_destroy(&_sem) )
		throw LockException("destroy Semaphore failed", __FILE__, __LINE__, errno);
}

bool Semaphore::post()
{
	return 0 == sem_post(&_sem);
}

bool Semaphore::wait(int mesc/* = 0*/)
{
	if( mesc == 0 )
		return 0 == sem_wait(&_sem);
	else{
		struct timespec tsp;
		if( 0 != clock_gettime(CLOCK_REALTIME, &tsp) )
			return false;
		//tsp.tv_sec  += (tsp.tv_nsec + mesc * 1000 * 1000) / (1000 * 1000 * 1000);
		//tsp.tv_nsec = (tsp.tv_nsec + mesc * 1000 * 1000) % (1000 * 1000 * 1000);	
		tsp.tv_sec += mesc/1000;
		tsp.tv_nsec += (mesc % 1000) * 1000 * 1000;
		return 0 == sem_timedwait(&_sem, &tsp);
	}
}


}
