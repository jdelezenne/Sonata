/*=============================================================================
Logger.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "Logger.h"

namespace SonataEngine
{

Logger::Logger():
	Manager(),
	_Filter(new DefaultLogFilter())
{
}

Logger::~Logger()
{
}

Logger::LogHandlerList& Logger::GetHandlers()
{
	return _handlers;
}

bool Logger::Create()
{
	return true;
}

void Logger::Destroy()
{
}

void Logger::Update(real64 elapsed)
{
}

void Logger::Log(const String& message)
{
	Log(LogLevel::None, String::Empty, message);
}

void Logger::Log(const String& source, const String& message)
{
	Log(LogLevel::None, message, source);
}

void Logger::Log(LogLevel level, const String& message)
{
	Log(level, String::Empty, message);
}

void Logger::Log(LogLevel level, const String& source, const String& message)
{
	if (_handlers.Count() == 0)
		return;

	LogRecord record(level, message, source);
	record.SetDateTime(DateTime::GetNow());

	if (_Filter != NULL)
	{
		if (!_Filter->CanLog(record))
			return;
	}

	LogHandlerList::Iterator it = _handlers.GetIterator();
	while (it.Next())
	{
		it.Current()->Log(record);
	}
}

void Logger::Assert(bool condition, const String& message)
{
	if (condition)
	{
		Log(LogLevel::Error, message);
	}
}

}
