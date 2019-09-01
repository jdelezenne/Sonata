/*=============================================================================
AABB.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_AABB_H_
#define _SE_AABB_H_

#include "Core/Math/Math.h"
#include "Core/Math/BoundingBox.h"

namespace SonataEngine
{

/**
	@brief Axis Aligned Bounding Box (AABB).

	An AABB is a bounding volume defined by a minimum vector and a maximum vector.
*/
typedef BoundingBox AABB;

}

#endif 
