/*=============================================================================
OGPRVertexFuncEnv.cpp
Project: Sonata Engine
Copyright � 2005
Julien Delezenne
=============================================================================*/

#include "OGPRVertexFuncEnv.h"

inline GPRRESULT OGPRVertexFuncEnv::ReadBuffer(UINT bufferIndex, UINT nCoords, const float *coords, void *dst)
{
	return GPRVertexFuncEnv_ReadBuffer(vertexenv, bufferIndex, nCoords, coords, dst);
}

inline GPRRESULT OGPRVertexFuncEnv::ReadConstant(UINT constantIndex, void *dst)
{
	return GPRVertexFuncEnv_ReadConstant(vertexenv, constantIndex, dst);
}

inline GPRRESULT OGPRVertexFuncEnv::ReadStream(UINT streamIndex, void *dst)
{
	return GPRVertexFuncEnv_ReadStream(vertexenv, streamIndex, dst);
}

inline GPRRESULT OGPRVertexFuncEnv::SampleBuffer(UINT bufferIndex, UINT nCoords, const float *coords, float *dst)
{
	return GPRVertexFuncEnv_SampleBuffer(vertexenv, bufferIndex, nCoords, coords, dst);
}

inline GPRRESULT OGPRVertexFuncEnv::WriteInterpolant(UINT interpolantIndex, const void *value)
{
	return GPRVertexFuncEnv_WriteInterpolant(vertexenv, interpolantIndex, value);
}

inline GPRRESULT OGPRVertexFuncEnv::WritePosition(UINT dimensions, const float *pos)
{
	return GPRVertexFuncEnv_WritePosition(vertexenv, dimensions, pos);
}
