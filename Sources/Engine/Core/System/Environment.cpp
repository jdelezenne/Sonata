/*=============================================================================
Environment.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "Environment.h"

namespace SonataEngine
{

void Environment::SwapEndian(SEbyte* data, int size, int count)
{
	if (size == 1)
		return;

	SEbyte* d = data;

	for (int j = 0; j < count; ++j)
	{
		for (int i = 0; i < size/2; ++i)
		{
			SEbyte swap = d[i];
			d[i] = d[size-1-i];
			d[size-1-i] = swap;
		}
		
		d += size;
	}
}

}
