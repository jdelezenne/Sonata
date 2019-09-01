/*=============================================================================
RandomStd.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_RANDOMSTD_H_
#define _SE_RANDOMSTD_H_

#include "Core/System/Random/Random.h"

namespace SonataEngine
{

/** Standard number generator.

	Period = 2^15

	Represents a pseudo-random number generator, a device that produces a sequence of numbers that meet certain statistical requirements for randomness.
	@remark The standard implementation of rand() is limited to RAND_MAX = 0x7fff.
*/
class SE_CORE_EXPORT RandomStd : public Random
{
public:
	RandomStd();
	RandomStd(int32 seed);

	virtual void GenerateSeed();
	virtual int32 RandomInt();
	virtual real RandomReal();
};

extern RandomStd SE_RandomStd;

}

#endif 
