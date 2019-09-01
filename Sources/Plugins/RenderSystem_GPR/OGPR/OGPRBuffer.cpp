/*=============================================================================
OGPRBuffer.cpp
Project: Sonata Engine
Copyright � 2005
Julien Delezenne
=============================================================================*/

#include "OGPRBuffer.h"

inline UINT OGPRBuffer::AddRef()
{
	return GPRBuffer_AddRef(buffer);
}

inline GPRRESULT OGPRBuffer::Clear(const void *value)
{
	return GPRBuffer_Clear(buffer, value);
}

inline GPRRESULT OGPRBuffer::Clone(OGPRBuffer **cloneBuffer)
{
	GPRRESULT result;
	GPRBuffer* gprBuffer;
	result = GPRBuffer_Clone(buffer, &gprBuffer);
	if (SUCCEEDED(result))
	{
		*cloneBuffer = new OGPRBuffer(gprBuffer);
	}

	return result;
}

inline GPRRESULT OGPRBuffer::GetDesc(GPRBUFFER_DESC *desc)
{
	return GPRBuffer_GetDesc(buffer, desc);
}

inline bool OGPRBuffer::IsLocked()
{
	return GPRBuffer_IsLocked(buffer) != 0;
}

inline GPRRESULT OGPRBuffer::Lock(GPRLOCK mode, void **data)
{
	return GPRBuffer_Lock(buffer, mode, data);
}

inline UINT OGPRBuffer::Release()
{
	return GPRBuffer_Release(buffer);
}

inline GPRRESULT OGPRBuffer::Resize(const UINT *bufferDimensions)
{
	return GPRBuffer_Resize(buffer, bufferDimensions);
}

inline GPRRESULT OGPRBuffer::Unlock(void **data)
{
	return GPRBuffer_Unlock(buffer, data);
}
