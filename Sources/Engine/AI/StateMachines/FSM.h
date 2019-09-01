/*=============================================================================
FSM.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_AI_FSM_H_
#define _SE_AI_FSM_H_

#include "Core/Core.h"
#include "AI/Common.h"
#include "AI/StateMachines/State.h"
#include "AI/StateMachines/Transition.h"
#include "AI/StateMachines/SymbolTable.h"

namespace SonataEngine
{
	namespace AI
	{
		/** Finite State Machine.
			An FSM has a starting state and several ending states.
			An FSM has only one current state at a time.
		*/
		class FSM : public RefObject
		{
		public:
			typedef Array<AI::StatePtr> StateList;
			typedef Array<AI::TransitionPtr> TransitionList;

			FSM();
			virtual ~FSM();

			StatePtr GetStartState() const { return _StartState; }
			void SetStartState(StatePtr value) { _StartState = value; }

			StatePtr GetCurrentState() const { return _currentState; }
			void SetCurrentState(StatePtr value);

			SymbolTablePtr GetSymbolTable() const { return _SymbolTable; }
			void SetSymbolTable(SymbolTablePtr value) { _SymbolTable = value; }

			void Update();

		public:
			StateList States;
			TransitionList Transitions;

		protected:
			StatePtr _StartState;
			StatePtr _currentState;
			SymbolTablePtr _SymbolTable;
		};

		typedef SmartPtr<FSM> FSMPtr;
	}
}

#endif 
