/*=============================================================================
VertexPositionTexture.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_VERTEXPOSITIONTEXTURE_H_
#define _SE_VERTEXPOSITIONTEXTURE_H_

#include "Graphics/Common.h"
#include "Graphics/Model/VertexElement.h"

namespace SonataEngine
{

/**
	@brief VertexPositionTexture.
*/
struct SE_GRAPHICS_EXPORT VertexPositionTexture
{
public:
	static const VertexElement VertexElements[2];
	static const int ElementCount;
	static const int SizeInBytes;
	Vector3 Position;
	Vector2 TextureCoordinate;

public:
	VertexPositionTexture(const Vector3& position, const Vector2& textureCoordinate);
};

}

#endif
