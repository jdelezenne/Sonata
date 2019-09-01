/*=============================================================================
VertexPositionTexture.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "VertexPositionTexture.h"

namespace SonataEngine
{

const VertexElement VertexPositionTexture::VertexElements[] =
{
	VertexElement(0, 0, VertexFormat_Float3, VertexSemantic_Position, 0),
	VertexElement(0, 12, VertexFormat_Float2, VertexSemantic_TextureCoordinate, 0)
};
const int VertexPositionTexture::ElementCount = 2;
const int VertexPositionTexture::SizeInBytes = sizeof(VertexPositionTexture);

VertexPositionTexture::VertexPositionTexture(const Vector3& position, const Vector2& textureCoordinate)
{
	Position = position;
	TextureCoordinate = textureCoordinate;
}

}
