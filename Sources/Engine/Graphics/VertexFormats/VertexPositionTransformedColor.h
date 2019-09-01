/*=============================================================================
VertexPositionTransformedColor.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_VERTEXPOSITIONTRANSFORMEDCOLOR_H_
#define _SE_VERTEXPOSITIONTRANSFORMEDCOLOR_H_

#include "Graphics/Common.h"
#include "Graphics/Model/VertexElement.h"

namespace SonataEngine
{

/**
	@brief VertexPositionTransformedColor.
*/
struct SE_GRAPHICS_EXPORT VertexPositionTransformedColor
{
public:
	static const VertexElement VertexElements[2];
	static const int ElementCount;
	static const int SizeInBytes;
	Vector4 Position;
	uint32 Color;

public:
	VertexPositionTransformedColor(const Vector4& position, const Color32& color);
};

}

#endif
