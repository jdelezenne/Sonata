/*=============================================================================
Memory.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_MEMORY_H_
#define _SE_MEMORY_H_

#include "Core/Common.h"

namespace SonataEngine
{
/*
#if SE_USE_MEMORYINFO
	void* operator new(size_t size, const char* file, int line);
	void* operator new[](size_t size, const char* file, int line);
	void operator delete(void* ptr, const char* file, int line);
	void operator delete[](void* ptr, const char* file, int line);
#else
	void* operator new(size_t size);
	void* operator new[](size_t size);
	void operator delete(void* ptr);
	void operator delete[](void* ptr);
#endif

#ifdef SE_USE_MEMORYINFO
#	define SE_OP_NEW(size) operator new(size, __FILE__, __LINE__)
#	define SE_NEW(size) new(__FILE__, __LINE__)
#	define SE_OP_NEW_TYPE(size, type) operator new(size, __FILE__, __LINE__, type)
#	define SE_NEW_TYPE(size) new(__FILE__, __LINE__, type)
#else
#	define SE_OP_NEW(size) operator new(size)
#	define SE_NEW(size) new()
#	define SE_OP_NEW_TYPE(size, type) operator new(size, type)
#	define SE_NEW_TYPE(type) new(type)
#endif
*/
/**
	@brief Memory manager.

	Provides memory management methods.

	@todo
		Track the memory allocations using the file and line parameters.
*/
class SE_CORE_EXPORT Memory
{
public:
	/** Initializes the memory manager. */
	static void Initialize();

	/** Terminates the memory manager. */
	static void Terminate();

	/**
		Allocates memory.
		@param size The number of bytes in memory required, in bytes.
		@return The pointer to the newly allocated memory.
	*/
	static void* Alloc(size_t size);

	/**
		Allocates an array in memory with elements initialized to zero.
		@param num The number of elements.
		@param size The number of bytes of each element, in bytes.
		@return The pointer to the newly allocated memory.
	*/
	static void* Calloc(size_t num, size_t size);

	/**
		Resizes a block of previously allocated memory.
		@param ptr The pointer to the previously allocated memory.
		@param size The number of bytes in memory required, in bytes.
		@return The pointer to the newly allocated memory.
	*/
	static void* ReAlloc(void* ptr, size_t size);

	/**
		Frees previously allocated memory.
		@param ptr The pointer to the memory to be freed.
	*/
	static void Free(void* ptr);

	/**
		Stores a value into a buffer repeatedly, for a specified number of times.
		@param ptr The pointer to the memory to copy data into.
		@param value The byte value that is to be set.
		@param size The size of the block of memory to fill, in bytes.
	*/
	static void Set(void* ptr, SEbyte value, size_t size);

	/**
		Fills a block of memory with zeros.
		@param ptr The pointer to the memory to fill with zeros.
		@param size The size of the block of memory to fill with zeros, in bytes.
	*/
	static void Zero(void* ptr, size_t size);

	/**
		Moves a block of memory from one location to another.
		@param destination The pointer to the destination memory.
		@param source The pointer to the source memory.
		@param size The size of the block of memory to move, in bytes.
	*/
	static void Move(void* destination, void* source, size_t size);

	/**
		Copies a block of memory from one location to another.
		@param destination The pointer to the destination memory.
		@param source The pointer to the source memory.
		@param size The size of the block of memory to copy, in bytes.
	*/
	static void Copy(void* destination, void* source, size_t size);

	/**
		Compares a block of memory with another.
		@param left The pointer to the first block of memory.
		@param right The pointer to the second block of memory.
		@param size The size of the block of memory to compare, in bytes.
	*/
	static size_t Compare(void* left, void* right, size_t size);

private:
	Memory();
};

}

#if defined(WIN32)
#	include "Platforms/Win32/Win32Memory.inl"
#elif defined(XBOX)
#	include "Platforms/Xbox/XboxMemory.inl"
#elif defined(XENON)
#	include "Platforms/Xenon/XenonMemory.inl"
#else
#	include "Platforms/Std/StdMemory.inl"
#endif

#endif 
