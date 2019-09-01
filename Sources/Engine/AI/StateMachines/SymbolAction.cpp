/*=============================================================================
SymbolAction.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "SymbolAction.h"
#include "AI/StateMachines/FSM.h"

namespace SonataEngine
{
	namespace AI
	{
		SymbolAction::SymbolAction() :
			Action()
		{
		}

		SymbolAction::~SymbolAction()
		{
		}

		void SymbolAction::Execute() const
		{
			if (GetMachine() == NULL || GetMachine()->GetSymbolTable() == NULL)
				return;

			return GetMachine()->GetSymbolTable()->SetSymbol(Symbol, Value);
		}
	}
}
