/*=============================================================================
SDLSemaphore.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include <SDL.h>
#include <SDL_thread.h>

#include "Core/Threading/Semaphore.h"
#include "Core/Exception/Exception.h"

namespace SonataEngine
{

class SemaphoreInternal
{
public:
	SemaphoreInternal();

public:
	SDL_sem* _handle;
};


SemaphoreInternal::SemaphoreInternal() :
	_handle(NULL)
{
}


Semaphore::Semaphore(int initialCount, int maximumCount) :
	_internal(new SemaphoreInternal())
{
	// maximumCount unsupported with SDL
	_internal->_handle = SDL_CreateSemaphore(initialCount);
	if (_internal->_handle == NULL)
	{
		SEthrow(Exception("Failed creating the semaphore object."));
	}
}

Semaphore::~Semaphore()
{
	SDL_DestroySemaphore(_internal->_handle);

	delete _internal;
}

void Semaphore::Wait()
{
	if (SDL_SemTryWait(_internal->_handle) != 0)
	{
		SEthrow(Exception("Failed locking the semaphore object."));
	}
}

void Semaphore::Wait(int32 millisecondsTimeout)
{
	int res = SDL_SemWaitTimeout(_internal->_handle, millisecondsTimeout);
	switch (res)
	{
	// The semaphore object was signaled.
	case 0:
		break;
	case SDL_MUTEX_TIMEOUT:
		SEthrow(Exception("Semaphore was nonsignaled, so a time-out occurred."));
	case -1:
		SEthrow(Exception("Failed locking the semaphore object."));
	}
}

int32 Semaphore::Release(int32 releaseCount)
{
	int32 previousCount = (int32)SDL_SemValue(_internal->_handle);
	while (releaseCount-- > 0)
	{
		if (SDL_SemPost(_internal->_handle) != 0)
		{
			SEthrow(Exception("Failed unlocking the semaphore object."));
			return previousCount;
		}
	}
	return previousCount;
}

}
