/*=============================================================================
Interlocked.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_INTERLOCKED_H_
#define _SE_INTERLOCKED_H_

#include "Core/Common.h"
#include "Core/Threading/Threading.h"

namespace SonataEngine
{

/**
	@class Interlocked
	@group Threading
	@brief Provides atomic operations for variables that are shared by multiple threads.
	@remarks
		The volatile keyword is a type qualifier used to declare that an object can be modified in the program by something such as the operating system, the hardware, or a concurrently executing thread.
*/
class SE_CORE_EXPORT Interlocked
{
public:
	/**
		Sets a variable to a specified value as an atomic operation.
		@param location
			The variable to set to the specified value.
		@param value
			The value to which the location parameter is set.
		@return The original value of location.
	*/
	static void* Exchange(volatile void** location, void* value);
	static int32 Exchange(volatile int32* location, int32 value);

	/**
		Compares two values for equality and, if they are equal, replaces one of the values.
		@param location
			The destination, whose value is compared with comparand and possibly replaced.
		@param value
			The value that replaces the destination value if the comparison results in equality.
		@param comparand
			The value that is compared to the value at location.
		@return The new value stored at location.
	*/
	static void* CompareExchange(volatile void** location, void* value, void* comparand);
	static int32 CompareExchange(volatile int32* location, int32 value, int32 comparand);

	/**
		Increments a specified variable and stores the result, as an atomic operation.
		@param location
			The variable whose value is to be incremented.
		@return The incremented value.
	*/
	static int32 Increment(volatile int32* location);

	/**
		Decrements a specified variable and stores the result, as an atomic operation.
		@param location
			The variable whose value is to be decremented.
		@return The decremented value.
	*/
	static int32 Decrement(volatile int32* location);

	/**
		Adds two integers and replaces the first integer with the sum, as an atomic operation.
		@param location
			A variable containing the first value to be added. The sum of the two values is stored in location.
		@param value
			The value to be added to the integer at location.
		@return The new value stored at location.
	*/
	static int32 Add(volatile int32* location, int32 value);
};

}

#if defined(WIN32)
#	include "Platforms/Win32/Win32Interlocked.inl"
#elif defined(XBOX)
#	include "Platforms/Xbox/XboxInterlocked.inl"
#elif defined(XENON)
#	include "Platforms/Xenon/XenonInterlocked.inl"
#else
#	include "Platforms/Std/StdInterlocked.inl"
#endif

#endif 
