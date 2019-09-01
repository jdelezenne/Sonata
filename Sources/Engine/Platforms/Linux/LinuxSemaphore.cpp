/*=============================================================================
LinuxSemaphore.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include <unistd.h>
#include <pthread.h>

#include "Core/Threading/Semaphore.h"
#include "Core/Exception/Exception.h"

namespace SonataEngine
{

class SemaphoreInternal
{
public:
	SemaphoreInternal();

public:
	sem_t _handle;
};


SemaphoreInternal::SemaphoreInternal() :
	_handle((sem_t)0)
{
}


Semaphore::Semaphore(int initialCount, int maximumCount) :
	_internal(new SemaphoreInternal())
{
	// maximumCount unsupported with POSIX
	if (sem_init(&_internal->_handle, 0, initialCount) != 0)
	{
		SEthrow(Exception("Failed creating the semaphore object."));
	}
}

Semaphore::~Semaphore()
{
	sem_destroy(&_internal->_handle);

	delete _internal;
}

void Semaphore::Wait()
{
	if (sem_wait(&_internal->_handle) != 0)
	{
		SEthrow(Exception("Failed locking the semaphore object."));
	}
}

void Semaphore::Wait(int32 millisecondsTimeout)
{
	while (millisecondsTimeout-- > 0) 
	{
		if (sem_trywait(&_internal->_handle) != 0)
		{
			SEthrow(Exception("Failed locking the semaphore object."));
			return;
		}
		usleep(1000);
	}
}

int32 Semaphore::Release(int32 releaseCount)
{
	int32 previousCount = 0;
	sem_getvalue(&_internal->_handle, &previousCount);

	while (releaseCount-- > 0)
	{
		if (sem_post(&_internal->_handle) != 0)
		{
			SEthrow(Exception("Failed unlocking the semaphore object."));
			return previousCount;
		}
	}
	return previousCount;
}

}
