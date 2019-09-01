/*=============================================================================
SymbolAction.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_AI_SYMBOLACTION_H_
#define _SE_AI_SYMBOLACTION_H_

#include "AI/Common.h"
#include "AI/StateMachines/Action.h"

namespace SonataEngine
{
	namespace AI
	{
		/** AI Symbol Action.
			Action that sets the value of a symbol.
		*/
		class SymbolAction : public Action
		{
		public:
			SymbolAction();
			virtual ~SymbolAction();

			virtual void Execute() const;

			String Symbol;
			bool Value;
		};
	}
}

#endif 
