/*=============================================================================
VertexPosition.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "VertexPosition.h"

namespace SonataEngine
{

const VertexElement VertexPosition::VertexElements[] =
{
	VertexElement(0, 0, VertexFormat_Float3, VertexSemantic_Position, 0)
};
const int VertexPosition::ElementCount = 1;
const int VertexPosition::SizeInBytes = sizeof(VertexPosition);

VertexPosition::VertexPosition(const Vector3& position)
{
	Position = position;
}

}
