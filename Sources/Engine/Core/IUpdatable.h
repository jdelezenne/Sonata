/*=============================================================================
IUpdatable.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_IUPDATABLE_H_
#define _SE_IUPDATABLE_H_

#include "Core/Common.h"

namespace SonataEngine
{

/** Interface for the updatable objects. */
class SE_CORE_EXPORT IUpdatable
{
public:
	/// Updates this object.
	virtual void Update(real64 elapsed) = 0;
};

}

#endif 
