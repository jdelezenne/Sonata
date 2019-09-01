/*=============================================================================
NullInterlocked.inl
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

namespace SonataEngine
{

SE_INLINE void* Interlocked::Exchange(volatile void** location, void* value)
{
	return NULL;
}

SE_INLINE int32 Interlocked::Exchange(volatile int32* location, int32 value)
{
	return 0;
}

SE_INLINE void* Interlocked::CompareExchange(volatile void** location, void* value, void* comparand)
{
	return NULL;
}

SE_INLINE int32 Interlocked::CompareExchange(volatile int32* location, int32 value, int32 comparand)
{
	return 0;
}

SE_INLINE int32 Interlocked::Increment(volatile int32* location)
{
	return 0;
}

SE_INLINE int32 Interlocked::Decrement(volatile int32* location)
{
	return 0;
}

SE_INLINE int32 Interlocked::Add(volatile int32* location, int32 value)
{
	return 0;
}

}
