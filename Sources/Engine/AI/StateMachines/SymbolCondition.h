/*=============================================================================
SymbolCondition.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_AI_SYMBOLCONDITION_H_
#define _SE_AI_SYMBOLCONDITION_H_

#include "AI/StateMachines/Condition.h"

namespace SonataEngine
{
	namespace AI
	{
		/** AI Symbol Condition.
			Condition value from a symbol.
		*/
		class SymbolCondition : public Condition
		{
		public:
			SymbolCondition();
			SymbolCondition(const String& symbol, bool value = true);
			virtual ~SymbolCondition();

			virtual bool Check() const;

			String Symbol;
			bool Value;
		};
	}
}

#endif 
