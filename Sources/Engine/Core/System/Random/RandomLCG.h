/*=============================================================================
RandomLCG.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_RANDOMLCG_H_
#define _SE_RANDOMLCG_H_

#include "Core/System/Random/Random.h"

namespace SonataEngine
{

/** Linear Congruential random number generator.

	Period = 2^31

	Linear Congruential Method, the "minimal standard generator"
	Fast, farly good random numbers (better than using rand)
	Park & Miller, 1988, Comm of the ACM, 31(10), pp. 1192-1201
*/
class SE_CORE_EXPORT RandomLCG : public Random
{
public:
	RandomLCG();
	RandomLCG(int32 seed);

	virtual int32 RandomInt();

private:
	static const int32 _Quotient;
	static const int32 _Remainder;
};

extern RandomLCG SE_RandomLCG;

}

#endif 
