/*=============================================================================
Transition.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_AI_TRANSITION_H_
#define _SE_AI_TRANSITION_H_

#include "AI/Common.h"
#include "AI/StateMachines/FSMObject.h"
#include "AI/StateMachines/State.h"
#include "AI/StateMachines/Condition.h"

namespace SonataEngine
{
	namespace AI
	{
		/** AI Transition. */
		class Transition : public FSMObject
		{
		public:
			typedef Array<AI::ConditionPtr> ConditionList;

			Transition();
			virtual ~Transition();

			StatePtr GetStartState() const { return _StartState; }
			void SetStartState(StatePtr value) { _StartState = value; }

			StatePtr GetEndState() const { return _EndState; }
			void SetEndState(StatePtr value) { _EndState = value; }

			void SetStates(StatePtr start, StatePtr end) { _StartState = start; _EndState = end; }

		public:
			ConditionList Conditions;

		protected:
			StatePtr _StartState;
			StatePtr _EndState;
		};

		typedef SmartPtr<Transition> TransitionPtr;
	}
}

#endif 
