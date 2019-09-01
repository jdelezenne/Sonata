/*=============================================================================
Win32DebugLogHandler.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "Win32DebugLogHandler.h"

namespace SonataEngine
{

Win32DebugLogHandler::Win32DebugLogHandler() :
	LogHandler()
{
}

Win32DebugLogHandler::~Win32DebugLogHandler()
{
	Close();
}

void Win32DebugLogHandler::Close()
{
}

void Win32DebugLogHandler::Write(const String& message)
{
	// Sends the message to the debugger for display.
	::OutputDebugString(message.Data());
}

}
