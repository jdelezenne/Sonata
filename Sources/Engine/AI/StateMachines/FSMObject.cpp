/*=============================================================================
FSMObject.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "FSMObject.h"

namespace SonataEngine
{
	namespace AI
	{
		FSMObject::FSMObject() :
			RefObject(),
			_Machine(NULL)
		{
		}

		FSMObject::FSMObject(AI::FSM* machine) :
			RefObject(),
			_Machine(machine)
		{
		}

		FSMObject::~FSMObject()
		{
		}
	}
}
