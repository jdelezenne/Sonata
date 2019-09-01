/*=============================================================================
Action.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_AI_ACTION_H_
#define _SE_AI_ACTION_H_

#include "AI/Common.h"
#include "AI/StateMachines/FSMObject.h"

namespace SonataEngine
{
	namespace AI
	{
		/** AI Action.
			Abstract class for the AI actions of the states.
		*/
		class Action : public FSMObject
		{
		public:
			virtual ~Action();

			virtual void Execute() const = 0;

		protected:
			Action();
		};

		typedef SmartPtr<Action> ActionPtr;
	}
}

#endif 
