/*=============================================================================
StdMemory.inl
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include <memory.h>

namespace SonataEngine
{

SE_INLINE void Memory::Initialize()
{
}

SE_INLINE void Memory::Terminate()
{
}

SE_INLINE void* Memory::Alloc(size_t size)
{
	return malloc(size);
}

SE_INLINE void* Memory::Calloc(size_t num, size_t size)
{
	return calloc(num, size);
}

SE_INLINE void* Memory::ReAlloc(void* ptr, size_t size)
{
	return realloc(ptr, size);
}

SE_INLINE void Memory::Free(void* ptr)
{
	free(ptr);
}

SE_INLINE void Memory::Set(void* ptr, SEbyte value, size_t size)
{
	memset(ptr, value, size);
}

SE_INLINE void Memory::Zero(void* ptr, size_t size)
{
	memset(ptr, 0, size);
}

SE_INLINE void Memory::Move(void* destination, void* source, size_t size)
{
	memmove(destination, source, size);
}

SE_INLINE void Memory::Copy(void* destination, void* source, size_t size)
{
	memcpy(destination, source, size);
}

SE_INLINE size_t Memory::Compare(void* left, void* right, size_t size)
{
	return memcmp(left, right, size);
}

}
