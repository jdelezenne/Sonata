/*=============================================================================
NullSemaphore.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "Core/Threading/Semaphore.h"

namespace SonataEngine
{

class SemaphoreInternal
{
};


Semaphore::Semaphore(int initialCount, int maximumCount) :
	_internal(NULL)
{
}

Semaphore::~Semaphore()
{
}

void Semaphore::Wait()
{
}

void Semaphore::Wait(int32 millisecondsTimeout)
{
}

int32 Semaphore::Release(int32 releaseCount)
{
	return 0;
}

}
