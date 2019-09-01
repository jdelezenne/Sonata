/*=============================================================================
IRenderable.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_IRENDERABLE_H_
#define _SE_IRENDERABLE_H_

#include "Core/Common.h"

namespace SonataEngine
{

/** Interface for the renderable objects. */
class SE_CORE_EXPORT IRenderable
{
public:
	/// Renders this object.
	virtual void Render() = 0;
};

}

#endif 
