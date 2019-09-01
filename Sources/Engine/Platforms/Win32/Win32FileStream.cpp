/*=============================================================================
Win32FileStream.cpp
Project: Sonata Engine
Author: Julien Delezenne
=============================================================================*/

#include "Win32Platform.h"
#include "Core/IO/FileStream.h"
#include "Core/IO/File.h"
#include "Core/Exception/ArgumentNullException.h"

namespace SonataEngine
{

class FileStreamInternal
{
public:
	FileStreamInternal();

public:
	File* _file;
	int32 _bufferSize;
};


FileStreamInternal::FileStreamInternal() :
	_file(NULL),
	_bufferSize(FileStream_BufferSize)
{
}


FileStream::FileStream() :
	Stream(),
	_internal(new FileStreamInternal())
{
}

FileStream::FileStream(File* file) :
	Stream(),
	_internal(new FileStreamInternal())
{
	if (file == NULL)
	{
		SEthrow(ArgumentNullException("file"));
		return;
	}

	_internal->_file = file;
}

FileStream::~FileStream()
{
	delete _internal;
}

int32 FileStream::GetLength() const
{
	if (_internal->_file == NULL)
	{
		return -1;
	}

	return ::GetFileSize((HANDLE)_internal->_file->GetHandle(), NULL);
}

void FileStream::SetLength(int32 value)
{
}

int32 FileStream::GetPosition() const
{
	if (_internal->_file == NULL)
	{
		return -1;
	}

	return ::SetFilePointer(_internal->_file->GetHandle(), 0, NULL, FILE_CURRENT);
}

void FileStream::SetPosition(int32 value)
{
	Seek(value, SeekOrigin_Begin);
}

void FileStream::Close()
{
	if (_internal->_file == NULL || !_internal->_file->IsOpen())
	{
		return;
	}

	_internal->_file->Close();
	_internal->_file = NULL;
}

void FileStream::Flush()
{
	if (_internal->_file == NULL || !_internal->_file->IsOpen())
	{
		return;
	}

	if (!::FlushFileBuffers((HANDLE)_internal->_file->GetHandle()))
	{
		SEthrow(Exception("Failed flushing the file."));
	}
}

int32 FileStream::Seek(int32 offset, SeekOrigin origin)
{
	if (_internal->_file == NULL || !_internal->_file->IsOpen())
	{
		return -1;
	}

	if (!CanSeek())
	{
		return GetPosition();
	}

	// Origin
	DWORD dwMoveMethod = FILE_CURRENT;
	if (origin == SeekOrigin_Begin)
		dwMoveMethod = FILE_BEGIN;
	else if (origin == SeekOrigin_End)
		dwMoveMethod = FILE_END;

	DWORD dwPtrLow = ::SetFilePointer((HANDLE)_internal->_file->GetHandle(), offset, NULL, dwMoveMethod);
	if (dwPtrLow == INVALID_SET_FILE_POINTER)
	{
		SEthrow(Exception("Failed setting the position within the file stream."));
		return -1;
	}

	return dwPtrLow;
}

bool FileStream::IsEOF() const
{
	return (GetPosition() == GetLength());
}

SEbyte FileStream::ReadByte()
{
	if (_internal->_file == NULL || !_internal->_file->IsOpen())
	{
		return -1;
	}

	if (!CanRead())
	{
		return -1;
	}

	SEbyte value;
	int32 count = Read(&value, 1);
	return ((count == 0 || count == EOF) ? -1 : value);
}

int32 FileStream::Read(SEbyte* buffer, int32 count)
{
	if (buffer == NULL)
	{
		return -1;
	}

	if (_internal->_file == NULL || !_internal->_file->IsOpen())
	{
		return -1;
	}

	if (!CanRead())
	{
		return -1;
	}

	DWORD dwNumberOfBytesRead;
	if (::ReadFile(_internal->_file->GetHandle(), (LPVOID) buffer, count, &dwNumberOfBytesRead, NULL) == 0)
	{
		SEthrow(Exception("Failed reading data from the file stream."));
		return -1;
	}

	return dwNumberOfBytesRead;
}

void FileStream::WriteByte(SEbyte value)
{
	if (_internal->_file == NULL || !_internal->_file->IsOpen())
	{
		return;
	}

	if (!CanWrite())
	{
		return;
	}

	Write(&value, 1);
}

int32 FileStream::Write(const SEbyte* buffer, int32 count)
{
	if (buffer == NULL)
	{
		return -1;
	}

	if (_internal->_file == NULL || !_internal->_file->IsOpen())
	{
		return -1;
	}

	if (!CanWrite())
	{
		return -1;
	}

	DWORD dwNumberOfBytesWritten;
	if (::WriteFile(_internal->_file->GetHandle(), buffer, count, &dwNumberOfBytesWritten, NULL) == 0)
	{
		SEthrow(Exception("Failed writing data to the file stream."));
		return -1;
	}

	return dwNumberOfBytesWritten;
}

String FileStream::GetFileName() const
{
	if (_internal->_file == NULL)
	{
		return String::Empty;
	}

	return _internal->_file->GetName();
}

}
