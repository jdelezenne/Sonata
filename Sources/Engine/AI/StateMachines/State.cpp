/*=============================================================================
State.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "State.h"
#include "AI/StateMachines/FSM.h"

namespace SonataEngine
{
	namespace AI
	{
		State::State() :
			FSMObject()
		{
		}

		State::~State()
		{
		}

		bool State::IsStartState() const
		{
			return (GetMachine() != NULL && GetMachine()->GetStartState().Get() == this);
		}

		void State::SetStartState(bool value)
		{
			if (GetMachine() != NULL)
				GetMachine()->SetStartState(this);
		}

		bool State::IsEndState() const
		{
			return (GetMachine() != NULL);
		}

		void State::SetEndState(bool value)
		{
		}
	}
}
