/*=============================================================================
MessageHandler.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "MessageHandler.h"

namespace SampleFramework
{

MessageHandler::MessageHandler()
{
}

MessageHandler::~MessageHandler()
{
}

void MessageHandler::SendMessage(Message& msg, MessageHandler* handler)
{
	if (handler == NULL)
		return;

	msg.SetSource(EntityID_Unknown);
	handler->HandleMessage(msg);
}

}
