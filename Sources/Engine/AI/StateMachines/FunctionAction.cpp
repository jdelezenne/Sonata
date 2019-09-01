/*=============================================================================
FunctionAction.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "FunctionAction.h"

namespace SonataEngine
{
	namespace AI
	{
		FunctionAction::FunctionAction() :
			Action()
		{
		}

		FunctionAction::FunctionAction(IActionSlot* slot) :
			Action(),
			Slot(slot)
		{
		}

		FunctionAction::~FunctionAction()
		{
		}

		void FunctionAction::Execute() const
		{
			if (Slot == NULL)
				return;

			Slot->Execute();
		}
	}
}
