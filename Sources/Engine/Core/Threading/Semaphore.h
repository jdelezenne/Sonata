/*=============================================================================
Semaphore.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_SEMAPHORE_H_
#define _SE_SEMAPHORE_H_

#include "Core/Common.h"
#include "Core/Threading/Threading.h"

namespace SonataEngine
{

class SemaphoreInternal;

/**
	@class Semaphore
	@group Threading
	@brief Limits the number of threads that can access a resource or pool of resources concurrently.
*/
class SE_CORE_EXPORT Semaphore
{
public:
	/**
		Initializes a new instance of the Semaphore class, specifying the maximum number of concurrent entries and optionally reserving some entries.
		@param initialCount
			The initial number of requests for the semaphore that can be granted concurrently. 
		@param maximumCount
			The maximum number of requests for the semaphore that can be granted concurrently.
	*/
	Semaphore(int initialCount, int maximumCount);

	/** Destructor. */
	~Semaphore();

	/** Blocks the current thread until the current semaphore receives a signal. */
	void Wait();

	/**
		Blocks the current thread until the current semaphore receives a signal, using the specified number of milliseconds.
		@param millisecondsTimeout
			The number of milliseconds for which the thread is blocked.
			Specify Infinite to block the thread indefinitely.
	*/
	void Wait(int32 millisecondsTimeout);

	/** Exits the semaphore a specified number of times and returns the previous count. */
	int32 Release(int32 releaseCount = 1);

private:
	SemaphoreInternal* _internal;
};

}

#endif 
