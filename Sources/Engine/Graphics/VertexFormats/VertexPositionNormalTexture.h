/*=============================================================================
VertexPositionNormalTexture.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_VERTEXPOSITIONNORMALTEXTURE_H_
#define _SE_VERTEXPOSITIONNORMALTEXTURE_H_

#include "Graphics/Common.h"
#include "Graphics/Model/VertexElement.h"

namespace SonataEngine
{

/**
	@brief VertexPositionNormalTexture.
*/
struct SE_GRAPHICS_EXPORT VertexPositionNormalTexture
{
public:
	static const VertexElement VertexElements[3];
	static const int ElementCount;
	static const int SizeInBytes;
	Vector3 Position;
	Vector3 Normal;
	Vector2 TextureCoordinate;

public:
	VertexPositionNormalTexture(const Vector3& position, const Vector3& normal, const Vector2& textureCoordinate);
};

}

#endif
