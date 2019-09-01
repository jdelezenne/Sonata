/*=============================================================================
Face.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_FACE_H_
#define _SE_FACE_H_

#include "Core/Core.h"
#include "Graphics/Common.h"

namespace SonataEngine
{

/** Base class for the faces. */
class SE_GRAPHICS_EXPORT Face
{
public:
	virtual ~Face();

protected:
	Face();
};

}

#endif 
