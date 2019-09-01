/*=============================================================================
MessageHandler.h
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#ifndef _SFW_MESSAGEHANDLER_H_
#define _SFW_MESSAGEHANDLER_H_

#include "SampleFramework/Common.h"
#include "SampleFramework/Messaging/Message.h"

namespace SampleFramework
{
	/**
		@brief Base class for message handler.
		A game is a collection of inter-communicating entities.
		A message handler can receive messages from other entities.
	*/
	class SE_SAMPLEFRAMEWORK_EXPORT MessageHandler
	{
	public:
		/** @name Constructors / Destructor. */
		//@{
		/** Constructor. */
		MessageHandler();

		/** Destructor. */
		virtual ~MessageHandler();
		//@}

		virtual void HandleMessage(Message& msg) = 0;

		void SendMessage(Message& msg, MessageHandler* handler);

	protected:
	};
}

#endif

/*=============================================================================
09-02-2006 Julien Delezenne
	Created.
=============================================================================*/
