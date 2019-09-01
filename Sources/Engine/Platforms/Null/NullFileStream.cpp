/*=============================================================================
NullFileStream.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "Core/IO/FileStream.h"
#include "Core/IO/File.h"
#include "Core/Exception/ArgumentNullException.h"

namespace SonataEngine
{

typedef FileStream NullFileStream;

NullFileStream::FileStream() :
	Stream()
{
	Init();
}

NullFileStream::FileStream(File* file) :
	Stream()
{
	if (file == NULL)
	{
		throw new ArgumentNullException("file");
		return;
	}

	Init();

	_file = file;
	_SourceName = file->GetName();
}

NullFileStream::~FileStream()
{
}

void NullFileStream::Init()
{
	_bufferSize = FileStream_BufferSize;
	_file = NULL;
}

int32 NullFileStream::GetLength() const
{
	return 0;
}

void NullFileStream::SetLength(int32 value)
{
}

int32 NullFileStream::GetPosition() const
{
	return 0;
}

void NullFileStream::SetPosition(int32 value)
{
}

void NullFileStream::Close()
{
	if (_file == NULL || !_file->IsOpen())
		return;

	_file->Close();
	Init();
}

void NullFileStream::Flush()
{
}

int32 NullFileStream::Seek(int32 offset, SeekOrigin origin)
{
	if (_file == NULL || !_file->IsOpen())
	{
		return 0;
	}

	if (!CanSeek())
		return GetPosition();

	return GetPosition();
}

bool NullFileStream::IsEOF() const
{
	return true;
}

byte NullFileStream::ReadByte()
{
	if (_file == NULL || !_file->IsOpen())
	{
		return -1;
	}

	if (!CanRead())
	{
		return -1;
	}

	return -1;
}

int32 NullFileStream::Read(byte* buffer, int32 count)
{
	if (buffer == NULL)
	{
		return -1;
	}

	if (_file == NULL || !_file->IsOpen())
	{
		return -1;
	}

	if (!CanRead())
	{
		return -1;
	}

	return 0;
}

void NullFileStream::WriteByte(byte value)
{
	if (_file == NULL || !_file->IsOpen())
	{
		return;
	}

	if (!CanWrite())
	{
		return;
	}
}

int32 NullFileStream::Write(const byte* buffer, int32 count)
{
	if (buffer == NULL)
	{
		return -1;
	}

	if (_file == NULL || !_file->IsOpen())
	{
		return -1;
	}

	if (!CanWrite())
	{
		return -1;
	}

	return 0;
}

String NullFileStream::GetFileName() const
{
	if (_file == NULL)
		return String::Empty;

	return _file->GetName();
}

}
