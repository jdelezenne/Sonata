/*=============================================================================
Engine.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_ENGINE_H_
#define _SE_ENGINE_H_

#include "Core/Core.h"

namespace SonataEngine
{

/** Root class. */
class SE_CORE_EXPORT Engine : public Singleton<Engine>
{
public:
	/** @name Constructors / Destructor. */
	//@{
	Engine();
	virtual ~Engine();
	//@}
};

}

#endif
