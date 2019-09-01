/*=============================================================================
VertexPositionTransformedTexture.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_VERTEXPOSITIONTRANSFORMEDTEXTURE_H_
#define _SE_VERTEXPOSITIONTRANSFORMEDTEXTURE_H_

#include "Graphics/Common.h"
#include "Graphics/Model/VertexElement.h"

namespace SonataEngine
{

/**
	@brief VertexPositionTransformedTexture.
*/
struct SE_GRAPHICS_EXPORT VertexPositionTransformedTexture
{
public:
	static const VertexElement VertexElements[2];
	static const int ElementCount;
	static const int SizeInBytes;
	Vector4 Position;
	Vector2 TextureCoordinate;

public:
	VertexPositionTransformedTexture(const Vector4& position, const Vector2& textureCoordinate);
};

}

#endif
