/*=============================================================================
RandomStd.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include <stdlib.h>
#include "RandomStd.h"

namespace SonataEngine
{

RandomStd SE_Random;

RandomStd::RandomStd() :
	Random()
{
}

RandomStd::RandomStd(int32 seed) :
	Random(seed)
{
}

void RandomStd::GenerateSeed()
{
	srand(_seed);
}

int32 RandomStd::RandomInt()
{
	//return (rand() / (RAND_MAX + 1));
	return rand();
}

real RandomStd::RandomReal()
{
	return (real)((real)RandomInt() * ((real)1.0/(real)RAND_MAX));
}

}
