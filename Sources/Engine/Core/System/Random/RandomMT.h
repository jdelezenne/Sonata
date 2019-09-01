/*=============================================================================
RandomMT.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_RANDOMMT_H_
#define _SE_RANDOMMT_H_

#include "Core/System/Random/Random.h"

namespace SonataEngine
{

/** Mersenne Twister random number generator.
	This generator is not cryptoraphically secure.

	Period = 2^32

	M. Matsumoto and T. Nishimura,
	"Mersenne Twister: A 623-Dimensionally Equidistributed Uniform  
	Pseudo-Random Number Generator",
	ACM Transactions on Modeling and Computer Simulation,
	Vol. 8, No. 1, January 1998, pp 3--30.
*/
class SE_CORE_EXPORT RandomMT : public Random
{
public:
	RandomMT();
	RandomMT(int32 seed);

	virtual void GenerateSeed();
	virtual int32 RandomInt();
	virtual real RandomReal();
};

extern RandomMT SE_RandomMT;

}

#endif 
