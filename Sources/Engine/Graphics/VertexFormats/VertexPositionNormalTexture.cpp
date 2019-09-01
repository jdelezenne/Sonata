/*=============================================================================
VertexPositionNormalTexture.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "VertexPositionNormalTexture.h"

namespace SonataEngine
{

const VertexElement VertexPositionNormalTexture::VertexElements[] =
{
	VertexElement(0, 0, VertexFormat_Float3, VertexSemantic_Position, 0),
	VertexElement(0, 12, VertexFormat_Float3, VertexSemantic_Normal, 0),
	VertexElement(0, 24, VertexFormat_Float2, VertexSemantic_TextureCoordinate, 0)
};
const int VertexPositionNormalTexture::ElementCount = 3;
const int VertexPositionNormalTexture::SizeInBytes = sizeof(VertexPositionNormalTexture);

VertexPositionNormalTexture::VertexPositionNormalTexture(const Vector3& position, const Vector3& normal, const Vector2& textureCoordinate)
{
	Position = position;
	Normal = normal;
	TextureCoordinate = textureCoordinate;
}

}
