/*=============================================================================
Condition.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_AI_CONDITION_H_
#define _SE_AI_CONDITION_H_

#include "AI/Common.h"
#include "AI/StateMachines/FSMObject.h"

namespace SonataEngine
{
	namespace AI
	{
		/**
			AI Condition.
			Base class for conditions.
		*/
		class Condition : public FSMObject
		{
		public:
			virtual ~Condition();

			virtual bool Check() const = 0;

		protected:
			Condition();
		};

		typedef SmartPtr<Condition> ConditionPtr;
	}
}

#endif 
