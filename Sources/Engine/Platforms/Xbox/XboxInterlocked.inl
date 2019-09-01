/*=============================================================================
XboxInterlocked.inl
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include <xtl.h>

namespace SonataEngine
{

SE_INLINE void* Interlocked::Exchange(volatile void** location, void* value)
{
	return (void*)InterlockedExchangePointer((PVOID*)location, (PVOID)value);
}

SE_INLINE int32 Interlocked::Exchange(volatile int32* location, int32 value)
{
	return (int32)InterlockedExchange((LONG*)location, (LONG)value);
}

SE_INLINE void* Interlocked::CompareExchange(volatile void** location, void* value, void* comparand)
{
	return (void*)InterlockedCompareExchangePointer((PVOID*)location, (PVOID)value, (PVOID)comparand);
}

SE_INLINE int32 Interlocked::CompareExchange(volatile int32* location, int32 value, int32 comparand)
{
	return (int32)InterlockedCompareExchange((LONG*)location, (LONG)value, (LONG)comparand);
}

SE_INLINE int32 Interlocked::Increment(volatile int32* location)
{
	return (int32)InterlockedIncrement((LONG*)location);
}

SE_INLINE int32 Interlocked::Decrement(volatile int32* location)
{
	return (int32)InterlockedDecrement((LONG*)location);
}

SE_INLINE int32 Interlocked::Add(volatile int32* location, int32 value)
{
	return (int32)InterlockedExchangeAdd((LONG*)location, (LONG)value);
}

}
