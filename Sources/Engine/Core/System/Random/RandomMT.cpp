/*=============================================================================
RandomMT.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "RandomMT.h"

namespace SonataEngine
{

RandomMT SE_RandomMT;

// Period parameters
#define MTRANDOM_N 624                   // length of state vector
#define MTRANDOM_M 397                   // period parameter
#define MTRANDOM_MATRIX_A 0x9908b0dfUL   // constant vector a
#define MTRANDOM_UPPER_MASK 0x80000000UL // most significant w-r bits
#define MTRANDOM_LOWER_MASK 0x7fffffffUL // least significant r bits

// Tempering parameters
#define MTRANDOM_TEMPERING_MASK_B 0x9d2c5680UL
#define MTRANDOM_TEMPERING_MASK_C 0xefc60000UL
#define MTRANDOM_TEMPERING_SHIFT_U(y) (y >> 11)
#define MTRANDOM_TEMPERING_SHIFT_S(y) (y << 7)
#define MTRANDOM_TEMPERING_SHIFT_T(y) (y << 15)
#define MTRANDOM_TEMPERING_SHIFT_L(y) (y >> 18)

int32 MT_StateVectors[MTRANDOM_N]; // the array for the state vector
int MT_Index;

RandomMT::RandomMT() :
	Random()
{
}

RandomMT::RandomMT(int32 seed) :
	Random(seed)
{
}

void RandomMT::GenerateSeed()
{
	MT_StateVectors[0] = _seed & 0xffffffffUL;
	for (MT_Index = 1; MT_Index < MTRANDOM_N; MT_Index++)
	{
		MT_StateVectors[MT_Index] =
			(1812433253UL * (MT_StateVectors[MT_Index-1] ^
			(MT_StateVectors[MT_Index-1] >> 30)) + MT_Index); 
		// See Knuth TAOCP Vol2. 3rd Ed. P.106 for multiplier.
		// In the previous versions, MSBs of the seed affect
		// only MSBs of the array MT_StateVectors[].
		// 2002/01/09 modified by Makoto Matsumoto
		MT_StateVectors[MT_Index] &= 0xffffffffUL;
		// for >32 bit machines
	}
}

int32 RandomMT::RandomInt()
{
	int32 y;
	static int32 mag01[2] = { 0x0UL, MTRANDOM_MATRIX_A };
	// mag01[x] = x * MATRIX_A  for x=0,1

	if (MT_Index >= MTRANDOM_N)
	{
		// generate N words at one time
		int kk;

		for (kk = 0; kk < (MTRANDOM_N-MTRANDOM_M); kk++)
		{
			y = (MT_StateVectors[kk] & MTRANDOM_UPPER_MASK) | (MT_StateVectors[kk+1] & MTRANDOM_LOWER_MASK);
			MT_StateVectors[kk] = MT_StateVectors[kk+MTRANDOM_M] ^ (y >> 1) ^ mag01[y & 0x1UL];
		}
		for (; kk < (MTRANDOM_N-1); kk++)
		{
			y = (MT_StateVectors[kk] & MTRANDOM_UPPER_MASK) | (MT_StateVectors[kk+1] & MTRANDOM_LOWER_MASK);
			MT_StateVectors[kk] = MT_StateVectors[kk+(MTRANDOM_M-MTRANDOM_N)] ^ (y >> 1) ^ mag01[y & 0x1UL];
		}
		y = (MT_StateVectors[MTRANDOM_N-1] & MTRANDOM_UPPER_MASK) | (MT_StateVectors[0] & MTRANDOM_LOWER_MASK);
		MT_StateVectors[MTRANDOM_N-1] = MT_StateVectors[MTRANDOM_M-1] ^ (y >> 1) ^ mag01[y & 0x1UL];

		MT_Index = 0;
	}

	y = MT_StateVectors[MT_Index++];

	// Tempering
	y ^= MTRANDOM_TEMPERING_SHIFT_U(y);
	y ^= MTRANDOM_TEMPERING_SHIFT_S(y) & MTRANDOM_TEMPERING_MASK_B;
	y ^= MTRANDOM_TEMPERING_SHIFT_T(y) & MTRANDOM_TEMPERING_MASK_C;
	y ^= MTRANDOM_TEMPERING_SHIFT_L(y);

	return y;
}

real RandomMT::RandomReal()
{
	return (real)((real)RandomInt() * ((real)1.0/(real)SE_MAX_U32));
}

}
