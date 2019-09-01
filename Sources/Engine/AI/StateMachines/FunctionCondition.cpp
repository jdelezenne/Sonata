/*=============================================================================
FunctionCondition.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "FunctionCondition.h"

namespace SonataEngine
{
	namespace AI
	{
		FunctionCondition::FunctionCondition() :
			Condition()
		{
		}

		FunctionCondition::FunctionCondition(IConditionSlot* slot) :
			Condition(),
			Slot(slot)
		{
		}

		FunctionCondition::~FunctionCondition()
		{
		}

		bool FunctionCondition::Check() const
		{
			if (Slot == NULL)
				return false;

			return Slot->Check();
		}
	}
}
