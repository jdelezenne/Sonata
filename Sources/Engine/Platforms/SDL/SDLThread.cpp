/*=============================================================================
SDLThread.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include <SDL.h>
#include <SDL_thread.h>

#include "Core/Threading/Thread.h"
#include "Core/Exception/Exception.h"

namespace SonataEngine
{

class ThreadInternal
{
public:
	ThreadInternal();

public:
	ThreadId _id;
	ThreadState _state;
	ThreadStart _start;
	SDL_Thread* _handle;
};


ThreadInternal::ThreadInternal() :
	_id((ThreadId)0),
	_state(ThreadState_Unstarted),
	_start(NULL),
	_handle(NULL)
{
}

	
const int32 Thread::Infinite = -1;

int _ThreadFunc(void* arg)
{
    ((Thread*)arg)->Run();
    return 0;
}

Thread::Thread(ThreadStart start) :
	_internal(new ThreadInternal())
{
	_internal->_start = start;
}

Thread::~Thread()
{
	if (_internal->_state == ThreadState_Running)
	{
		Abort();
	}

	delete _internal;
}

ThreadId Thread::GetCurrentThreadId()
{
	return (ThreadId)SDL_ThreadID();
}

void Thread::Exit()
{
	exit(0);
}

void Thread::Sleep(int32 millisecondsTimeout)
{
	SDL_Delay(millisecondsTimeout);
}

ThreadId Thread::GetThreadId() const
{
	return _internal->_id;
}

ThreadPriority Thread::GetPriority() const
{
	// Unsupported with SDL
	return ThreadPriority_Normal;
}

void Thread::SetPriority(ThreadPriority value)
{
	// Unsupported with SDL
}

ThreadState Thread::GetThreadState() const
{
	return _internal->_state;
}

bool Thread::IsAlive() const
{
	return (_internal->_handle != NULL);
}

void Thread::Start()
{
	if (_internal->_state != ThreadState_Unstarted)
		return;

	_internal->_handle = SDL_CreateThread(_ThreadFunc, this);
	if (_internal->_handle == NULL)
	{
		SEthrow(Exception("Failed creating the thread."));
	}
	else
	{
		_internal->_id = (ThreadId)SDL_GetThreadID(_internal->_handle);
		_internal->_state = ThreadState_Running;
	}
}

void Thread::Abort()
{
	if (_internal->_state != ThreadState_Running)
		return;

	SDL_KillThread(_internal->_handle);
	_internal->_state = ThreadState_Stopped;
}

void Thread::Suspend()
{
	// Unsupported with SDL
}

void Thread::Resume()
{
	// Unsupported with SDL
}

void Thread::Join(int32 millisecondsTimeout)
{
	if (_internal->_state == ThreadState_Unstarted)
		return;

	// millisecondsTimeout unsupported with SDL
	SDL_WaitThread(_internal->_handle, NULL);
}

void Thread::Run()
{
	if (_internal->_start != NULL)
	{
	    (_internal->_start)();
	}
}

}
