/*=============================================================================
Win32ConsoleStream.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "Core/IO/ConsoleStream.h"
#include "Core/Exception/ArgumentNullException.h"
#include "Core/Exception/ArgumentOutOfRangeException.h"
#include "Win32Platform.h"

namespace SonataEngine
{

typedef ConsoleStream Win32ConsoleStream;

Win32ConsoleStream* Win32ConsoleStream::StandardInput = new Win32ConsoleStream(StdConsoleType_Input);
Win32ConsoleStream* Win32ConsoleStream::StandardOutput = new Win32ConsoleStream(StdConsoleType_Output);
Win32ConsoleStream* Win32ConsoleStream::StandardError = new Win32ConsoleStream(StdConsoleType_Error);

Win32ConsoleStream::ConsoleStream() :
	Stream()
{
	_ConsoleType = (StdConsoleType)0;
	_handle = INVALID_HANDLE_VALUE;
}

Win32ConsoleStream::ConsoleStream(StdConsoleType type) :
	Stream()
{
	_ConsoleType = type;
	if (type == StdConsoleType_Input)
		_handle = ::GetStdHandle(STD_INPUT_HANDLE);
	else if (type == StdConsoleType_Output)
		_handle = ::GetStdHandle(STD_OUTPUT_HANDLE);
	else if (type == StdConsoleType_Error)
		_handle = ::GetStdHandle(STD_ERROR_HANDLE);
	else
		_handle = INVALID_HANDLE_VALUE;
}

Win32ConsoleStream::~ConsoleStream()
{
}

bool Win32ConsoleStream::CanRead() const
{
	if ((HANDLE)_handle == INVALID_HANDLE_VALUE)
		return false;

	return (_ConsoleType == StdConsoleType_Input);
}

bool Win32ConsoleStream::CanWrite() const
{
	if ((HANDLE)_handle == INVALID_HANDLE_VALUE)
		return false;

	return ((_ConsoleType == StdConsoleType_Output) || (_ConsoleType == StdConsoleType_Error));
}

bool Win32ConsoleStream::CanSeek() const
{
	return false;
}

int32 Win32ConsoleStream::GetLength() const
{
	return 0;
}

void Win32ConsoleStream::SetLength(int32 value)
{
}

int32 Win32ConsoleStream::GetPosition() const
{
	return 0;
}

void Win32ConsoleStream::SetPosition(int32 value)
{
}

void Win32ConsoleStream::Close()
{
	if ((HANDLE)_handle != INVALID_HANDLE_VALUE)
	{
		_ConsoleType = (StdConsoleType)0;
		_handle = INVALID_HANDLE_VALUE;
	}
}

void Win32ConsoleStream::Flush()
{
}

int32 Win32ConsoleStream::Seek(int32 offset, SeekOrigin origin)
{
	return 0;
}

bool Win32ConsoleStream::IsEOF() const
{
	return false;
}

byte Win32ConsoleStream::ReadByte()
{
	byte value;
	return (Read(&value, 1) == -1 ? -1 : value);
}

int32 Win32ConsoleStream::Read(byte* buffer, int32 count)
{
	if (buffer == NULL)
	{
		throw ArgumentNullException("buffer");
	}

	if (count < 0)
	{
		throw ArgumentOutOfRangeException("count");
	}

	if ((HANDLE)_handle == INVALID_HANDLE_VALUE)
	{
		return -1;
	}

	if (!CanRead())
	{
		return -1;
	}

	DWORD dwNumberOfBytesRead;
	if (::ReadFile((HANDLE)_handle, buffer, count, &dwNumberOfBytesRead, NULL) == FALSE)
	{
		return -1;
	}

	return dwNumberOfBytesRead;
}

void Win32ConsoleStream::WriteByte(byte value)
{
	Write(&value, 1);
}

int32 Win32ConsoleStream::Write(const byte* buffer, int32 count)
{
	if (buffer == NULL)
	{
		throw ArgumentNullException("buffer");
	}

	if (count < 0)
	{
		throw ArgumentOutOfRangeException("count");
	}

	if ((HANDLE)_handle == INVALID_HANDLE_VALUE)
	{
		return -1;
	}

	if (!CanWrite())
	{
		return -1;
	}

	DWORD dwNumberOfBytesWritten;
	if (::WriteFile((HANDLE)_handle, buffer, count, &dwNumberOfBytesWritten, NULL) == FALSE)
	{
		return -1;
	}

	return dwNumberOfBytesWritten;
}

}
