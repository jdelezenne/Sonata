/*=============================================================================
Win32Mutex.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include <windows.h>

#include "Core/Threading/Mutex.h"
#include "Core/Exception/Exception.h"

namespace SonataEngine
{

class MutexInternal
{
public:
	MutexInternal();

public:
	HANDLE _handle;
};


MutexInternal::MutexInternal() :
	_handle(NULL)
{
}


Mutex::Mutex() :
	_internal(new MutexInternal())
{
	_internal->_handle = ::CreateMutex(NULL, FALSE, NULL);
	if (_internal->_handle == NULL)
	{
		SEthrow(Exception("Failed creating the mutex object."));
	}
}

Mutex::~Mutex()
{
	::CloseHandle(_internal->_handle);

	delete _internal;
}

void Mutex::Enter()
{
	// Request ownership of mutex.
	DWORD dwWaitResult = ::WaitForSingleObject(_internal->_handle, INFINITE);
	switch (dwWaitResult)
	{
	// The thread got mutex ownership.
	case WAIT_OBJECT_0:
		break;
	// Got ownership of the abandoned mutex object.
	case WAIT_ABANDONED:
		SEthrow(Exception("Got ownership of the abandoned mutex object."));
	}
}

void Mutex::TryEnter()
{
	// Request ownership of mutex.
	DWORD dwWaitResult = ::WaitForSingleObject(_internal->_handle, 10);
	switch (dwWaitResult)
	{
	// The thread got mutex ownership.
	case WAIT_OBJECT_0:
		break;
	// Cannot get mutex ownership due to time-out.
	case WAIT_TIMEOUT:
		SEthrow(Exception("Cannot get mutex ownership due to time-out."));
	// Got ownership of the abandoned mutex object.
	case WAIT_ABANDONED:
		SEthrow(Exception("Got ownership of the abandoned mutex object."));
	}
}

void Mutex::Exit()
{
	if (::ReleaseMutex(_internal->_handle) == 0)
	{
		SEthrow(Exception("Failed releasing ownership of the mutex object."));
	}
}

}
