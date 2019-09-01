/*=============================================================================
Random.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "Random.h"
#include "Core/System/TimeValue.h"
#include "Core/Exception/ArgumentOutOfRangeException.h"
#include "Core/Exception/ArgumentNullException.h"

namespace SonataEngine
{

Random::Random()
{
	SetSeed((int32)((real64)TimeValue::GetTime() / 1000.0));
}

Random::Random(int32 seed)
{
	SetSeed(seed);
}

int32 Random::GetSeed()
{
	return _seed;
}

void Random::SetSeed(int32 seed)
{
	_seed = seed;
	GenerateSeed();
}

void Random::GenerateSeed()
{
}

real Random::RandomReal()
{
	return (real)((real)RandomInt() * ((real)1.0/(real)SE_MAX_I32));
}

int32 Random::RandomInt(int32 maxValue)
{
	return (int32)(RandomInt() % (maxValue + 1));
}

int32 Random::RandomInt(int32 minValue, int32 maxValue)
{
	if (minValue > maxValue)
	{
		SEthrow(ArgumentOutOfRangeException("minValue"));
		return 0;
	}

	return (int32)((RandomInt() % (maxValue - minValue + 1)) + minValue);
}

real Random::RandomReal(real maxValue)
{
	return (maxValue * RandomReal());
}

real Random::RandomReal(real minValue, real maxValue)
{
	if (minValue > maxValue)
	{
		SEthrow(ArgumentOutOfRangeException("minValue"));
		return 0.0f;
	}

	return ((maxValue - minValue) * RandomReal()) + minValue;
}

void Random::RandomBytes(SEbyte* buffer, int32 length)
{
	if (buffer == NULL)
	{
		SEthrow(ArgumentNullException("buffer"));
		return;
	}
	if (length < 0)
	{
		SEthrow(ArgumentOutOfRangeException("length"));
		return;
	}

	int32 count = length;
	while (--count >= 0)
	{
		buffer[count] = RandomInt(255);
	}
}

}
