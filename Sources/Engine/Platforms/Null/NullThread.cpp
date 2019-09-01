/*=============================================================================
NullThread.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "Core/Threading/Thread.h"

namespace SonataEngine
{

class ThreadInternal
{
};


const int32 Thread::Infinite = -1;

Thread::Thread(ThreadStart start) :
	_internal(NULL)
{
}

Thread::~Thread()
{
}

ThreadId Thread::GetCurrentThreadId()
{
	return (ThreadId)0;
}

void Thread::Sleep(int32 millisecondsTimeout)
{
}

void Thread::Exit()
{
}

ThreadId Thread::GetThreadId() const
{
	return 0;
}

ThreadPriority Thread::GetPriority() const
{
	return ThreadPriority_Normal;
}

void Thread::SetPriority(ThreadPriority value)
{
}

ThreadState Thread::GetThreadState() const
{
	return ThreadState_Unstarted;
}

bool Thread::IsAlive() const
{
	return false;
}

void Thread::Start()
{
}

void Thread::Abort()
{
}

void Thread::Suspend()
{
}

void Thread::Resume()
{
}

void Thread::Join(int32 millisecondsTimeout)
{
}

void Thread::Run()
{
}

}
