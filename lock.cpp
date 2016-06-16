#include <errno.h>
#include "lock.h"

namespace CommonLock{
MutexLock::MutexLock()
{
	pthread_mutexattr_t  m_attr;
	if(0 != pthread_mutexattr_init(&m_attr) )
		throw LockException("init mutex attr failed", __FILE__, __LINE__, errno);
	if(0 != pthread_mutexattr_settype(&m_attr, PTHREAD_MUTEX_ERRORCHECK) )
		throw LockException("set mutex attr type failed", __FILE__, __LINE__, errno);
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

}
