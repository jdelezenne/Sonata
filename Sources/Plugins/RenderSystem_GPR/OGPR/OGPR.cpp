// OGPR.cpp

#include "OGPR.h"

GPRRESULT OGPR_CreateRenderer(DWORD version, OGPRRenderer **renderer)
{
	GPRRESULT result;
	GPRRenderer *r;
	result = GPR_CreateRenderer(version, &r);
	if (SUCCEEDED(result))
	{
		*renderer = new OGPRRenderer(r);
	}

	return result;
}
