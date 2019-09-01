/*=============================================================================
VertexPositionColorTexture.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_VERTEXPOSITIONCOLORTEXTURE_H_
#define _SE_VERTEXPOSITIONCOLORTEXTURE_H_

#include "Graphics/Common.h"
#include "Graphics/Model/VertexElement.h"

namespace SonataEngine
{

/**
	@brief VertexPositionColorTexture.
*/
struct SE_GRAPHICS_EXPORT VertexPositionColorTexture
{
public:
	static const VertexElement VertexElements[3];
	static const int ElementCount;
	static const int SizeInBytes;
	Vector3 Position;
	uint32 Color;
	Vector2 TextureCoordinate;

public:
	VertexPositionColorTexture(const Vector3& position, const Color32& color, const Vector2& textureCoordinate);
};

}

#endif
