/*=============================================================================
TriangleFace.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_TRIANGLEFACE_H_
#define _SE_TRIANGLEFACE_H_

#include "Core/Core.h"
#include "Graphics/Common.h"
#include "Graphics/Geometry/TriangleFace.h"

namespace SonataEngine
{

/** Triangle face. */
class SE_GRAPHICS_EXPORT TriangleFace : public Face
{
public:
	TriangleFace();
	virtual ~TriangleFace();

protected:
	int32[3] _edges;
	int32[3] _vertices;
};

}

#endif 
