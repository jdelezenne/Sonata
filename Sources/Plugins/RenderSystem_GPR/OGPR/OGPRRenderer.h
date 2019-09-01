/*=============================================================================
OGPRRenderer.h
Project: Sonata Engine
Copyright � 2005
Julien Delezenne
=============================================================================*/

#ifndef _OGPRRENDERER_H_
#define _OGPRRENDERER_H_

#include <stdlib.h>
#include "GPR.h"
#include "OGPRBuffer.h"

class OGPRRenderer
{
public:
	OGPRRenderer(GPRRenderer *r)
	{
		renderer = r;
	}

	virtual ~OGPRRenderer()
	{
	}

	GPRRenderer *GetRenderer() { return renderer; }

	UINT AddRef();
	GPRRESULT CreateBuffer(UINT nDimensions, const UINT *bufferDimensions, GPRDECLTYPE elementType, UINT nComponents, OGPRBuffer **buffer);
	GPRRESULT DrawIndexedPrimitive(GPRPRIMITIVETYPE primitiveType, GPRBuffer *indexBuffer, UINT startIndex, UINT nPrimitives);
	GPRRESULT DrawPrimitive(GPRPRIMITIVETYPE primitiveType, UINT startVertex, UINT nPrimitives);
	GPRRESULT ExecutePixelFunction(GPRPixelFunction func);
	GPRRESULT GetCaps(GPRCAPS *caps);
	UINT Release();
	GPRRESULT SetBuffer(GPRFUNCSTAGE stage, UINT bufferIndex, OGPRBuffer* buffer, GPRUSAGE usage);
	GPRRESULT SetConstant(GPRFUNCSTAGE stage, UINT constantIndex, GPRDECLTYPE type, UINT nComponents, const void *value);
	GPRRESULT SetFragmentFunction(GPRFragmentFunction func);
	GPRRESULT SetInterpolantFormat(UINT interpolantIndex, GPRDECLTYPE type, UINT nComponents);
	GPRRESULT SetRasterResolution(UINT width, UINT height);
	GPRRESULT SetRenderState(GPRRENDERSTATE state, DWORD value);
	GPRRESULT SetSamplerState(GPRFUNCSTAGE stage, UINT bufferIndex, GPRSAMPLERSTATE state, DWORD value);
	GPRRESULT SetStreamSource(UINT streamIndex, OGPRBuffer* buffer, UINT componentOffset, UINT nComponents);
	GPRRESULT SetVertexFunction(GPRVertexFunction func);
	GPRRESULT SetViewport(UINT x, UINT y, UINT width, UINT height);

private:
	GPRRenderer *renderer;
};

#include "OGPRRenderer.cpp"

#endif
