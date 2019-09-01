/*=============================================================================
OGPRBuffer.h
Project: Sonata Engine
Copyright � 2005
Julien Delezenne
=============================================================================*/

#ifndef _OGPRBUFFER_H_
#define _OGPRBUFFER_H_

#include <stdlib.h>
#include "GPR.h"

class OGPRBuffer
{
public:
	OGPRBuffer(GPRBuffer *b)
	{
		buffer = b;
	}

	virtual ~OGPRBuffer()
	{
	}

	GPRBuffer *GetBuffer() { return buffer; }

	UINT AddRef();
	GPRRESULT Clear(const void *value);
	GPRRESULT Clone(OGPRBuffer **cloneBuffer);
	GPRRESULT GetDesc(GPRBUFFER_DESC *desc);
	bool IsLocked();
	GPRRESULT Lock(GPRLOCK mode, void **data);
	UINT Release();
	GPRRESULT Resize(const UINT *bufferDimensions);
	GPRRESULT Unlock(void **data);

private:
	GPRBuffer *buffer;
};

#include "OGPRBuffer.cpp"

#endif
