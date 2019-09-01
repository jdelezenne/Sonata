/*=============================================================================
FunctionAction.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_AI_FUNCTIONACTION_H_
#define _SE_AI_FUNCTIONACTION_H_

#include "AI/StateMachines/Action.h"

namespace SonataEngine
{
	namespace AI
	{
		class IActionSlot
		{
		public:
			virtual void Execute() = 0;
		};

		class ActionFunctorSlot : public IActionSlot
		{
		public:
			typedef void (*FunctorDelegate) ();

			SE_INLINE ActionFunctorSlot(FunctorDelegate delegate)
			{
				Delegate = delegate;
			}

			virtual SE_INLINE void Execute()
			{
				(Delegate)();
			}

			FunctorDelegate Delegate;
		};

		template <class T>
		class ActionMethodSlot : public IActionSlot
		{
		public:
			typedef void (T::*MethodDelegate)();

			SE_INLINE ActionMethodSlot(T* slotClass, void (T::*delegate)())
			{
				SlotClass = slotClass;
				Delegate = delegate;
			}

			virtual SE_INLINE void Execute()
			{
				(SlotClass->*Delegate)();
			}

			T* SlotClass;
			MethodDelegate Delegate;
		};

		/** AI Function Action.
			Action value from a symbol.
		*/
		class FunctionAction : public Action
		{
		public:
			FunctionAction();
			FunctionAction(IActionSlot* slot);
			virtual ~FunctionAction();

			virtual void Execute() const;

			IActionSlot* Slot;
		};
	}
}

#endif 
