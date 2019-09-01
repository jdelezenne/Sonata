/*=============================================================================
EffectFunction.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_EFFECTFUNCTION_H_
#define _SE_EFFECTFUNCTION_H_

#include "Graphics/Common.h"

namespace SonataEngine
{

/**
	@brief Effect Shader Function.
*/
class SE_GRAPHICS_EXPORT EffectFunction : public NamedObject
{
public:
	/** @name Constructors / Destructor. */
	//@{
	/** Constructor. */
	EffectFunction();

	/** Destructor. */
	virtual ~EffectFunction();
	//@}
};

}

#endif
