/*=============================================================================
Transition.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "Transition.h"

namespace SonataEngine
{
	namespace AI
	{
		Transition::Transition() :
			FSMObject(),
			_StartState(NULL),
			_EndState(NULL)
		{
		}

		Transition::~Transition()
		{
		}
	}
}
