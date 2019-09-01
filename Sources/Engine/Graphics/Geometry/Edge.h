/*=============================================================================
Edge.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_EDGE_H_
#define _SE_EDGE_H_

#include "Core/Core.h"
#include "Graphics/Common.h"

namespace SonataEngine
{

/** Edge. */
class SE_GRAPHICS_EXPORT Edge
{
public:
	Edge();
	virtual ~Edge();

public:
	Vector3 v0;
	Vector3 v1;
};

}

#endif 
