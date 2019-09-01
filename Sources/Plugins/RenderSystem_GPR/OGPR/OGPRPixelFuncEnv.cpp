/*=============================================================================
OGPRPixelFuncEnv.cpp
Project: Sonata Engine
Copyright � 2005
Julien Delezenne
=============================================================================*/

#include "OGPRPixelFuncEnv.h"

inline GPRRESULT OGPRPixelFuncEnv::GetPosition(UINT *x, UINT *y)
{
	return GPRPixelFuncEnv_GetPosition(pixelenv, x, y);
}

inline GPRRESULT OGPRPixelFuncEnv::ReadBuffer(UINT bufferIndex, UINT x, UINT y, void *dst)
{
	return GPRPixelFuncEnv_ReadBuffer(pixelenv, bufferIndex, x, y, dst);
}

inline GPRRESULT OGPRPixelFuncEnv::ReadConstant(UINT constantIndex, void *dst)
{
	return GPRPixelFuncEnv_ReadConstant(pixelenv, constantIndex, dst);
}

inline GPRRESULT OGPRPixelFuncEnv::ReadPixel(UINT bufferIndex, void *dst)
{
	return GPRPixelFuncEnv_ReadPixel(pixelenv, bufferIndex, dst);
}

inline GPRRESULT OGPRPixelFuncEnv::WritePixel(UINT bufferIndex, const void *value)
{
	return GPRPixelFuncEnv_WritePixel(pixelenv, bufferIndex, value);
}
