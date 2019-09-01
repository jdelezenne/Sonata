/*=============================================================================
LogFormatter.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "LogFormatter.h"
#include "Core/System/Environment.h"

namespace SonataEngine
{

DefaultLogFormatter::DefaultLogFormatter() :
	_Options(LogOptions_None)
{
}

DefaultLogFormatter::~DefaultLogFormatter()
{
}

String DefaultLogFormatter::Format(const LogRecord& record) const
{
	String message;

	if (_Options == LogOptions_None)
	{
		message = record.GetMessage();
	}
	else
	{
		if ((_Options & LogOptions_Level) != 0 && !record.GetSource().IsEmpty())
		{
			message += _T("[") + record.GetLevel().GetName() + _T("] ");
		}

		if ((_Options & LogOptions_Date) != 0 && (_Options & LogOptions_Time) != 0)
		{
			message += record.GetDateTime().ToString() + _T(" : ");
		}
		else
		{
			if ((_Options & LogOptions_Date) != 0)
			{
				message += record.GetDateTime().DateToString() + _T(" : ");
			}
			else if ((_Options & LogOptions_Time) != 0)
			{
				message += record.GetDateTime().TimeToString() + _T(" : ");
			}
		}

		if ((_Options & LogOptions_Source) != 0 && !record.GetSource().IsEmpty())
		{
			message += _T("<") + record.GetSource() + _T("> ");
		}

		message += record.GetMessage();
	}

	return message;
}

}
