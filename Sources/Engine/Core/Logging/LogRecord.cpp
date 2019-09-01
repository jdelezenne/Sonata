/*=============================================================================
LogRecord.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "LogRecord.h"

namespace SonataEngine
{

LogRecord::LogRecord() :
	_Level(LogLevel::None)
{
}

LogRecord::LogRecord(const LogLevel& level, const String& message, const String& source) :
	_Level(level),
	_Message(message),
	_Source(source)
{
}

LogRecord::LogRecord(const LogRecord& value) :
	_Level(value.GetLevel()),
	_Message(value.GetMessage()),
	_Source(value.GetSource())
{
}

}
