/*=============================================================================
Win32ConsoleLogHandler.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "Win32ConsoleLogHandler.h"
#include "Core/Exception/Exception.h"

namespace SonataEngine
{

Win32ConsoleLogHandler::Win32ConsoleLogHandler() :
	LogHandler(),
	_Row(0),
	_Colored(true)
{
	// Retrieves a handle for the standard handler device
	_hConsoleOutput = ::GetStdHandle(STD_OUTPUT_HANDLE); 
	if (_hConsoleOutput == INVALID_HANDLE_VALUE) 
	{
		throw Exception("Can't retrieve the standard handler device");
	}

	// There's no console
	else if (_hConsoleOutput == NULL) 
	{
		// Allocate a new console.
		if (::AllocConsole() != 0)
			throw Exception("Can't allocate a new console.");

		_hConsoleOutput = ::GetStdHandle(STD_OUTPUT_HANDLE); 
	}
}

Win32ConsoleLogHandler::~Win32ConsoleLogHandler()
{
	Close();
}

void Win32ConsoleLogHandler::Close()
{
	if (_hConsoleOutput == NULL)
		return;

	FreeConsole();
	_hConsoleOutput = NULL;
}

void Win32ConsoleLogHandler::Log(const LogRecord& record)
{
	if (_Formatter != NULL)
	{
		if (_Filter != NULL)
		{
			if (!_Filter->CanLog(record))
				return;
		}

		String s = _Formatter->Format(record);

		// Set the console attributes. 
		if (_Colored)
		{
			SetConsoleAttributes(record.GetLevel(), s.Length());
		}

		WriteLine(s);
	}
}

void Win32ConsoleLogHandler::Write(const String& message)
{
	if (_hConsoleOutput == NULL)
		return;

	COORD dwWriteCoord = { 0, _Row++ }; 
	DWORD dwWritten;

	// Write a string of characters to a screen buffer.
	if (::WriteConsoleOutputCharacter(_hConsoleOutput, message.Data(), message.Length(), 
		dwWriteCoord, &dwWritten) == 0)
	{
		throw Exception("Can't write to the console handler.");
	}
}

void Win32ConsoleLogHandler::WriteLine(const String& message)
{
	Write(message);
}

void Win32ConsoleLogHandler::SetConsoleAttributes(const LogLevel& level, int32 length) const
{
	WORD wColor = 0;
	WORD* wAttributes;
	COORD dwWriteCoord = { 0, _Row }; 
	DWORD dwWritten;

	wAttributes = new WORD[length];

	if (level.GetValue() == LogLevel::Fatal.GetValue())
		wColor = FOREGROUND_RED | FOREGROUND_INTENSITY;
	else if (level.GetValue() == LogLevel::Critical.GetValue())
		wColor = FOREGROUND_RED | FOREGROUND_INTENSITY;
	else if (level.GetValue() == LogLevel::Error.GetValue())
		wColor = FOREGROUND_RED;
	else if (level.GetValue() == LogLevel::Warning.GetValue())
		wColor = FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY;
	else if (level.GetValue() == LogLevel::Notice.GetValue())
		wColor = FOREGROUND_GREEN | FOREGROUND_INTENSITY;
	else if (level.GetValue() == LogLevel::Information.GetValue())
		wColor = FOREGROUND_BLUE | FOREGROUND_INTENSITY;
	else if (level.GetValue() == LogLevel::Debug.GetValue())
		wColor = FOREGROUND_GREEN | FOREGROUND_INTENSITY | BACKGROUND_BLUE | BACKGROUND_INTENSITY;
	else
		wColor = FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED;

	Memory::Copy(wAttributes, (void*)wColor, sizeof(WORD)*length);

	// Set the console attributes. 
	if (::WriteConsoleOutputAttribute(_hConsoleOutput, wAttributes, length, dwWriteCoord, &dwWritten) == 0)
	{
		delete wAttributes;
	}

	delete wAttributes;
}

}
