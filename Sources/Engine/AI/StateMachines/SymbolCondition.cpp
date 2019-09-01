/*=============================================================================
SymbolCondition.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "SymbolCondition.h"
#include "AI/StateMachines/FSM.h"

namespace SonataEngine
{
	namespace AI
	{
		SymbolCondition::SymbolCondition() :
			Condition()
		{
		}

		SymbolCondition::SymbolCondition(const String& symbol, bool value) :
			Condition(),
			Symbol(symbol),
			Value(value)
		{
		}

		SymbolCondition::~SymbolCondition()
		{
		}

		bool SymbolCondition::Check() const
		{
			if (GetMachine() == NULL || GetMachine()->GetSymbolTable() == NULL)
				return false;

			return GetMachine()->GetSymbolTable()->GetSymbol(Symbol) == Value;
		}
	}
}
