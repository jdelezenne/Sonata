/*=============================================================================
OGPRFragmentFuncEnv.h
Project: Sonata Engine
Copyright � 2005
Julien Delezenne
=============================================================================*/

#ifndef _OGPRFRAGMENTFUNCENV_H_
#define _OGPRFRAGMENTFUNCENV_H_

#include <stdlib.h>
#include "GPR.h"

class OGPRFragmentFuncEnv
{
public:
	OGPRFragmentFuncEnv(GPRFragmentFuncEnv *env)
	{
		fragmentenv = env;
	}

	virtual ~OGPRFragmentFuncEnv()
	{
	}

	GPRRESULT GetDepth(float *dst) const;
	GPRRESULT ReadBuffer(UINT bufferIndex, UINT nCoords, const float *coords, void *dst) const;
	GPRRESULT ReadConstant(UINT constantIndex, void *dst) const;
	GPRRESULT ReadInterpolant(UINT interpolantIndex, float *dst) const;
	GPRRESULT ReadPixel(UINT bufferIndex, void *dst) const;
	GPRRESULT SampleBuffer(UINT bufferIndex, UINT nCoords, const float *coords, float *dst);
	GPRRESULT WritePixel(UINT bufferIndex, const void *value);

private:
	GPRFragmentFuncEnv *fragmentenv;
};

#include "OGPRFragmentFuncEnv.cpp"

#endif
