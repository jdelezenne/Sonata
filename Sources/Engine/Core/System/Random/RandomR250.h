/*=============================================================================
RandomR250.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_RANDOMR250_H_
#define _SE_RANDOMR250_H_

#include "Core/System/Random/Random.h"
#include "Core/System/Random/RandomStd.h"

namespace SonataEngine
{

/** Fast random number generator.

	Period = 2^249

	Kirkpatrick, S., and E. Stoll, 1981; A Very Fast Shift-Register
	Sequence Random Number Generator, Journal of Computational Physics,
	V. 40.

	Maier, W.L., 1991; A Fast Pseudo Random Number Generator,
	Dr. Dobb's Journal, May, pp. 152 - 157
*/
class SE_CORE_EXPORT RandomR250 : public Random
{
public:
	RandomR250(Random& init = RandomStd());
	RandomR250(int32 seed, Random& init = RandomStd());

	virtual void GenerateSeed();
	virtual int32 RandomInt();

private:
	Random& _Init;
	int32 _Buffer[250];
	int32 _Index;
};

extern RandomR250 SE_RandomR250;

}

#endif 
