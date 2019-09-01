/*=============================================================================
Win32Memory.inl
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "Win32Platform.h"

namespace SonataEngine
{

SE_INLINE Memory::Memory()
{
}

SE_INLINE void Memory::Initialize()
{
}

SE_INLINE void Memory::Terminate()
{
}

SE_INLINE void* Memory::Alloc(size_t size)
{
	return ::HeapAlloc(::GetProcessHeap(), 0, size);
}

SE_INLINE void* Memory::Calloc(size_t num, size_t size)
{
	return ::HeapAlloc(::GetProcessHeap(), HEAP_ZERO_MEMORY, num * size);
}

SE_INLINE void* Memory::ReAlloc(void* ptr, size_t size)
{
	return ::HeapReAlloc(::GetProcessHeap(), 0, ptr, size);
}

SE_INLINE void Memory::Free(void* ptr)
{
	::HeapFree(::GetProcessHeap(), 0, ptr);
}

SE_INLINE void Memory::Set(void* ptr, SEbyte value, size_t size)
{
	::FillMemory(ptr, size, value);
}

SE_INLINE void Memory::Zero(void* ptr, size_t size)
{
	::ZeroMemory(ptr, size);
}

SE_INLINE void Memory::Move(void* destination, void* source, size_t size)
{
	::MoveMemory(destination, source, size);
}

SE_INLINE void Memory::Copy(void* destination, void* source, size_t size)
{
	::CopyMemory(destination, source, size);
}

SE_INLINE size_t Memory::Compare(void* left, void* right, size_t size)
{
	return memcmp(left, right, size);
}

}
