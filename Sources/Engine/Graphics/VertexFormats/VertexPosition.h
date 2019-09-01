/*=============================================================================
VertexPosition.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_VERTEXPOSITION_H_
#define _SE_VERTEXPOSITION_H_

#include "Graphics/Common.h"
#include "Graphics/Model/VertexElement.h"

namespace SonataEngine
{

/**
	@brief VertexPosition.
*/
struct SE_GRAPHICS_EXPORT VertexPosition
{
public:
	static const VertexElement VertexElements[1];
	static const int ElementCount;
	static const int SizeInBytes;
	Vector3 Position;

public:
	VertexPosition(const Vector3& position);
};

}

#endif
