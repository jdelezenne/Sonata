/*=============================================================================
Mutex.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_MUTEX_H_
#define _SE_MUTEX_H_

#include "Core/Common.h"
#include "Core/Threading/Threading.h"

namespace SonataEngine
{

class MutexInternal;

/**
	@class Mutex
	@group Threading
	@brief A synchronization primitive that can also be used for interprocess synchronization.
*/
class SE_CORE_EXPORT Mutex
{
public:
	/** Initializes a new instance of the Mutex class. */
	Mutex();

	/** Destructor. */
	~Mutex();

	/** Acquires an exclusive lock on the specified object. */
	void Enter();

	/** Attempts to acquire an exclusive lock on the specified object. */
	void TryEnter();

	/** Releases an exclusive lock on the specified object. */
	void Exit();

private:
	MutexInternal* _internal;
};


/**
	@class MutexLocker
	@group Threading
	@brief A convenience class that simplifies locking and unlocking mutexes.
*/
class SE_CORE_EXPORT MutexLocker
{
private:
	Mutex* _mutex;

public:
	/**
		Initializes a new instance of the MutexLocker class, specifying the Mutex to lock. The mutex will be unlocked when the MutexLocker is destroyed.
		@param mutex
			The Mutex to lock.
	*/
	explicit MutexLocker(Mutex* mutex);

	/** Destructor. */
	~MutexLocker();
	
	/** Gets the mutex. */
	Mutex* GetMutex();

	/** Acquires an exclusive lock on the mutex. */
	void Enter();

	/** Releases an exclusive lock on the mutex. */
	void Exit();
};


SE_INLINE MutexLocker::MutexLocker(Mutex* mutex) :
	_mutex(mutex)
{
	Enter();
}

SE_INLINE MutexLocker::~MutexLocker()
{
	Exit();
}

SE_INLINE void MutexLocker::Enter()
{
	if (_mutex != NULL)
	{
		_mutex->Enter();
	}
}

SE_INLINE void MutexLocker::Exit()
{
	if (_mutex != NULL)
	{
		_mutex->Exit();
	}
}

}

#endif 
