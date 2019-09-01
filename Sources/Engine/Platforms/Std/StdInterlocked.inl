/*=============================================================================
StdInterlocked.inl
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

namespace SonataEngine
{

SE_INLINE void* Interlocked::Exchange(volatile void** location, void* value)
{
	volatile void* oldLocation = (*val);
	(*location) = value;
	return oldLocation;
}

SE_INLINE int32 Interlocked::Exchange(volatile int32* location, int32 value)
{
	int32 oldLocation = (*val);
	(*location) = value;
	return oldLocation;
}

SE_INLINE void* Interlocked::CompareExchange(volatile void** location, void* value, void* comparand)
{
	volatile void* oldLocation = (*location);
	if ((*location) == comparand)
	{
		(*location) = value;
	}
	return (void*)oldLocation;
}

SE_INLINE int32 Interlocked::CompareExchange(volatile int32* location, int32 value, int32 comparand)
{
	int32 oldLocation = (*location);
	if ((*location) == comparand)
	{
		(*location) = value;
	}
	return oldLocation;
}

SE_INLINE int32 Interlocked::Increment(volatile int32* location)
{
	return ++(*location);
}

SE_INLINE int32 Interlocked::Decrement(volatile int32* location)
{
	return --(*location);
}

SE_INLINE int32 Interlocked::Add(volatile int32* location, int32 value)
{
	(*location) += value;
	return *location;
}

}
