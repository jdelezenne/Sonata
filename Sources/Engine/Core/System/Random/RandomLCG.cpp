/*=============================================================================
RandomLCG.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "RandomLCG.h"

namespace SonataEngine
{

RandomLCG SE_RandomLCG;

const int32 RandomLCG::_Quotient  = SE_MAX_I32 / 16807L;
const int32 RandomLCG::_Remainder = SE_MAX_I32 % 16807L;

RandomLCG::RandomLCG() :
	Random()
{
}

RandomLCG::RandomLCG(int32 seed) :
	Random(seed)
{
}

int32 RandomLCG::RandomInt()
{
	if (_seed <= _Quotient)
	{
		_seed = (_seed * 16807L) % SE_MAX_I32;
	}
	else
	{
		int32 high_part = _seed / _Quotient;
		int32 low_part  = _seed % _Quotient;

		int32 test = (16807L * low_part) - (_Remainder * high_part);

		if ( test > 0 )
			_seed = test;
		else
			_seed = test + SE_MAX_I32;
	}
	return _seed; 
}

}
