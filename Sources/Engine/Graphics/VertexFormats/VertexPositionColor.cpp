/*=============================================================================
VertexPositionColor.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "VertexPositionColor.h"

namespace SonataEngine
{

const VertexElement VertexPositionColor::VertexElements[] =
{
	VertexElement(0, 0, VertexFormat_Float3, VertexSemantic_Position, 0),
	VertexElement(0, 12, VertexFormat_Color, VertexSemantic_Color, 0)
};
const int VertexPositionColor::ElementCount = 2;
const int VertexPositionColor::SizeInBytes = sizeof(VertexPositionColor);

VertexPositionColor::VertexPositionColor(const Vector3& position, const Color32& color)
{
	Position = position;
	Color = color.ToARGB();
}

}
