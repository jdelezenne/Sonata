/*=============================================================================
IFormattable.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_IFORMATTABLE_H_
#define _SE_IFORMATTABLE_H_

#include "Core/Common.h"
#include "Core/String.h"

namespace SonataEngine
{

/** Interface for the formattable objects. */
class IFormattable
{
public:
	/// Formats the value of the current instance.
	virtual String ToString() = 0;
};

}

#endif 
