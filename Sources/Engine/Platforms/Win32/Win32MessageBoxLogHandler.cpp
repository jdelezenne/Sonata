/*=============================================================================
Win32MessageBoxLogHandler.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "Win32MessageBoxLogHandler.h"

namespace SonataEngine
{

Win32MessageBoxLogHandler::Win32MessageBoxLogHandler() :
	LogHandler(),
	_IconFlag(0)
{
}

Win32MessageBoxLogHandler::~Win32MessageBoxLogHandler()
{
	Close();
}

void Win32MessageBoxLogHandler::Close()
{
}

void Win32MessageBoxLogHandler::Log(const LogRecord& record)
{
	_IconFlag = GetIconFlag(record.GetLevel());

	LogHandler::Log(record);

	_IconFlag = 0;
}

void Win32MessageBoxLogHandler::Write(const String& message)
{
	// Display a Windows message box.
	MessageBox(NULL, message.Data(), _Title.Data(), MB_OK | _IconFlag);
}

UINT Win32MessageBoxLogHandler::GetIconFlag(const LogLevel& level)
{
	if (level.GetValue() == LogLevel::Fatal.GetValue() ||
		level.GetValue() == LogLevel::Critical.GetValue() ||
		level.GetValue() == LogLevel::Error.GetValue())
	{
		return MB_ICONERROR;
	}
	else if (level.GetValue() == LogLevel::Warning.GetValue())
	{
		return MB_ICONWARNING;
	}

	return MB_ICONINFORMATION;
}

}
