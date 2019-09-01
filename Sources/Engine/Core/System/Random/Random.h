/*=============================================================================
Random.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_RANDOM_H_
#define _SE_RANDOM_H_

#include "Core/Common.h"

namespace SonataEngine
{

/**
	Random number generator interface.
*/
class SE_CORE_EXPORT Random
{
protected:
	int32 _seed;

public:
	/** Seed value. */
	//@{
	/** Retrieves the seed value. */
	int32 GetSeed();

	/** Sets the seed value. */
	void SetSeed(int32 seed);

	/** Generate the initial sequence. */
	virtual void GenerateSeed();
	//@}

	/** Random number. */
	//@{
	/** Returns a nonnegative random number.
	*/
	virtual int32 RandomInt() = 0;

	/** Returns a random number between 0.0 and 1.0. */
	virtual real RandomReal();

	/** Returns a nonnegative random number less than the specified maximum. */
	int32 RandomInt(int32 maxValue);

	/** Returns a random number within a specified range.
	@remark A 32-bit signed integer greater than or equal to minValue and less than maxValue; that is, the range of return values includes minValue but not MaxValue.
	*/
	int32 RandomInt(int32 minValue, int32 maxValue);

	/** Returns a float random number less than the specified maximum. */
	real RandomReal(real maxValue);

	/** Returns a float random number within a specified range. */
	real RandomReal(real minValue, real maxValue);

	/** Fills the elements of a specified array of bytes with random numbers. */
	void RandomBytes(SEbyte* buffer, int32 length);
	//@}

protected:
	/** @name Constructors. */
	//@{
	/** Initializes a new instance of the Random class, using a time-dependent default seed value. */
	Random();

	/** Initializes a new instance of the Random class, using the specified seed value. */
	Random(int32 seed);
	//@}
};

}

#endif 
