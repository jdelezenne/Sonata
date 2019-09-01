/*=============================================================================
LogHandler.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "LogHandler.h"
#include "Core/System/Environment.h"

namespace SonataEngine
{

LogHandler::LogHandler() :
	RefObject(),
	_Filter(new DefaultLogFilter()),
	_Formatter(new DefaultLogFormatter()),
	_NeedIndent(false),
	_IndentLevel(0),
	_IndentSize(4)
{
}

LogHandler::~LogHandler()
{
	SE_DELETE(_Filter);
	SE_DELETE(_Formatter);
}

void LogHandler::WriteLine(const String& message)
{
	Write(message + Environment::NewLine());
}


void LogHandler::WriteIndent()
{
	if (_NeedIndent)
	{
		for (int i = 0; i < _IndentLevel * _IndentSize; i++)
			Write(Char::Space);

		_NeedIndent = false;
	}
}

void LogHandler::Log(const LogRecord& record)
{
	if (_Formatter != NULL)
	{
		if (_Filter != NULL)
		{
			if (!_Filter->CanLog(record))
				return;
		}

		WriteLine(_Formatter->Format(record));
	}
}

}
