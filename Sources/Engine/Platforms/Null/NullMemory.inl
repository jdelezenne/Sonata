/*=============================================================================
NullMemory.inl
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

namespace SonataEngine
{

SE_INLINE void NullMemory::Initialize()
{
}

SE_INLINE void NullMemory::Terminate()
{
}

SE_INLINE void* NullMemory::Alloc(size_t size)
{
	return NULL;
}

SE_INLINE void* NullMemory::Calloc(size_t num, size_t size)
{
	return NULL;
}

SE_INLINE void* NullMemory::ReAlloc(void* ptr, size_t size)
{
	return NULL;
}

SE_INLINE void NullMemory::Free(void* ptr)
{
}

SE_INLINE void NullMemory::Set(void* ptr, SEbyte value, size_t size)
{
}

SE_INLINE void NullMemory::Zero(void* ptr, size_t size)
{
}

SE_INLINE void NullMemory::Move(void* destination, void* source, size_t size)
{
}

SE_INLINE void NullMemory::Copy(void* destination, void* source, size_t size)
{
}

SE_INLINE size_t NullMemory::Compare(void* left, void* right, size_t size)
{
	return 0;
}

}
