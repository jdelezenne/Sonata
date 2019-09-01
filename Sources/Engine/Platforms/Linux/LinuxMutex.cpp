/*=============================================================================
LinuxMutex.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include <pthread.h>

#include "Core/Threading/Mutex.h"
#include "Core/Exception/Exception.h"

namespace SonataEngine
{

class MutexInternal
{
public:
	MutexInternal();

public:
	pthread_mutex_t _handle;
};


MutexInternal::MutexInternal() :
	_handle((pthread_mutex_t)0)
{
}


Mutex::Mutex() :
	_internal(new MutexInternal())
{
	if (pthread_mutex_init(&_internal->_handle, FALSE, NULL) != 0)
	{
		SEthrow(Exception("Failed creating the mutex object."));
	}
}

Mutex::~Mutex()
{
	pthread_mutex_destroy(&_internal->_handle);

	delete _internal;
}

void Mutex::Enter()
{
	if (pthread_mutex_lock(&_internal->_handle) != 0)
	{
		SEthrow(Exception("Failed locking the mutex object."));
	}
}

void Mutex::TryEnter()
{
	if (pthread_mutex_trylock(&_internal->_handle) != 0)
	{
		SEthrow(Exception("Failed locking the mutex object."));
	}
}

void Mutex::Exit()
{
	if (pthread_mutex_unlock(&_internal->_handle) != 0)
	{
		SEthrow(Exception("Failed locking the mutex object."));
	}
}

}
