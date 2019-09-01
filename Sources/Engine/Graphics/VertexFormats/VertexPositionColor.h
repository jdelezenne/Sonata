/*=============================================================================
VertexPositionColor.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_VERTEXPOSITIONCOLOR_H_
#define _SE_VERTEXPOSITIONCOLOR_H_

#include "Graphics/Common.h"
#include "Graphics/Model/VertexElement.h"

namespace SonataEngine
{

/**
	@brief VertexPositionColor.
*/
struct SE_GRAPHICS_EXPORT VertexPositionColor
{
public:
	static const VertexElement VertexElements[2];
	static const int ElementCount;
	static const int SizeInBytes;
	Vector3 Position;
	uint32 Color;

public:
	VertexPositionColor(const Vector3& position, const Color32& color);
};

}

#endif
