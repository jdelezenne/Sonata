/*=============================================================================
RenderData.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "RenderData.h"

namespace SonataEngine
{

VertexStream::VertexStream() :
	VertexBuffer(NULL),
	Stride(0)
{
}

VertexStream::VertexStream(HardwareBufferPtr vertexBuffer, uint32 stride) :
	VertexBuffer(vertexBuffer),
	Stride(stride)
{
}

VertexStream::~VertexStream()
{
	//SE_RELEASE(VertexBuffer);
}


VertexData::VertexData() :
	VertexLayout(NULL),
	VertexCount(0)
{
}

VertexData::~VertexData()
{
	SE_RELEASE(VertexLayout);
}


IndexData::IndexData() :
	IndexBuffer(NULL),
	IndexCount(0)
{
}

IndexData::~IndexData()
{
	//SE_RELEASE(IndexBuffer);
}


RenderData::RenderData() :
	Type(PrimitiveType_Undefined),
	PrimitiveCount(0),
	VertexData(NULL),
	IndexData(NULL),
	IsIndexed(false),
	StartVertex(0),
	StartIndex(0)
{
}

RenderData::~RenderData()
{
}

uint32 RenderData::GetPrimitiveCount(PrimitiveType type, uint32 vertexCount)
{
	switch (type)
	{
	case PrimitiveType_PointList:
		return vertexCount;
	case PrimitiveType_LineList:
		return vertexCount / 2;
	case PrimitiveType_LineStrip:
		return vertexCount - 1;
	case PrimitiveType_TriangleList:
		return vertexCount / 3;
	case PrimitiveType_TriangleStrip:
		return vertexCount - 2;
	case PrimitiveType_TriangleFan:
		return vertexCount - 2;
	case PrimitiveType_QuadList:
		return vertexCount / 4;
    case PrimitiveType_QuadStrip:
		return vertexCount;
    case PrimitiveType_Polygon:
		return 1;
	default:
		return 0;
	}
}

}
