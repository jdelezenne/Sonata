/*=============================================================================
OGPRFragmentFuncEnv.cpp
Project: Sonata Engine
Copyright � 2005
Julien Delezenne
=============================================================================*/

#include "OGPRFragmentFuncEnv.h"

inline GPRRESULT OGPRFragmentFuncEnv::GetDepth(float *dst) const
{
	return GPRFragmentFuncEnv_GetDepth(fragmentenv, dst);
}

inline GPRRESULT OGPRFragmentFuncEnv::ReadBuffer(UINT bufferIndex, UINT nCoords, const float *coords, void *dst) const
{
	return GPRFragmentFuncEnv_ReadBuffer(fragmentenv, bufferIndex, nCoords, coords, dst);
}

inline GPRRESULT OGPRFragmentFuncEnv::ReadConstant(UINT constantIndex, void *dst) const
{
	return GPRFragmentFuncEnv_ReadConstant(fragmentenv, constantIndex, dst);
}

inline GPRRESULT OGPRFragmentFuncEnv::ReadInterpolant(UINT interpolantIndex, float *dst) const
{
	return GPRFragmentFuncEnv_ReadInterpolant(fragmentenv, interpolantIndex, dst);
}

inline GPRRESULT OGPRFragmentFuncEnv::ReadPixel(UINT bufferIndex, void *dst) const
{
	return GPRFragmentFuncEnv_ReadPixel(fragmentenv, bufferIndex, dst);
}

inline GPRRESULT OGPRFragmentFuncEnv::SampleBuffer(UINT bufferIndex, UINT nCoords, const float *coords, float *dst)
{
	return GPRFragmentFuncEnv_SampleBuffer(fragmentenv, bufferIndex, nCoords, coords, dst);
}

inline GPRRESULT OGPRFragmentFuncEnv::WritePixel(UINT bufferIndex, const void *value)
{
	return GPRFragmentFuncEnv_WritePixel(fragmentenv, bufferIndex, value);
}
