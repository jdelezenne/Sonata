/*=============================================================================
Memory.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "Memory.h"

namespace SonataEngine
{
/*
#if SE_USE_MEMORYINFO
	void* operator new(size_t size, const char* file, int line)
	{
		return Memory::Alloc(size);
	}

	void* operator new[](size_t size, const char* file, int line)
	{
		return Memory::Alloc(size);
	}

	void operator delete(void* ptr, const char* file, int line)
	{
		if (ptr != NULL)
		{
			Memory::Free(ptr);
		}
	}

	void operator delete[](void* ptr, const char* file, int line)
	{
		if (ptr != NULL)
		{
			Memory::Free(ptr);
		}
	}
#else
	void* operator new(size_t size)
	{
		return Memory::Alloc(size);
	}

	void* operator new[](size_t size)
	{
		return Memory::Alloc(size);
	}

	void operator delete(void* ptr)
	{
		if (ptr != NULL)
		{
			Memory::Free(ptr);
		}
	}

	void operator delete[](void* ptr)
	{
		if (ptr != NULL)
		{
			Memory::Free(ptr);
		}
	}
#endif
*/
}
