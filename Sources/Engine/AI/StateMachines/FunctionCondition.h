/*=============================================================================
FunctionCondition.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_AI_FUNCTIONCONDITION_H_
#define _SE_AI_FUNCTIONCONDITION_H_

#include "AI/StateMachines/Condition.h"

namespace SonataEngine
{
	namespace AI
	{
		class IConditionSlot
		{
		public:
			virtual bool Check() = 0;
		};

		class ConditionFunctorSlot : public IConditionSlot
		{
		public:
			typedef bool (*FunctorDelegate) ();

			SE_INLINE ConditionFunctorSlot(FunctorDelegate delegate)
			{
				Delegate = delegate;
			}

			virtual SE_INLINE bool Check()
			{
				return (Delegate)();
			}

			FunctorDelegate Delegate;
		};

		template <class T>
		class ConditionMethodSlot : public IConditionSlot
		{
		public:
			typedef bool (T::*MethodDelegate)();

			SE_INLINE ConditionMethodSlot(T* slotClass, bool (T::*delegate)())
			{
				SlotClass = slotClass;
				Delegate = delegate;
			}

			virtual SE_INLINE bool Check()
			{
				return (SlotClass->*Delegate)();
			}

			T* SlotClass;
			MethodDelegate Delegate;
		};

		/** AI Function Condition.
			Condition value from a symbol.
		*/
		class FunctionCondition : public Condition
		{
		public:
			FunctionCondition();
			FunctionCondition(IConditionSlot* slot);
			virtual ~FunctionCondition();

			virtual bool Check() const;

			IConditionSlot* Slot;
		};
	}
}

#endif 
