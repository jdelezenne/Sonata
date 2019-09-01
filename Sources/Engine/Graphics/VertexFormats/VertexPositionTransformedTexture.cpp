/*=============================================================================
VertexPositionTransformedTexture.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "VertexPositionTransformedTexture.h"

namespace SonataEngine
{

const VertexElement VertexPositionTransformedTexture::VertexElements[] =
{
	VertexElement(0, 0, VertexFormat_Float4, VertexSemantic_PositionTransformed, 0),
	VertexElement(0, 16, VertexFormat_Float2, VertexSemantic_TextureCoordinate, 0)
};
const int VertexPositionTransformedTexture::ElementCount = 2;
const int VertexPositionTransformedTexture::SizeInBytes = sizeof(VertexPositionTransformedTexture);

VertexPositionTransformedTexture::VertexPositionTransformedTexture(const Vector4& position, const Vector2& textureCoordinate)
{
	Position = position;
	TextureCoordinate = textureCoordinate;
}

}
