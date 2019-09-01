/*=============================================================================
FSM.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "FSM.h"
#include "AI/StateMachines/State.h"
#include "AI/StateMachines/Transition.h"

namespace SonataEngine
{
	namespace AI
	{
		FSM::FSM() :
			RefObject(),
			_StartState(NULL),
			_currentState(NULL),
			_SymbolTable(NULL)
		{
		}

		FSM::~FSM()
		{
		}

		void FSM::SetCurrentState(StatePtr value)
		{
			if (_currentState != NULL)
			{
				Action* action;
				foreach (action, _currentState->ExitActions, State::ActionList)
				{
					action->Execute();
				}
			}

			_currentState = value;

			if (_currentState != NULL)
			{
				Action* action;
				foreach (action, _currentState->EnterActions, State::ActionList)
				{
					action->Execute();
				}
			}
		}

		void FSM::Update()
		{
			Transition* transition;
			foreach (transition, Transitions, TransitionList)
			{
				if (transition->GetStartState() != GetCurrentState())
					continue;

				Condition* condition;
				foreach (condition, transition->Conditions, Transition::ConditionList)
				{
					if (condition->Check())
					{
						//TODO: build checked list
						SetCurrentState(transition->GetEndState());
						return;
					}
				}
			}
		}
	}
}
