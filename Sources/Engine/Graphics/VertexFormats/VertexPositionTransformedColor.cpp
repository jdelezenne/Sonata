/*=============================================================================
VertexPositionTransformedColor.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "VertexPositionTransformedColor.h"

namespace SonataEngine
{

const VertexElement VertexPositionTransformedColor::VertexElements[] =
{
	VertexElement(0, 0, VertexFormat_Float4, VertexSemantic_PositionTransformed, 0),
	VertexElement(0, 16, VertexFormat_Color, VertexSemantic_Color, 0)
};
const int VertexPositionTransformedColor::ElementCount = 2;
const int VertexPositionTransformedColor::SizeInBytes = sizeof(VertexPositionTransformedColor);

VertexPositionTransformedColor::VertexPositionTransformedColor(const Vector4& position, const Color32& color)
{
	Position = position;
	Color = color.ToARGB();
}

}
