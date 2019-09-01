/*=============================================================================
OGPRVertexFuncEnv.h
Project: Sonata Engine
Copyright � 2005
Julien Delezenne
=============================================================================*/

#ifndef _OGPRVERTEXFUNCENV_H_
#define _OGPRVERTEXFUNCENV_H_

#include <stdlib.h>
#include "GPR.h"

class OGPRVertexFuncEnv
{
public:
	OGPRVertexFuncEnv(GPRVertexFuncEnv *env)
	{
		vertexenv = env;
	}

	virtual ~OGPRVertexFuncEnv()
	{
	}

	GPRRESULT ReadBuffer(UINT bufferIndex, UINT nCoords, const float *coords, void *dst);
	GPRRESULT ReadConstant(UINT constantIndex, void *dst);
	GPRRESULT ReadStream(UINT streamIndex, void *dst);
	GPRRESULT SampleBuffer(UINT bufferIndex, UINT nCoords, const float *coords, float *dst);
	GPRRESULT WriteInterpolant(UINT interpolantIndex, const void *value);
	GPRRESULT WritePosition(UINT dimensions, const float *pos);

private:
	GPRVertexFuncEnv *vertexenv;
};

#include "OGPRVertexFuncEnv.cpp"

#endif
