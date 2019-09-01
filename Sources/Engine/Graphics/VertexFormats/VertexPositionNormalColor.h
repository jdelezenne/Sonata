/*=============================================================================
VertexPositionNormalColor.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_VERTEXPOSITIONNORMALCOLOR_H_
#define _SE_VERTEXPOSITIONNORMALCOLOR_H_

#include "Graphics/Common.h"
#include "Graphics/Model/VertexElement.h"

namespace SonataEngine
{

/**
	@brief VertexPositionNormalColor.
*/
struct SE_GRAPHICS_EXPORT VertexPositionNormalColor
{
public:
	static const VertexElement VertexElements[3];
	static const int ElementCount;
	static const int SizeInBytes;
	Vector3 Position;
	Vector3 Normal;
	uint32 Color;

public:
	VertexPositionNormalColor(const Vector3& position, const Vector3& normal, const Color32& color);
};

}

#endif
