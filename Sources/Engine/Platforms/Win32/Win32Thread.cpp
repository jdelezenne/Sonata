/*=============================================================================
Win32Thread.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include <windows.h>

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
	HANDLE _handle;
};


ThreadInternal::ThreadInternal() :
	_id((ThreadId)0),
	_state(ThreadState_Unstarted),
	_start(NULL),
	_handle(NULL)
{
}

	
const int32 Thread::Infinite = -1;

static DWORD WINAPI _ThreadProc(LPVOID lpParameter)
{
    ((Thread*)lpParameter)->Run();
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

	::CloseHandle(_internal->_handle);

	delete _internal;
}

ThreadId Thread::GetCurrentThreadId()
{
	return (ThreadId)::GetCurrentThreadId();
}

void Thread::Sleep(int32 millisecondsTimeout)
{
	::Sleep(millisecondsTimeout);
}

void Thread::Exit()
{
	::ExitThread(0);
}

ThreadId Thread::GetThreadId() const
{
	return _internal->_id;
}

ThreadPriority Thread::GetPriority() const
{
	int nPriority = ::GetThreadPriority(_internal->_handle);
	switch (nPriority)
	{
	case THREAD_PRIORITY_ABOVE_NORMAL:
		return ThreadPriority_AboveNormal;
	case THREAD_PRIORITY_BELOW_NORMAL:
		return ThreadPriority_BelowNormal;
	case THREAD_PRIORITY_HIGHEST:
		return ThreadPriority_Highest;
	case THREAD_PRIORITY_LOWEST:
		return ThreadPriority_Lowest;
	case THREAD_PRIORITY_NORMAL:
		return ThreadPriority_Normal;
	default:
		return ThreadPriority_Normal;
	}
}

void Thread::SetPriority(ThreadPriority value)
{
	int nPriority;
	switch (value)
	{
	case ThreadPriority_AboveNormal:
		nPriority = THREAD_PRIORITY_ABOVE_NORMAL;
		break;
	case ThreadPriority_BelowNormal:
		nPriority = THREAD_PRIORITY_BELOW_NORMAL;
		break;
	case ThreadPriority_Highest:
		nPriority = THREAD_PRIORITY_HIGHEST;
		break;
	case ThreadPriority_Lowest:
		nPriority = THREAD_PRIORITY_LOWEST;
		break;
	case ThreadPriority_Normal:
		nPriority = THREAD_PRIORITY_NORMAL;
		break;
	default:
		nPriority = THREAD_PRIORITY_NORMAL;
	}
	::SetThreadPriority(_internal->_handle, nPriority);
}

ThreadState Thread::GetThreadState() const
{
	return _internal->_state;
}

bool Thread::IsAlive() const
{
	if (_internal->_handle == NULL)
		return false;

	DWORD nExitCode;
	::GetExitCodeThread(_internal->_handle, &nExitCode);
	return (nExitCode == STILL_ACTIVE);
}

void Thread::Start()
{
	if (_internal->_state != ThreadState_Unstarted)
		return;

	DWORD dwThreadId;
	_internal->_handle = ::CreateThread(NULL, 0, _ThreadProc, (LPVOID)this, 0, &dwThreadId);
	if (_internal->_handle == NULL)
	{
		SEthrow(Exception("Failed creating the thread."));
	}
	else
	{
		_internal->_id = (ThreadId)dwThreadId;
		_internal->_state = ThreadState_Running;
	}
}

void Thread::Abort()
{
	if (_internal->_state != ThreadState_Running)
		return;

	::TerminateThread(_internal->_handle, 0);
	_internal->_state = ThreadState_Stopped;
}

void Thread::Suspend()
{
	if (_internal->_state != ThreadState_Running)
		return;

	::SuspendThread(_internal->_handle);
	_internal->_state = ThreadState_Suspended;
}

void Thread::Resume()
{
	if (_internal->_state != ThreadState_Suspended)
		return;

	::ResumeThread(_internal->_handle);
	_internal->_state = ThreadState_Running;
}

void Thread::Join(int32 millisecondsTimeout)
{
	if (_internal->_state == ThreadState_Unstarted)
		return;

	::WaitForSingleObject(_internal->_handle, millisecondsTimeout);
}

void Thread::Run()
{
	if (_internal->_start != NULL)
	{
	    (_internal->_start)();
	}
}

}
