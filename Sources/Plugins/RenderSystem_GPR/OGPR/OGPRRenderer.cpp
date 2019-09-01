/*=============================================================================
OGPRRenderer.cpp
Project: Sonata Engine
Copyright � 2005
Julien Delezenne
=============================================================================*/

#include "OGPRRenderer.h"

inline UINT OGPRRenderer::AddRef()
{
	return GPRRenderer_AddRef(renderer);
}

inline GPRRESULT OGPRRenderer::CreateBuffer(UINT nDimensions, const UINT *bufferDimensions, GPRDECLTYPE elementType, UINT nComponents, OGPRBuffer **buffer)
{
	GPRRESULT result;
	GPRBuffer *b;
	result = GPRRenderer_CreateBuffer(renderer, nDimensions, bufferDimensions, elementType, nComponents, &b);
	if (SUCCEEDED(result))
	{
		*buffer = new OGPRBuffer(b);
	}

	return result;
}

inline GPRRESULT OGPRRenderer::DrawIndexedPrimitive(GPRPRIMITIVETYPE primitiveType, GPRBuffer *indexBuffer, UINT startIndex, UINT nPrimitives)
{
	return GPRRenderer_DrawIndexedPrimitive(renderer, primitiveType, indexBuffer, startIndex, nPrimitives);
}

inline GPRRESULT OGPRRenderer::DrawPrimitive(GPRPRIMITIVETYPE primitiveType, UINT startVertex, UINT nPrimitives)
{
	return GPRRenderer_DrawPrimitive(renderer, primitiveType, startVertex, nPrimitives);
}

inline GPRRESULT OGPRRenderer::ExecutePixelFunction(GPRPixelFunction func)
{
	return GPRRenderer_ExecutePixelFunction(renderer, func);
}

inline GPRRESULT OGPRRenderer::GetCaps(GPRCAPS *caps)
{
	return GPRRenderer_GetCaps(renderer, caps);
}

inline UINT OGPRRenderer::Release()
{
	return GPRRenderer_Release(renderer);
}

inline GPRRESULT OGPRRenderer::SetBuffer(GPRFUNCSTAGE stage, UINT bufferIndex, OGPRBuffer* buffer, GPRUSAGE usage)
{
	GPRBuffer* gprBuffer = (buffer != NULL ? buffer->GetBuffer() : NULL);
	return GPRRenderer_SetBuffer(renderer, stage, bufferIndex, gprBuffer, usage);
}

inline GPRRESULT OGPRRenderer::SetConstant(GPRFUNCSTAGE stage, UINT constantIndex, GPRDECLTYPE type, UINT nComponents, const void *value)
{
	return GPRRenderer_SetConstant(renderer, stage, constantIndex, type, nComponents, value);
}

inline GPRRESULT OGPRRenderer::SetFragmentFunction(GPRFragmentFunction func)
{
	return GPRRenderer_SetFragmentFunction(renderer, func);
}

inline GPRRESULT OGPRRenderer::SetInterpolantFormat(UINT interpolantIndex, GPRDECLTYPE type, UINT nComponents)
{
	return GPRRenderer_SetInterpolantFormat(renderer, interpolantIndex, type, nComponents);
}

inline GPRRESULT OGPRRenderer::SetRasterResolution(UINT width, UINT height)
{
	return GPRRenderer_SetRasterResolution(renderer, width, height);
}

inline GPRRESULT OGPRRenderer::SetRenderState(GPRRENDERSTATE state, DWORD value)
{
	return GPRRenderer_SetRenderState(renderer, state, value);
}

inline GPRRESULT OGPRRenderer::SetSamplerState(GPRFUNCSTAGE stage, UINT bufferIndex, GPRSAMPLERSTATE state, DWORD value)
{
	return GPRRenderer_SetSamplerState(renderer, stage, bufferIndex, state, value);
}

inline GPRRESULT OGPRRenderer::SetStreamSource(UINT streamIndex, OGPRBuffer* buffer, UINT componentOffset, UINT nComponents)
{
	GPRBuffer* gprBuffer = (buffer != NULL ? buffer->GetBuffer() : NULL);
	return GPRRenderer_SetStreamSource(renderer, streamIndex, gprBuffer, componentOffset, nComponents);
}

inline GPRRESULT OGPRRenderer::SetVertexFunction(GPRVertexFunction func)
{
	return GPRRenderer_SetVertexFunction(renderer, func);
}

inline GPRRESULT OGPRRenderer::SetViewport(UINT x, UINT y, UINT width, UINT height)
{
	return GPRRenderer_SetViewport(renderer, x, y, width, height);
}
