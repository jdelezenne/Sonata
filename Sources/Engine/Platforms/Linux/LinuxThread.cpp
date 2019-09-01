/*=============================================================================
LinuxThread.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include <unistd.h>
#include <pthread.h>

#include "Core/Threading/Thread.h"
#include "Core/Exception/Exception.h"

namespace SonataEngine
{

class ThreadInternal
{
public:
	ThreadInternal();

public:
	ThreadState _state;
	ThreadStart _start;
	pthread_t _handle;
};


ThreadInternal::ThreadInternal() :
	_state(ThreadState_Unstarted),
	_start(NULL),
	_handle((pthread_t)0)
{
}

	
const int32 Thread::Infinite = -1;

void* _ThreadFunc(void* arg)
{
    ((Thread*)arg)->Run();
	return NULL;
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

	pthread_detach(_internal->_handle);

	delete _internal;
}

ThreadId Thread::GetCurrentThreadId()
{
	// Unsupported with POSIX
	return (ThreadId)0;
}

void Thread::Sleep(int32 millisecondsTimeout)
{
	usleep(millisecondsTimeout * 1000);
}

void Thread::Exit()
{
	pthread_exit(0);
}

ThreadId Thread::GetThreadId() const
{
	return _internal->_handle;
}

ThreadPriority Thread::GetPriority() const
{
	// Unsupported with POSIX
	return ThreadPriority_Normal;
}

void Thread::SetPriority(ThreadPriority value)
{
	// Unsupported with POSIX
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

	if (pthread_create(&_internal->_handle, NULL, _ThreadFunc, this) != 0)
	{
		SEthrow(Exception("Failed creating the thread."));
	}
	else
	{
		_internal->_id = (ThreadId)::GetThreadId(_internal->_handle);
		_internal->_state = ThreadState_Running;
	}
}

void Thread::Abort()
{
	if (_internal->_state != ThreadState_Running)
		return;

	pthread_cancel(_internal->_handle);
	_internal->_state = ThreadState_Stopped;
}

void Thread::Suspend()
{
	// Unsupported with POSIX
}

void Thread::Resume()
{
	// Unsupported with POSIX
}

void Thread::Join(int32 millisecondsTimeout)
{
	if (_internal->_state == ThreadState_Unstarted)
		return;

	// millisecondsTimeout unsupported with POSIX
	pthread_join(_internal->_handle, NULL);
}

void Thread::Run()
{
	if (_internal->_start != NULL)
	{
	    (_internal->_start)();
	}
}

}
