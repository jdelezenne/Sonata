/*=============================================================================
StdConsoleStream.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "Core/IO/ConsoleStream.h"
#include "Core/Exception/ArgumentNullException.h"
#include "Core/Exception/ArgumentOutOfRangeException.h"

#include <stdlib.h>
#include <stdio.h>

namespace SonataEngine
{

typedef ConsoleStream StdConsoleStream;

StdConsoleStream* StdConsoleStream::StandardInput = new StdConsoleStream(StdConsoleType_Input);
StdConsoleStream* StdConsoleStream::StandardOutput = new StdConsoleStream(StdConsoleType_Output);
StdConsoleStream* StdConsoleStream::StandardError = new StdConsoleStream(StdConsoleType_Error);

StdConsoleStream::ConsoleStream() :
	Stream()
{
	_ConsoleType = (StdConsoleType)0;
	_handle = NULL;
}

StdConsoleStream::ConsoleStream(StdConsoleType type) :
	Stream()
{
	_ConsoleType = type;
	if (type == StdConsoleType_Input)
		_handle = stdin;
	else if (type == StdConsoleType_Output)
		_handle = stdout;
	else if (type == StdConsoleType_Error)
		_handle = stderr;
	else
		_handle = NULL;
}

StdConsoleStream::~ConsoleStream()
{
}

bool StdConsoleStream::CanRead() const
{
	if (_handle == NULL)
		return false;

	return (_ConsoleType == StdConsoleType_Input);
}

bool StdConsoleStream::CanWrite() const
{
	if (_handle == NULL)
		return false;

	return ((_ConsoleType == StdConsoleType_Output) || (_ConsoleType == StdConsoleType_Error));
}

bool StdConsoleStream::CanSeek() const
{
	return false;
}

int32 StdConsoleStream::GetLength() const
{
	return 0;
}

void StdConsoleStream::SetLength(int32 value)
{
}

int32 StdConsoleStream::GetPosition() const
{
	return 0;
}

void StdConsoleStream::SetPosition(int32 value)
{
}

void StdConsoleStream::Close()
{
	if (_handle != NULL)
	{
		_ConsoleType = (StdConsoleType)0;
		_handle = NULL;
	}
}

void StdConsoleStream::Flush()
{
}

int32 StdConsoleStream::Seek(int32 offset, SeekOrigin origin)
{
	return 0;
}

bool StdConsoleStream::IsEOF() const
{
	return false;
}

SEbyte StdConsoleStream::ReadByte()
{
	SEbyte value;
	return (Read(&value, 1) == -1 ? -1 : value);
}

int32 StdConsoleStream::Read(SEbyte* buffer, int32 count)
{
	if (buffer == NULL)
	{
		throw ArgumentNullException("buffer");
	}

	if (count < 0)
	{
		throw ArgumentOutOfRangeException("count");
	}

	if (_handle == NULL)
	{
		return -1;
	}

	if (!CanRead())
	{
		return -1;
	}

	size_t nRead;
	if ((nRead = fread(buffer, count, 1, (FILE*)_handle)) == 0)
	{
		return -1;
	}
	//FIX
	SEbyte* crBuffer[1];
	fread(crBuffer, 1, 1, (FILE*)_handle);

	return (int32)nRead;
}

void StdConsoleStream::WriteByte(SEbyte value)
{
	Write(&value, 1);
}

int32 StdConsoleStream::Write(const SEbyte* buffer, int32 count)
{
	if (buffer == NULL)
	{
		throw ArgumentNullException("buffer");
	}

	if (count < 0)
	{
		throw ArgumentOutOfRangeException("count");
	}

	if (_handle == NULL)
	{
		return -1;
	}

	if (!CanWrite())
	{
		return -1;
	}

	size_t nWritten;
	if ((nWritten = fwrite(buffer, count, 1, (FILE*)_handle)) == 0)
	{
		return -1;
	}

	return (int32)nWritten;
}

}
