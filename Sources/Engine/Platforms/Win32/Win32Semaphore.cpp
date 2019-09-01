/*=============================================================================
Win32Semaphore.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include <windows.h>

#include "Core/Threading/Semaphore.h"
#include "Core/Exception/Exception.h"

namespace SonataEngine
{

class SemaphoreInternal
{
public:
	SemaphoreInternal();

public:
	HANDLE _handle;
};


SemaphoreInternal::SemaphoreInternal() :
	_handle(NULL)
{
}


Semaphore::Semaphore(int initialCount, int maximumCount) :
	_internal(new SemaphoreInternal())
{
	_internal->_handle = ::CreateSemaphore(NULL, initialCount, maximumCount, NULL);
	if (_internal->_handle == NULL)
	{
		SEthrow(Exception("Failed creating the semaphore object."));
	}
}

Semaphore::~Semaphore()
{
	::CloseHandle(_internal->_handle);

	delete _internal;
}

void Semaphore::Wait()
{
	// Try to enter the semaphore gate.
	DWORD dwWaitResult = ::WaitForSingleObject(_internal->_handle, INFINITE);
	switch (dwWaitResult)
	{
	// The semaphore object was signaled.
	case WAIT_OBJECT_0:
		break;
	// Semaphore was nonsignaled.
	case WAIT_TIMEOUT:
		SEthrow(Exception("Semaphore was nonsignaled."));
	}
}

void Semaphore::Wait(int32 millisecondsTimeout)
{
	// Try to enter the semaphore gate.
	DWORD dwWaitResult = ::WaitForSingleObject(_internal->_handle, millisecondsTimeout);
	switch (dwWaitResult)
	{
	// The semaphore object was signaled.
	case WAIT_OBJECT_0:
		break;
	// Semaphore was nonsignaled, so a time-out occurred.
	case WAIT_TIMEOUT:
		SEthrow(Exception("Semaphore was nonsignaled, so a time-out occurred."));
	}
}

int32 Semaphore::Release(int32 releaseCount)
{
	int32 nPreviousCount;
	if (::ReleaseSemaphore(_internal->_handle, releaseCount, (LPLONG)&nPreviousCount) == 0)
	{
		SEthrow(Exception("Failed increasing the count of the semaphore object."));
		return 0;
	}
	return nPreviousCount;
}

}
