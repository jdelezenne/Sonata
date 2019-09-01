/*=============================================================================
RenderData.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_RENDERDATA_H_
#define _SE_RENDERDATA_H_

#include "Graphics/Common.h"
#include "Graphics/Model/VertexLayout.h"
#include "Graphics/System/HardwareBuffer.h"

namespace SonataEngine
{

class SE_GRAPHICS_EXPORT VertexStream
{
public:
	HardwareBufferPtr VertexBuffer;
	uint32 Stride;

public:
	VertexStream();
	VertexStream(HardwareBufferPtr vertexBuffer, uint32 stride);
	~VertexStream();
};


class SE_GRAPHICS_EXPORT VertexData
{
public:
	VertexLayoutPtr VertexLayout;
	BaseArray<VertexStream> VertexStreams;
	uint32 VertexCount;

public:
	VertexData();
	~VertexData();
};


class SE_GRAPHICS_EXPORT IndexData
{
public:
	HardwareBufferPtr IndexBuffer;
	int32 IndexCount;

public:
	IndexData();
	~IndexData();
};


/**
	@brief Render data.
	
	This class describes a renderable object to be sent to the render system.
*/
class SE_GRAPHICS_EXPORT RenderData
{
public:
	PrimitiveType Type;
	uint32 PrimitiveCount;
	VertexData* VertexData;
	IndexData* IndexData;
	bool IsIndexed;
	uint32 StartVertex;
	uint32 StartIndex;

public:
	RenderData();
	~RenderData();

	/**
		Returns the number of primitives from a primitive type and a number
		of vertices.
		@param type The type of primitive.
		@param vertexCount The numer of vertices.
		@return The number of primitive.
	*/
	static uint32 GetPrimitiveCount(PrimitiveType type, uint32 vertexCount);
};

}

#endif
