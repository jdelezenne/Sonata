/*=============================================================================
State.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_AI_STATE_H_
#define _SE_AI_STATE_H_

#include "AI/Common.h"
#include "AI/StateMachines/FSMObject.h"
#include "AI/StateMachines/Action.h"

namespace SonataEngine
{
	namespace AI
	{
		/**
			AI State.
			Base class for the AI states of the agents.
		*/
		class State : public FSMObject
		{
		public:
			typedef Array<AI::ActionPtr> ActionList;

			State();
			virtual ~State();

			bool IsStartState() const;
			void SetStartState(bool value);

			bool IsEndState() const;
			void SetEndState(bool value);

		public:
			ActionList EnterActions;
			ActionList ExitActions;
			ActionList ExecuteActions;
		};

		typedef SmartPtr<State> StatePtr;
	}
}

#endif 
