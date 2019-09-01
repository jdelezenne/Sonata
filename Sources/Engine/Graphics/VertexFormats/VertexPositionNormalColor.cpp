/*=============================================================================
VertexPositionNormalColor.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "VertexPositionNormalColor.h"

namespace SonataEngine
{

const VertexElement VertexPositionNormalColor::VertexElements[] =
{
	VertexElement(0, 0, VertexFormat_Float3, VertexSemantic_Position, 0),
	VertexElement(0, 12, VertexFormat_Float3, VertexSemantic_Normal, 0),
	VertexElement(0, 24, VertexFormat_Color, VertexSemantic_Color, 0)
};
const int VertexPositionNormalColor::ElementCount = 3;
const int VertexPositionNormalColor::SizeInBytes = sizeof(VertexPositionNormalColor);

VertexPositionNormalColor::VertexPositionNormalColor(const Vector3& position, const Vector3& normal, const Color32& color)
{
	Position = position;
	Normal = normal;
	Color = color.ToARGB();
}

}
