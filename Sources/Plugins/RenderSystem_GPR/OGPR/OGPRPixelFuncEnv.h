/*=============================================================================
OGPRPixelFuncEnv.h
Project: Sonata Engine
Copyright � 2005
Julien Delezenne
=============================================================================*/

#ifndef _OGPRPIXELFUNCENV_H_
#define _OGPRPIXELFUNCENV_H_

#include <stdlib.h>
#include "GPR.h"

class OGPRPixelFuncEnv
{
public:
	OGPRPixelFuncEnv(GPRPixelFuncEnv *env)
	{
		pixelenv = env;
	}

	virtual ~OGPRPixelFuncEnv()
	{
	}

	GPRRESULT GetPosition(UINT *x, UINT *y);
	GPRRESULT ReadBuffer(UINT bufferIndex, UINT x, UINT y, void *dst);
	GPRRESULT ReadConstant(UINT constantIndex, void *dst);
	GPRRESULT ReadPixel(UINT bufferIndex, void *dst);
	GPRRESULT WritePixel(UINT bufferIndex, const void *value);

private:
	GPRPixelFuncEnv *pixelenv;
};

#include "OGPRPixelFuncEnv.cpp"

#endif
