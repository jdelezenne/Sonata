/*=============================================================================
NullConsoleStream.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "Core/IO/ConsoleStream.h"
#include "Core/Exception/ArgumentNullException.h"
#include "Core/Exception/ArgumentOutOfRangeException.h"

namespace SonataEngine
{

typedef ConsoleStream NullConsoleStream;

NullConsoleStream* NullConsoleStream::StandardInput = new NullConsoleStream();
NullConsoleStream* NullConsoleStream::StandardOutput = new NullConsoleStream();
NullConsoleStream* NullConsoleStream::StandardError = new NullConsoleStream();

NullConsoleStream::ConsoleStream() :
	Stream()
{
	_ConsoleType = (StdConsoleType)0;
	_handle = NULL;
}

NullConsoleStream::ConsoleStream(StdConsoleType type) :
	Stream()
{
	_ConsoleType = type;
	_handle = NULL;
}

NullConsoleStream::~ConsoleStream()
{
}

bool NullConsoleStream::CanRead() const
{
	return false;
}

bool NullConsoleStream::CanWrite() const
{
	return false;
}

bool NullConsoleStream::CanSeek() const
{
	return false;
}

int32 NullConsoleStream::GetLength() const
{
	return 0;
}

void NullConsoleStream::SetLength(int32 value)
{
}

int32 NullConsoleStream::GetPosition() const
{
	return 0;
}

void NullConsoleStream::SetPosition(int32 value)
{
}

void NullConsoleStream::Close()
{
	_ConsoleType = (StdConsoleType)0;
	_handle = NULL;
}

void NullConsoleStream::Flush()
{
}

int32 NullConsoleStream::Seek(int32 offset, SeekOrigin origin)
{
	return 0;
}

bool NullConsoleStream::IsEOF() const
{
	return true;
}

byte NullConsoleStream::ReadByte()
{
	return 0;
}

int32 NullConsoleStream::Read(byte* buffer, int32 count)
{
	if (buffer == NULL)
	{
		throw ArgumentNullException("buffer");
	}

	if (count < 0)
	{
		throw ArgumentOutOfRangeException("count");
	}

	if (!CanRead())
	{
		return -1;
	}

	return 0;
}

void NullConsoleStream::WriteByte(byte value)
{
	Write(&value, 1);
}

int32 NullConsoleStream::Write(const byte* buffer, int32 count)
{
	if (buffer == NULL)
	{
		throw ArgumentNullException("buffer");
	}

	if (count < 0)
	{
		throw ArgumentOutOfRangeException("count");
	}

	if (!CanWrite())
	{
		return -1;
	}

	return 0;
}

}
