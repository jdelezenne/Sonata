/*=============================================================================
VertexPositionColorTexture.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "VertexPositionColorTexture.h"

namespace SonataEngine
{

const VertexElement VertexPositionColorTexture::VertexElements[] =
{
	VertexElement(0, 0, VertexFormat_Float3, VertexSemantic_Position, 0),
	VertexElement(0, 12, VertexFormat_Color, VertexSemantic_Color, 0),
	VertexElement(0, 16, VertexFormat_Float2, VertexSemantic_TextureCoordinate, 0)
};
const int VertexPositionColorTexture::ElementCount = 3;
const int VertexPositionColorTexture::SizeInBytes = sizeof(VertexPositionColorTexture);

VertexPositionColorTexture::VertexPositionColorTexture(const Vector3& position, const Color32& color, const Vector2& textureCoordinate)
{
	Position = position;
	Color = color.ToARGB();
	TextureCoordinate = textureCoordinate;
}

}
