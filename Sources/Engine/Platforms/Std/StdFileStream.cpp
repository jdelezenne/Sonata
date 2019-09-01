/*=============================================================================
StdFileStream.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "Core/IO/FileStream.h"
#include "Core/IO/File.h"
#include "Core/Exception/ArgumentNullException.h"

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>

namespace SonataEngine
{

typedef FileStream StdFileStream;

StdFileStream::FileStream()
{
	Init();
}

StdFileStream::FileStream(File* file) :
	Stream()
{
	SE_ASSERT(file);
	if (file == NULL)
	{
		throw new ArgumentNullException("file");
		return;
	}

	Init();

	_file = file;
}

StdFileStream::~FileStream()
{
}

void StdFileStream::Init()
{
	_bufferSize = FileStream_BufferSize;
	_file = NULL;
}

int32 StdFileStream::GetLength() const
{
	SE_ASSERT(_file);
	if (_file == NULL)
		return 0;

	return _file->GetLength();
}

int32 StdFileStream::GetPosition() const
{
	SE_ASSERT(_file);
	if (_file == NULL)
		return 0;

	FILE* fp = (FILE*)_file->GetHandle();
	return ftell(fp);
}

void StdFileStream::SetPosition(int32 value)
{
	Seek(value, SeekOrigin_Begin);
}

void StdFileStream::Close()
{
	SE_ASSERT(_file);
	if (_file == NULL || !_file->IsOpen())
		return;

	_file->Close();
	Init();
}

void StdFileStream::Flush()
{
	SE_ASSERT(_file);
	if (_file == NULL || !_file->IsOpen())
	{
		return;
	}

	if (_file->GetHandle())
	{
		FILE* fp = (FILE*)_file->GetHandle();
		if (fflush(fp) == 0)
		{
			return;
		}
	}
}

int32 StdFileStream::Seek(int32 offset, SeekOrigin origin)
{
	SE_ASSERT(_file);
	if (_file == NULL || !_file->IsOpen())
	{
		return 0;
	}

	if (!CanSeek())
		return GetPosition();

	int stdOrigin;
	if (origin == SeekOrigin_Begin)
		stdOrigin = SEEK_SET;
	else if (origin == SeekOrigin_Current)
		stdOrigin = SEEK_CUR;
	else if (origin == SeekOrigin_End)
		stdOrigin = SEEK_END;
	else
		return 0;

	FILE* fp = (FILE*)_file->GetHandle();
	if (fseek(fp, offset, stdOrigin) == 0)
	{
		return 0;
	}

	return GetPosition();
}

bool StdFileStream::IsEOF() const
{
	return GetPosition() == GetLength();
}

byte StdFileStream::ReadByte()
{
	SE_ASSERT(_file);
	if (_file == NULL || !_file->IsOpen())
	{
		return EOF;
	}

	if (!CanRead())
	{
		return EOF;
	}

	byte value;
	return Read(&value, 1) == EOF ? EOF : value;
}

int32 StdFileStream::Read(byte* buffer, int32 count)
{
	SE_ASSERT(_file);
	if (buffer == NULL)
	{
		return EOF;
	}

	if (_file == NULL || !_file->IsOpen())
	{
		return EOF;
	}

	if (!CanRead())
	{
		return EOF;
	}

	FILE* fp = (FILE*)_file->GetHandle();
	size_t read = fread(buffer, count, 1, fp);
	if ((int)read < count)
	{
		return EOF;
	}

	return (int)read;
}

void StdFileStream::WriteByte(byte value)
{
	SE_ASSERT(_file);
	if (_file == NULL || !_file->IsOpen())
	{
		return;
	}

	if (!CanWrite())
	{
		return;
	}

	Write(&value, 1);
}

int32 StdFileStream::Write(const byte* buffer, int32 count)
{
	SE_ASSERT(buffer && _file);
	if (buffer == NULL)
	{
		return EOF;
	}

	if (_file == NULL || !_file->IsOpen())
	{
		return EOF;
	}

	if (!CanWrite())
	{
		return EOF;
	}

	FILE* fp = (FILE*)_file->GetHandle();
	size_t written = fwrite(buffer, count, 1, fp);
	if ((int)written < count)
	{
		return EOF;
	}

	return (int)written;
}

String StdFileStream::GetName() const
{
	if (_file == NULL)
		return String::Empty;

	return _file->GetName();
}

}
