/*=============================================================================
FSMObject.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_AI_FSMOBJECT_H_
#define _SE_AI_FSMOBJECT_H_

#include "Core/Core.h"
#include "AI/Common.h"

namespace SonataEngine
{
	namespace AI
	{
		class FSM;

		/** AI FSM Object.
			Base class for the AI objects owned by a machine.
		*/
		class FSMObject : public RefObject
		{
		public:
			FSMObject();
			FSMObject(AI::FSM* machine);
			virtual ~FSMObject();

			AI::FSM* GetMachine() const { return _Machine; }
			void SetMachine(AI::FSM* value) { _Machine = value; }

		protected:
			AI::FSM* _Machine;
		};

		typedef SmartPtr<FSMObject> FSMObjectPtr;
	}
}

#endif 
