/*=============================================================================
SDLMutex.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include <SDL.h>
#include <SDL_mutex.h>

#include "Core/Threading/Mutex.h"
#include "Core/Exception/Exception.h"

namespace SonataEngine
{

class MutexInternal
{
public:
	MutexInternal();

public:
	SDL_mutex* _handle;
};


MutexInternal::MutexInternal() :
	_handle(NULL)
{
}


Mutex::Mutex() :
	_internal(new MutexInternal())
{
	_internal->_handle = SDL_CreateMutex();
	if (_internal->_handle == NULL)
	{
		SEthrow(Exception("Failed creating the mutex object."));
	}
}

Mutex::~Mutex()
{
	SDL_DestroyMutex(_internal->_handle);

	delete _internal;
}

void Mutex::Enter()
{
	// Locks the mutex.
	if (SDL_LockMutex(_internal->_handle) == -1)
	{
		SEthrow(Exception("Failed locking the mutex object."));
	}
}

void Mutex::TryEnter()
{
	// Unsupported with SDL, same as Enter
	Enter();
}

void Mutex::Exit()
{
	// Unlocks the mutex.
	if (SDL_UnlockMutex(_internal->_handle) == -1)
	{
		SEthrow(Exception("Failed unlocking the mutex object."));
	}
}

}
