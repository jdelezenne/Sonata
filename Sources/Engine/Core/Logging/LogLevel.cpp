/*=============================================================================
LogLevel.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "LogLevel.h"

namespace SonataEngine
{

const LogLevel LogLevel::None = LogLevel("None", 0);
const LogLevel LogLevel::Debug = LogLevel("Debug", 1);
const LogLevel LogLevel::Information = LogLevel("Information", 2);
const LogLevel LogLevel::Notice = LogLevel("Notice", 3);
const LogLevel LogLevel::Warning = LogLevel("Warning", 4);
const LogLevel LogLevel::Error = LogLevel("Error", 5);
const LogLevel LogLevel::Critical = LogLevel("Critical", 6);
const LogLevel LogLevel::Fatal = LogLevel("Fatal", 7);

LogLevel::LogLevel(const String& name, int32 value) :
	_name(name),
	_Value(value)
{
}

LogLevel::LogLevel(const LogLevel& value) :
	_name(value.GetName()),
	_Value(value.GetValue())
{
}

}
