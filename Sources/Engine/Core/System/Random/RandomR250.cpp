/*=============================================================================
RandomR250.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "RandomR250.h"

namespace SonataEngine
{

RandomR250 SE_RandomR250;

RandomR250::RandomR250(Random& init) :
	Random(),
	_Init(init)
{
}

RandomR250::RandomR250(int32 seed, Random& init) :
	Random(seed),
	_Init(init)
{
}

void RandomR250::GenerateSeed()
{
	_Init.SetSeed(_seed);
	_Index = 0;

	int32 j;
	// fill r250 buffer with bit values
	for (j = 0; j < 250; j++)
	{
		_Buffer[j] = _Init.RandomInt();
	}

	// set some MSBs to 1
	for (j = 0; j < 250; j++)
	{
		if (_Init.RandomInt() > 0x40000000L)
		{
			_Buffer[j] |= 0x80000000L;
		}
	}


	uint32 msb  = 0x80000000; // turn on diagonal bit
	uint32 mask = 0xffffffff; // turn off the leftmost bits

	for (j = 0; j < 32; j++)
	{
		int32 k = 7 * j + 3; // select a word to operate on
		_Buffer[k] &= mask;  // turn off bits left of the diagonal 
		_Buffer[k] |= msb;   // turn on the diagonal bit 
		mask >>= 1;
		msb >>= 1;
	}
}

int32 RandomR250::RandomInt()
{
	int32 j;

	// wrap pointer around 
	if (_Index >= 147)
		j = _Index - 147;   
	else
		j = _Index + 103;

	uint32 new_rand = _Buffer[_Index] ^ _Buffer[j];
	_Buffer[_Index] = new_rand;

	// increment pointer for next time
	if (_Index >= 249)
		_Index = 0;
	else
		_Index++;

	return new_rand >> 1;
}

}
