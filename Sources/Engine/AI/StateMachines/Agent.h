/*=============================================================================
Agent.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SE_AI_AGENT_H_
#define _SE_AI_AGENT_H_

#include "AI/Common.h"
#include "Core/Object.h"
#include "AI/StateMachines/FSM.h"

namespace SonataEngine
{
	namespace AI
	{
		/** AI Agent.
			Base class for the AI agents.
		*/
		class Agent : public RefObject
		{
		public:
			Agent();
			virtual ~Agent();

			virtual void Update(real64 elapsed);

			FSM* GetFSM() const { return _FSM; }
			void SetFSM(FSM* value) { _FSM = value; }

		protected:
			FSM* _FSM;
		};
	}
}

#endif 
